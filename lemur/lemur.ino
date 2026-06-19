//lemur
//open source bike computer designed for esp32 
//by ugly[ethan budde; budde@pdx.edu]

// includes
// screen
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define MENUSCALE 2 

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< datasheet supposedly says 0x3D -> 128x64, 0x3C -> 128x32
                            // idk why 3C works but it does


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON1 14
#define BUTTON2 15

int lastButtonReading[2];
int stableButtonState[2] = {1, 1};
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
int reading[2];

struct menuRoutine{
  void *routine;
  String menuText;
};

int fib = 1;
int n = 0;
int light[5] = {15,14,13,11,7};
  
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };


static const unsigned char PROGMEM lemur_logo[] = 
{ 0b00000000, 0b00000000,
  0b00000111, 0b11110000,
  0b00000011, 0b11000000,
  0b00000011, 0b10000000,
  0b00000011, 0b10000000,
  0b00000011, 0b10000000,
  0b00000111, 0b00000000,
  0b00000111, 0b00001100,
  0b00000111, 0b01101100,
  0b00000111, 0b01100000,
  0b00001110, 0b00000000,
  0b00001110, 0b00000100,
  0b00001110, 0b00001100,
  0b00011110, 0b00111000,
  0b00111111, 0b11111000,
  0b00000000, 0b00000000 };

void setup() {
  Serial.begin(9600);

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  // Wait for display
  delay(500);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  testdrawbitmap();    // Draw a small bitmap image

  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  delay(1000);
}



void loop() {
  display.clearDisplay();

  fib = fib+fib;
  
  if(fib>10000000){
    fib =1;
  }
  readButton(BUTTON1, 1);
  readButton(BUTTON2, 2);

  String top = "testing!";
  String bt1 = String(rand());
  String bt2 = String(fib);
  String bot = String(millis());
  
  setframe(light[n], top, bt1, bt2, bot);
  
  display.display();
  delay(50);
}

void readButton(int pin, int work){
  reading[work] = digitalRead(pin);
  /*
  Serial.print("button"); Serial.print(work); Serial.print(":");  
  Serial.println(reading[work]);
  
  Serial.print("lastrd"); Serial.print(work); Serial.print(":");  
  Serial.println(lastButtonReading[work]);

  Serial.print("stable"); Serial.print(work); Serial.print(":");  
  Serial.println(stableButtonState[work]);
  //bottom comments here

  if (reading[work] != lastButtonReading[work]) {
    lastDebounceTime = millis();
  }
  */
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading[work] != stableButtonState[work]) {
      stableButtonState[work] = reading[work];

      if (stableButtonState[work] == LOW) {
        switch(pin){
          case(BUTTON1):
              n+=1;
              if(n==5){
                n=0;
              }
            break;
          case(BUTTON2):
            
            break;
          default:
            Serial.println("undefined button input detected.");
            break;
        }
      } 
    }
  }

  lastButtonReading[work] = reading[work];	
}


String buttonPrint(int pin){
	int value = digitalRead(pin);
	String prefix = String(pin);
	String worker = (value) ? "off" : "on" ;
	String print = prefix+","+worker;
	return print;    
}
  

// TODO:ARBITRARY FRAME SIZES
//    NEED FRAME SIZE, SCROLLING TO ACCOUNT
//    AUTORETURN TO TOP

void setframe(byte select, String upper, String midUpper, String midLower, String lower){	
	int light[3];
  for (int i = 3; i >= 0; i--){
    light[i] = bitRead(select, i);
  }
  
  arbPrint(MENUSCALE, light[0], 0, 0, upper);
	arbPrint(MENUSCALE, light[1], 0, 16, midUpper);
	arbPrint(MENUSCALE, light[2], 0, 32, midLower);
	arbPrint(MENUSCALE, light[3], 0, 48, lower);
}

void arbPrint(int scale, int color, int x, int y, String text){
  display.setTextSize(scale);         
  if(color == 0){
    display.setTextColor(0,1);
  }else if(color == 1){
    display.setTextColor(1);
  }
  display.setCursor(x,y);             
  display.println(text);
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(1);    // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  //display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    lemur_logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}


/*
menuing
network
gpsMgr
sensors
    for barometric pressure, other sensor integration
gfx







*/




// THEORY
//loop
//  state check {debug shit by the end}
//  button check
//    if first, hold condition
//      if yes, trip enable/disable (trip off on startup)
//      if no, page displayed screen
//    if second, run menu routine
//  trip? -> if yes, update trip metrics
//  update base metrics
//  print, loop

//menu routine
//  maintain passive stats (total distance, trip time, etc)
//  print top menu
//  button check
//    if first, enter selected menu option, print
//    if second, page down 1 menu option, print

//menu design            should this be menu youre on? menu you return to? configurable?
//  __________________   |         __________________              __________________
// |-> return      TOP| <-        |->ock settings    |            |->ging cfg        |
// |clock settings    | button 2  |paging cfg        | button 2   |gps control       |
// |paging cfg        |    ->     |gps control       |    ->      |memory cfg        |
// |gps_control_______|           |memory_cfg________|            |battery_cfg_______|
//   \|/ button 1 \|/               \|/ button 1 \|/                \|/ button 1 \|/
//  __________________             __________________              __________________
// |12:34          72%|           |-> return         |            |-> return         |           
// |  26.45   2:30 hr | button 2  |retry clock sync  | button 2   |order/list        |
// |  miles    trip   |    ->     |timezone set      |    ->      |page design       |
// |__________________|           |formatting________|            |reset_pages_______|

//menu tree
//debug
//  overrides, testings, etc
//clock settings
//  retry clock sync
//  timezone set {override, saved?}
//  formatting
//    [standard]->[24hour]->
//paging cfg
//  order/list
//    page 1
//    page 2
//    ...
//    page n
//      up
//      down
//      top
//      bottom
//  page design
//    page 1
//    page 2
//    ...
//    page n
//      panel amount
//        [1] -> [2] ->
//      panel 1 select {left if 2 panel}
//        [TIME] -> [TRIP TIME] -> [TIME/TRIPTIME] -> [MOVING SPEED] -> [TOP SPEED] -> [MOVING/TOP] -> [AVG SPEED] -> [MOVING/AVG] -> [TEMPERATURE] -> [HUMIDITY] -> [TEMP/HUMID] -> [AGGREGATE ELEV] -> [CLIMB] -> [COAST] -> [CIMB/COAST] -> [AVG MILE TIME] -> [GPS COORDS] -> [CARDINAL DIR]
//      panel 2 select {right if 2 panel, off if 1 panel}
//        ''
//      topbar toggle
//        [TOPBAR ON] -> [TOPBAR OFF] ->
//  reset pages
//    u sure?
//  page toggling
//    type
//      [BUTTON] -> [SCROLL, BUTTON SKIP] -> [LOCKED SCROLL] ->
//    scroll delay
//      in s
//gps control
//  reping position
//  ...
//memory cfg
//  capacity
//  trip save rules
//    save prompt
//      [ON] -> [OFF] ->
//    type
//      [APPEND UNTIL FULL] -> [KEEP LAST X] -> [DELETE AFTER X WEEKS] -> [DELETE ON RESET] -> 
//  x {keep last/weeks timer}
//    int
//  replace logic
//    [DO NOT REPLACE] -> [END TRIP] -> [DELETE OLDEST, APPEND] -> [DELETE LARGEST, APPEND] ->
//battery cfg
//  stats
//  ...
//topbar cfg
//  design
//    left
//    center
//    right
//      toggle
//        [ON] -> [OFF] ->
//      value
//        [TIME] -> [TRIP TIME] -> [MOVING SPEED] -> [TOP SPEED] -> [AVG SPEED] -> [TEMPERATURE] -> [HUMIDITY] -> [AGGREGATE ELEV] -> [CLIMB] -> [COAST] -> [AVG MILE TIME] -> [CARDINAL DIR]
//  override
//    [ALWAYS DISPLAY] -> [PAGE RULE]
//screen cfg
//  brightness
//  test pattern
//  ...
//settings cfg
//  order
//  ...
         
