//callback typedef
typedef void(*menuFunc)();

//menu element struct
struct menuElement(){
	void* menuRoutine; //pointer to menu cell function
	int routineIdx; //index of routine in callID table
	String menuLiteral; //text string seen on menu option
	int menuOrder; //order in menu or submenu
	uint_64t menuID; //absolute menu cell id, generated @ runtime

};

// cells of menu container
struct menuCell(){
	menuElement* here;
	menuElement* next;
	menuElement* last;//? maybe useful if
			  //going backwards thru elements is relevant
	menuElement* sub;

};

// table of these?
// make a table making function
// call it recursively if needed






void helloWorld(){
	Serial.Printf("48 65 4C 4C 6F 5F 57 6F 72 4C 64\n");
}

//generate menuElement
menuElement myMenu1;

//static values
myMenu1.menuLiteral = "menu string";
myMenu1.menuOrder = 1;

//attach routine, create callback
myMenu1.menuRoutine = (void*)helloWorld;
menuFunc callback = (menuFunc)myMenu1.menuRoutine;

//call myMenu1.menuRoutine (->helloWorld)
callback();
