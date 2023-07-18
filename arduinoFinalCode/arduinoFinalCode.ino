#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


//Pin Initialization

//Untrasonic Pin Initialization
int sugarTrigPin = 13;
int sugarEchoPin = 12;

int bioChipTrigPin = 9;
int bioChipEchoPin = 8;

int slakelimeTrigPin = 11;
int slakelimeEchoPin = 10;

//Buzzer Pin initialization
int tonePin = 7;

//LED Pin initialization
int sugarLedPin = 2;
int bioChipLedPin = 3;
int slakelimeLedPin = 4;


//Buzzer Function
void buzzer(int delayVal) {
  tone(tonePin, 500, 700);
  delay(delayVal);
}


//Nema17 Function Need to check
void Nema17Driver(int dirPin,int stepPin,int grams){
  int stepsPerRevolution = 200;
  int actualStepToGo = ((stepsPerRevolution/360)* grams);

  // Set motor direction clockwise
	digitalWrite(dirPin, HIGH); // Change LOW or HIGh to change rotation

	// Spin motor slowly
	for(int x = 0; x < actualStepToGo; x++)
	{
		digitalWrite(stepPin, HIGH);
		delayMicroseconds(2000);
		digitalWrite(stepPin, LOW);
		delayMicroseconds(2000);
	}

}

//Ultrasonic sensors levels checking
bool isCanisterEmpty(int dis){
  int canisterEndPoint = 10; // need to check
  if(dis > canisterEndPoint){
    Serial.println("True");
    return true;
  }else{
    Serial.println("False");
    return false;
  }
}

bool levelCheck(int trigPin, int echoPin) {
  int distance;
  long duration;
  // int canisterEndPoint = 10; // need to check
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  digitalWrite(trigPin, LOW);
  delayMicroseconds(50);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(50);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.println(distance);
  // NewPing sonar(trigPin, echoPin, 400);
  // int distance = sonar.ping_cm();

  
  // if(distance > canisterEndPoint){
  //   Serial.println("True");
  //   return 1;
  // }else{
  //   Serial.println("False");
  //   return 0;
  // }
  bool status = isCanisterEmpty(distance);

  return status;
}


// display initialization
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//keypad intialization
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {41, 39, 37, 35}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {33, 31, 29, 27}; //connect to the column pinouts of the keypad

// //Create an object of keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


//Keypad Function
String number = "";
float floatValue = 0.0;
bool numberEntered = false;

float KeypadFunction() {
  char key = keypad.getKey();

  if (key) {
    if (key >= '0' && key <= '9') {
      // Append the entered digit to the number
      number += key;

      lcd.setCursor(0, 1);
      lcd.print(number);
    } else if (key == '#') {
      // Number entry complete
      floatValue = number.toFloat();
      numberEntered = true;
    } else if (key == 'C') {
      // Clear the entered number
      number = "";

      lcd.setCursor(0, 1);
      lcd.print("            ");
    } else if (key == '*') {
      // Print decimal point
      number += ".";
      
      lcd.setCursor(0, 1);
      lcd.print(number);
    }
  }

  if (numberEntered) {
    lcd.clear();
    lcd.print("Entered number:");
    lcd.setCursor(0, 1);
    lcd.print(floatValue, 2);
    numberEntered = false;
  }
}

void relay(int relayPin) {
  digitalWrite(relayPin, LOW);
}

void smallTankMixing(int dirr,int step,int grams){
  Nema17Driver(dirr,step,grams);
  relay(45); // dc motor
  delay(120000);
  digitalWrite(45, HIGH);
  relay(43); //large pump
  delay(15000); //need to check
  digitalWrite(43, HIGH);
}

void webBaseFunction(char command){
  // Serial.println("Web Based");
  Serial.println(command);
  if(command == '1'){
    // TODO: Treatment functions
    // Serial.println("Start Low PH threatment");
    smallTankMixing(A0,A1,40);
  }else if(command == '2'){
    // TODO: Treatment functions
    // Serial.println("Start High PH threatment");
    smallTankMixing(A2,A3,40);
  }else if(command == '3'){
    // TODO: Treatment functions
    // Serial.println("Start NH3 threatment");
    smallTankMixing(A5,A6,40);
  }else if(command == '4'){
    // TODO: Treatment functions
    
    if(levelCheck(bioChipTrigPin, bioChipEchoPin)==true){
      //To Do : Serial Communication
      Serial.println("Start NH3 Ultrasonic");
      lcd.setCursor(0, 0);
      lcd.print("Enter bioChip");
      for(int i=0;i<1;i++){
        buzzer(1000);
        digitalWrite(bioChipLedPin, HIGH);
        delay(1000);
        digitalWrite(bioChipLedPin, LOW);
        delay(1000);
      }
      Serial1.print('z');
    }else{
      //To Do : Serial Communication
      webBaseFunction('3');
      // Serial1.print('a');
      
    }
  }else if(command == '5'){
    // TODO: Treatment functions
    
    if(levelCheck(slakelimeTrigPin,slakelimeEchoPin)==true){
      //To Do : Serial Communication
      Serial.println("Start Low PH Ultrasonic");
      lcd.setCursor(0, 0);
      lcd.print("Enter slakelime");
      for(int i=0;i<4;i++){
        buzzer(1000);
        digitalWrite(slakelimeLedPin, HIGH);
        delay(1000);
        digitalWrite(slakelimeLedPin, LOW);
        delay(1000);
      }
      Serial1.print(4);
    }else{
      //To Do : Serial Communication
      // webBaseFunction('1');
      Serial1.print(3);
    }
  }else if(command == '6'){
    // TODO: Treatment functions
    
    if(levelCheck(sugarTrigPin,sugarEchoPin)==true){
      //To Do : Serial Communication
      Serial.println("Start High PH Ultrasonic");
      lcd.setCursor(0, 0);
      lcd.print("Enter sugar");
      for(int i=0;i<4;i++){
        buzzer(1000);
        digitalWrite(sugarLedPin, HIGH);
        delay(1000);
        digitalWrite(sugarLedPin, LOW);
        delay(1000);
      }
      Serial1.print(6);
    }else{
      //To Do : Serial Communication
      // webBaseFunction('2');
      Serial1.print(5);
    }

  }else if(command == '7'){
    // Serial.println("No treatment");
    lcd.setCursor(0,0);
    lcd.print("You do not have");
    lcd.setCursor(0,1);
    lcd.print("any treatement");

  }
  // else{
  //   Serial.println("No threatment");
  // }
}

void keypadBaseFunction(){
  // Serial.println("Keypad Based");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("1.Water I/O");
  lcd.setCursor(4, 1);
  lcd.print("System");
  delay(5000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("2.Check PH");
  delay(5000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("3.Check NH3");
  delay(5000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Enter your");
  lcd.setCursor(0, 1);
  lcd.print("choice:");
  // delay(10000);

  //Ask for function
  int key = keypad.getKey();
  while (key == NO_KEY) {
    key = keypad.getKey();
  }
  lcd.print(key - 48);
  delay(2000);

  if(key == 49){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("1.Input water to");
    lcd.setCursor(4, 1);
    lcd.print("large tank");
    delay(5000);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("2.Harvesting");
    delay(5000);
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Enter your");
    lcd.setCursor(0, 1);
    lcd.print("choice:");
    // delay(10000);
    int key1 = keypad.getKey();
    while (key1 == NO_KEY) {
      key1 = keypad.getKey();
    }
    lcd.print(key1 - 48);
    delay(2000);

  //TODO: Send number to nodemcu

    if(key1 == 49){
     Serial1.print('a');
    }else if(key == 50){
     Serial1.print('b');
    }

    }else if(key == 50){
    //TODO: Send signal to start ph treatement
     Serial1.print('c');
     
  }else if(key == 51){
    //TODO: Start nh3 treatement
     lcd.setCursor(0,0);
     lcd.print("Enter NH3 Value:");
    //  Keypad();
    //  lcd.clear();
    //  lcd.setCursor(0,0);
    //  lcd.print("NH3 Value:");
    //  lcd.setCursor(11,0);
    //  lcd.print(inputInt);
    //  delay(2000);
    //  lcd.clear();
    //  lcd.setCursor(0,0);
    //  lcd.print("Is it correct");


     Serial1.print('d'); 
  }
  


}


void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();

  //pin declare 
  pinMode(A0, OUTPUT); // Nema 1 DirPin 
  pinMode(A1, OUTPUT); // Nema 1 stepPin 
  pinMode(A2, OUTPUT); // Nema 2 Dir Pin 
  pinMode(A3, OUTPUT); // Nema 2 stepPin 
  pinMode(A4, OUTPUT); // Nema 3 DIR Pin 
  pinMode(A5, OUTPUT); // Nema 3 stepPin 
  pinMode(43, OUTPUT); // Small tank solinoid valve
  pinMode(45, OUTPUT); // water pump to small tank solinoid valve

  digitalWrite(43, HIGH); //Trun off relay channel
  digitalWrite(45, HIGH); //Trun off relay channel

  
  


}
  
void loop(){
  if(Serial1.available()){
    char command = Serial1.read();
    // Serial.println(command);
    webBaseFunction(command);
  }else{
    keypadBaseFunction();
  }
}