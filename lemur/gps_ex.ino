
// on startup...
// RMCx5 slows RMC to every 5 cycles...
// time info may be ephemeral (warm start) time may be from a 2d fix, check GSA
sprintf(pubx40,"$PUBX,40,RMC,0,5,0,0,0,0"); //page 82
if (SetCheckSum(pubx40, sizeof(pubx40))) {
  String sPubx=pubx40;
  Serial.print(sPubx);
  gpsSerial.print(sPubx);
}

// noGSV turns off GSV a useless sentence, just because a satellite is in view does not make for fixed
sprintf(pubx40,"$PUBX,40,GSV,0,0,0,0,0,0");
if (SetCheckSum(pubx40, sizeof(pubx40))) {
  String sPubx=pubx40;
  Serial.print(sPubx);
  gpsSerial.print(sPubx);
}

bool SetCheckSum(unsigned char *pPacket, int bufferSize){
  bool bResult=false;

  //packet sent with five extra spaces -and- without an asterick 0x26 CR_A CR_B 0x0d 0x0a
  int iSize=strlen(pPacket);
  if ((iSize+5)<=bufferSize){
    //do not include $
    int iChecksum = 0;
    for (int x=1;x<iSize;x++){
      iChecksum = iChecksum ^ (byte)pPacket[x]; //xor
    }
    char Calc[3];
    sprintf(Calc,"%02X",iChecksum);
    pPacket[iSize]='*';
    pPacket[iSize+1]=Calc[0];
    pPacket[iSize+2]=Calc[1];
    pPacket[iSize+3]=0x0D;
    pPacket[iSize+4]=0x0A;
   bResult=true;
  }
  return bResult;
}

bool CheckSum(String sPacket){
  bool bResult=false;
  int iSize=sPacket.length();
  if (iSize>10){
    int iAstericks=sPacket.indexOf('*');
    if (iAstericks>0){
      String sCS=sPacket.substring(iAstericks+1,iAstericks+3);
      //xor bytes between $ and *
      sPacket=sPacket.substring(1,iAstericks);
      iSize=sPacket.length();
      int iChecksum = 0;
      for (int x=0;x<iSize;x++){
        iChecksum = iChecksum ^ (byte)sPacket[x]; //xor
      }
      //yeah, this could be better... think i got the false negatives; case and < 0x10
      char Calc[3];
      sprintf(Calc,"%02X",iChecksum);
      bResult=sCS==Calc;
      }
    }
  return bResult;
}

// in your loop...
// maintain a global buffer building sentences from chipset
// a complete packet starts with $ and ends with 0x0d 0x0a
// check the sentence checksum then process based on sentence type
