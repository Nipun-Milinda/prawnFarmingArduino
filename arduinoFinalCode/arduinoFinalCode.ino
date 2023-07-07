#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


//Pin Initialization
//hi
//Untrasonic Pin Initialization
int sugarTrigPin = 9;
int sugarEchoPin = 8;

int bioChipTrigPin = 11;
int bioChipEchoPin = 10;

int slakelimeTrigPin = 13;
int slakelimeEchoPin = 12;

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

void webBaseFunction(char command){
  // Serial.println("Web Based");
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
      Serial1.print('a');
      
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
    // Serial.println(command);
    webBaseFunction(command);
  }else{
    keypadBaseFunction();
  }
}