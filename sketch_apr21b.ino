#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
char incomingByte = "";
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

void loop(){
  
        // send data only when you receive data:
        while (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte);
                lcd.clear();
                lcd.setCursor(0,0);
                append(str, incomingByte);
                lcd.print(str);
                }
        }
