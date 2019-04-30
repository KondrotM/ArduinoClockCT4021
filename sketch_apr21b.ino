#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
char incomingByte = "";
int newmsg = 0;
char str[256] = "";
int buttonPushCounter = 0;
//Moving between screens
const int buttonPin1 = 6;
int buttonState1 = 0;
int prevState1 = 0;
char dscreen1 = 'w';
int screen = -1;
int reply;

//Moving between
const int buttonPin2 = 7;
int buttonState2 = 0;
int prevState2 = 0;
char dscreen2 = 'h';

//Moving between
const int buttonPin3 = 8;
int buttonState3 = 0;
int prevState3 = 0;
char dscreen3 = "t";

//Moving between 
const int buttonPin4 = 9;
int buttonState4 = 0;
int prevState4 = 0;
char dscreen4 = 's';



//Show words from serial
char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

//Receive with end marker
const byte numChars = 32;
char receivedChars[numChars]; //array to store receieved data

boolean newData = false;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  lcd.print("Enter data through");
  lcd.setCursor(0,1);
  lcd.print("serial port :)");
}

//https://ubuntuforums.org/showthread.php?t=1016188
void append(char* s, char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

void weather() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WEATHER");
  lcd.setCursor(0,1);
  lcd.print("");
}

void receive() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  if (Serial.available() > 0) {
    rc = Serial.read();
    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars){
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData() {
  if (newData == true) {
    newData = false;
    lcd.clear();
    lcd.setCursor(0,1);
    //Serial.println('a');
    lcd.print(receivedChars);
  }
}

void checkbutton(int buttonState,int buttonPin, int prevState, char dscreen) {
  buttonState = digitalRead(buttonPin);
  if (buttonState != prevState){
    if (buttonState == HIGH) {
      buttonPushCounter++;
      Serial.println(buttonPushCounter);
    }
  }
  prevState = buttonState;
}

void loop(){
        checkbutton(buttonState1, buttonPin1, prevState1, dscreen1);
        checkbutton(buttonState2, buttonPin2, prevState2, dscreen2);
        checkbutton(buttonState3, buttonPin3, prevState3, dscreen3);
        checkbutton(buttonState4, buttonPin4, prevState4, dscreen4);
        receive();
        showNewData();
        }
