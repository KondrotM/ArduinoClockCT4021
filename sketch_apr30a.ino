#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
char incomingByte = "";
int settings = 0;
int newmsg = 0;
char str[256] = "";
int buttonPushCounter = 0;
int starsign = 0;

//Moving between screens
int screen = -1;
int reply;

//Setting up buttons
const int buttonPin1 = 6;
int buttonState1 = 0;
int prevState1 = 0;
char dscreen1 = 'w';

const int buttonPin2 = 7;
int buttonState2 = 0;
int prevState2 = 0;
char dscreen2 = 'h';

const int buttonPin3 = 8;
int buttonState3 = 0;
int prevState3 = 0;
char dscreen3 = "t";

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

void showWeather() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WEATHER");
  lcd.setCursor(0,1);
  Serial.println('w');
  //receive();
  //showNewData();
}

void showTime() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TIME");
  lcd.setCursor(0,1);
  Serial.println('t');
  }


void showHoroscope() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HOROSCOPE");
  lcd.setCursor(0,1);
  Serial.println('h');
}


char showNewData() {
  if (newData == true) {
    newData = false;
    lcd.setCursor(0,1);
    //Serial.println('a');
    lcd.print(receivedChars);
  }
}

int checkbutton(int buttonState,int buttonPin, int prevState, char dscreen) {
  buttonState = digitalRead(buttonPin);
  if (buttonState != prevState){
    if (buttonState == HIGH) {
      buttonPushCounter++;
      Serial.println(buttonPushCounter);
    }
  }
  return buttonState;

}

void showSettings(int screen){
  int newsign = 1;
  while (settings){
  lcd.blink();
  
  buttonState4 = digitalRead(buttonPin4);
  if (buttonState4 != prevState4) {
    if (buttonState4 == HIGH) {
      settings = 0;
    }
    prevState4 = buttonState4;
  }  

  
  buttonState3 = digitalRead(buttonPin3);
  if (buttonState3 != prevState3) {
    if (buttonState3 == HIGH) {
      newsign = 1;
      starsign += 1;
      }
    prevState3 = buttonState3;
  }
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 != prevState2) {
    if (buttonState2 == HIGH) {
      newsign = 1;
      starsign -= 1;
      }
  }
    prevState2 = buttonState2;
    
    if (newsign){
      newsign = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      switch(starsign){
        case 0:
        lcd.print("Aries");
        break;
        case 1:
        lcd.print("Taurus");
        break;
        case 2:
        lcd.print("Gemini");
        break;
        case 3:
        lcd.print("Cancer");
        break;
        case 4:
        lcd.print("Leo");
        break;
        case 5:
        lcd.print("Virgo");
        break;
        case 6:
        lcd.print("Libra");
        break;
        case 7:
        lcd.print("Scorpio");
        break;
        case 8:
        lcd.print("Sagittarius");
        break;
        case 9:
        lcd.print("Capricorn");
        break;
        case 10:
        lcd.print("Aquarius");
        break;
        case 11:
        lcd.print("Pisces");
        case -1:
        starsign = 11;
        case 12:
        starsign = -1;
      }
    }
  }
  lcd.noBlink();
  }



void scrolltext(char line2){
int stringStart, stringStop = 0;
int scrollCursor = 16;
      
}

void loop(){
        buttonState1 = digitalRead(buttonPin1);
        if (buttonState1 != prevState1) {
          if (buttonState1 == HIGH) {
            screen += 1;
            switch(screen){
              case 0:
              showWeather();
              break;
              case 1:
              showHoroscope();
              break;
              case 2:
              showTime();
              screen = -1;
            }
          }
          prevState1 = buttonState1;
        }
        
        buttonState4 = digitalRead(buttonPin4);
        if (buttonState4 != prevState4) {
          if (buttonState4 == HIGH) {
            settings = 1;
            prevState4 = buttonState4;
            showSettings(screen);
          }
          prevState4 = buttonState4;
        }
        receive();
        showNewData();
  }
