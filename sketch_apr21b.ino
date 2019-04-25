#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
char incomingByte = "";
int newmsg = 0;
char str[256] = "";

//Moving between screens
const int buttonPin = 6;
int buttonState = 0;
int prevState = 0;
int screen = -1;
int reply;

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
  pinMode(buttonPin, INPUT);
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

void checkbutton() {
  buttonState = digitalRead(buttonPin);
  if (buttonState != prevState){
    if (buttonState == HIGH) {
      Serial.println('w');
    }
  }
  prevState = buttonState;
}

void loop(){
        checkbutton();
        receive();
        showNewData();
        }
