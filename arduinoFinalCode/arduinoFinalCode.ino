#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

//Pin Initialization
//hi
//Untrasonic Pin Initialization
int sugarTrigPin = 8;
int sugarEchoPin = 9;

int bioChipTrigPin = 10;
int bioChipEchoPin = 11;

int slakelimeTrigPin = 12;
int slakelimeEchoPin = 13;

//Buzzer Pin initialization
int tonePin = 7;

//LED Pin initialization
int sugarLedPin = 4;
int bioChipLedPin = 3;
int slakelimeLedPin = 2;


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
    return true;
  }else{
    return false;
  }
}

bool levelCheck(int trigPin, int echoPin) {
  int distance;
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(50);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(50);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.println(distance);

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
// Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


// //Keypad Function
// String inputString;
// float inputInt;

// float Keypad() {
//   char key = keypad.getKey();

//   if (key) {
//     //Serial.print(key);

//     if (key >= '0' && key <= '9') {  // only act on numeric keys
//       inputString += key;            // append new character to input string
//     } else if (key == 'D') {
//       inputString.concat('.');
//     } else if (key == '#') {
//       if (inputString.length() > 0) {
//         inputInt = inputString.toFloat();  // YOU GOT AN INTEGER NUMBER
//         inputString = "";                  // clear input
//         // DO YOUR WORK HERE
//         inputInt = inputInt;
//         Serial.print(inputInt);
//         Serial.print("\n");
//         return inputInt;
//       }
//     } else if (key == '*') {

//       inputString = "";  // clear input
//     }

//     // append new character to input string
//   }
//   return 0;
// }

// void relay(int relayPin, int delay) {
//   digitalWrite(relayPin, HIGH);
//   delay(delay);
//   digitalWrite(relayPin, LOW);
//   delay(delay);
// }

void webBaseFunction(char command){
  Serial.println("Web Based");
  Serial.println(command);
  if(command == '1'){
    // TODO: Treatment functions
    Serial.println("Start Low PH threatment");
  }else if(command == '2'){
    // TODO: Treatment functions
    Serial.println("Start High PH threatment");
  }else if(command == '3'){
    // TODO: Treatment functions
    Serial.println("Start NH3 threatment");
  }else if(command == '4'){
    // TODO: Treatment functions
    Serial.println("Start NH3 Ultrasonic");
    if(levelCheck(bioChipTrigPin,bioChipEchoPin)){
      //To Do : Serial Communication
      lcd.setCursor(0, 0);
      lcd.print("Enter bioChip");
      for(int i=0;i<15;i++){
        buzzer(1000);
        digitalWrite(bioChipLedPin, HIGH);
        delay(1000);
        digitalWrite(bioChipLedPin, LOW);
        delay(1000);
      }
    }else{
      //To Do : Serial Communication
    }
  }else if(command == '5'){
    // TODO: Treatment functions
    Serial.println("Start Low PH Ultrasonic");
    if(levelCheck(slakelimeTrigPin,slakelimeEchoPin)){
      //To Do : Serial Communication
      lcd.setCursor(0, 0);
      lcd.print("Enter slakelime");
      for(int i=0;i<15;i++){
        buzzer(1000);
        digitalWrite(slakelimeLedPin, HIGH);
        delay(1000);
        digitalWrite(slakelimeLedPin, LOW);
        delay(1000);
      }
    }else{
      //To Do : Serial Communication
    }
  }else if(command == '6'){
    // TODO: Treatment functions
    Serial.println("Start High PH Ultrasonic");
    if(levelCheck(sugarTrigPin,sugarEchoPin)){
      //To Do : Serial Communication
      lcd.setCursor(0, 0);
      lcd.print("Enter sugar");
      for(int i=0;i<15;i++){
        buzzer(1000);
        digitalWrite(sugarLedPin, HIGH);
        delay(1000);
        digitalWrite(sugarLedPin, LOW);
        delay(1000);
      }
    }else{
      //To Do : Serial Communication
    }
  }else{
    Serial.println("No treatment");
  }
}

void keypadBaseFunction(){
  Serial.println("Keypad Based");
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
  }else if(key == 50){
    //TODO: Send signal to start ph treatement
  }else if(key==51){
    //TODO: Start nh3 treatement 
  }
  


}


void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();

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
    
    webBaseFunction(command);
  }else{
    keypadBaseFunction();
  }
}