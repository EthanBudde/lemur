//callback typedef
typedef void(*menuFunc)();

//menu element struct
struct menuElement(){
	void* menuRoutine;
	String menuLiteral;
	int menuOrder;
};

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