#include <Wire.h> // 1
#include <TimeLib.h> // 2 
#include <RtcDS3231.h> // 3 import modules for DS3231
#include <LiquidCrystal.h>
#define DS3231_I2C_ADDRESS 0x68
RtcDS3231<TwoWire> Rtc(Wire);
LiquidCrystal lcd(12,11,5,4,3,2); //sets up LCD screen
//RS on LCD connected to pin 12
//E connected to pin 11
//pins 2,3,4,5 are data pins

byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

char incomingByte = "";
char horoln = 'a'; //horoscope single character sent over serial
String horonm = "Aries"; //horoscope display name
int settings = 0; //turns settings on or off
char str[256] = "";
int buttonPushCounter = 0; //redundant from testing
int starsign = 0; //shows starsign on settings select screen
int screen = -1; //Moving between screens

//Setting up time
uint16_t yr = 2019; //unsigned 16-bit integer value given by clock module
uint8_t mh = 1; //unsigned 8-bit integer values given by clock module
uint8_t dy = 1;
uint8_t hr = 0;
uint8_t mn = 0;
uint8_t sd = 0;


//Setting up buttons
const int buttonPin1 = 6; //pin on arduino
int buttonState1 = 0; //current state
int prevState1 = 0; //previous state
char dscreen1 = 'w'; //redundant

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
  Serial.begin(9600); //sets baud rate
  lcd.begin(16,2); //size of lcd
  pinMode(buttonPin1, INPUT); //sets all button pin integers to actual pins
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  Rtc.Begin();
  RtcDateTime manual = RtcDateTime(yr,mh,dy,hr,mn,sd);
  Rtc.SetDateTime(manual);
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

  if (Serial.available() > 0) { //if there is a byte available to read
    rc = Serial.read(); 
    if (rc != endMarker) { //checks if byte is \n, the end marker
      receivedChars[ndx] = rc; //appends byte to word sent, increments ndx until end marker
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
  lcd.print("Weather"); 
  lcd.setCursor(0,1);
  Serial.println('w'); //sends input to python script on computer
  //receive();
  //showNewData();
}

void readDS3231time(uint8_t *second,
uint8_t *minute,
uint8_t *hour,
uint8_t *dayOfWeek,
uint8_t *dayOfMonth,
uint8_t *month,
uint16_t *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); 
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void displayTime() {
byte prevsec = 0;
bool showtime = 1;
while(showtime){
  uint8_t  second, minute, hour, dayOfWeek, dayOfMonth, month;
  uint16_t year;
  
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  if (prevsec != second){
  lcd.clear();
  lcd.setCursor(0,0);
  if (hour<10){
    lcd.print("0");
  }
  lcd.print(hour, DEC);
 
  lcd.print(":");
  if (minute<10)
  {
    lcd.print("0");
  }
  lcd.print(minute, DEC);
  lcd.print(":");
  if (second<10)
  {
    lcd.print("0");
  }
  lcd.print(second, DEC);
  lcd.setCursor(0,1);
  if (dayOfMonth<10){
    lcd.print("0");
  }
  lcd.print(dayOfMonth, DEC);
  lcd.print("/");
  if (month<10){
    lcd.print("0");
  }
  lcd.print(month, DEC);
  lcd.print("/");
  lcd.print(year, DEC);
  lcd.print(", ");
  switch(dayOfWeek){
  case 1:
    lcd.println("Sun   ");
    break;
  case 2:
    lcd.println("Mon   ");
    break;
  case 3:
    lcd.println("Tue   ");
    break;
  case 4:
    lcd.println("Wed   ");
    break;
  case 5:
    lcd.println("Thu   ");
    break;
  case 6:
    lcd.println("Fri   ");
    break;
  case 7:
    lcd.println("Sat   ");
    break;
  }
  }
  prevsec = second;
  buttonState1 = digitalRead(buttonPin1);
  if (buttonState1 != prevState1) {
    if (buttonState1 == HIGH) {
      showtime = 0;
      }
    prevState1 = buttonState1;
  }
  buttonState4 = digitalRead(buttonPin4);
    if (buttonState4 != prevState4) {
      if (buttonState4 == HIGH) {
        settings = 1; //button displays settings
        prevState4 = buttonState4; buttonState4 = digitalRead(buttonPin4);
        if (buttonState4 != prevState4) {
          if (buttonState4 == HIGH) {
            settings = 1; //button displays settings
            prevState4 = buttonState4;
            showSettings(screen);
          }
          prevState4 = buttonState4;
        }
        showSettings(screen);
      }
      prevState4 = buttonState4;
    }
}
}

void showTime() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TIME");
  lcd.setCursor(0,1);
//  Wire.beginTransmission(DS3231_I2C_ADDRESS);
//  lcd.print(decToBcd(second)
  }


void showHoroscope() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(horonm);
  lcd.setCursor(0,1);
  Serial.println(horoln); //sends input to python script on computer, 
                          //based on horoscope profile on arduino
}


char showNewData() {
  if (newData == true) {
    newData = false; //no new data once new data detected
    lcd.setCursor(0,1);
    lcd.print(receivedChars); //prints recieved data
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
  if (screen == 1) {
  int newsign = 1; //checks for new starsign input
  while (settings){ //remain in settings until settings turned off
  lcd.blink(); //visual aid for selecting starsign
  
  buttonState4 = digitalRead(buttonPin4);
  if (buttonState4 != prevState4) {
    if (buttonState4 == HIGH) {
      settings = 0; //button4 turns off settings
    }
    prevState4 = buttonState4;
  }  

  
  buttonState3 = digitalRead(buttonPin3);
  if (buttonState3 != prevState3) {
    if (buttonState3 == HIGH) {
      newsign = 1;
      starsign += 1; //increments to next sign
      }
    prevState3 = buttonState3;
  }
    
    if (newsign){
      newsign = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      switch(starsign){ //displays selected starsign, sets horoln for serial transfer
        case 0:
        horonm = "Aries";
        lcd.print(horonm);
        horoln = 'a';
        break;
        case 1:
        horonm = "Taurus";
        lcd.print(horonm);
        horoln = 'b';
        break;
        case 2:
        horonm = "Gemini";
        lcd.print(horonm);
        horoln = 'c';
        break;
        case 3:
        horonm = "Cancer";
        lcd.print(horonm);
        horoln = 'd';
        break;
        case 4:
        horonm = "Leo";
        lcd.print(horonm);
        horoln = 'e';
        break;
        case 5:
        horonm = "Virgo";
        lcd.print(horonm);;
        horoln = 'f';
        break;
        case 6:
        horonm = "Libra";
        lcd.print(horonm);
        horoln = 'g';
        break;
        case 7:
        horonm = "Scorpio";
        lcd.print(horonm);
        horoln = 'h';
        break;
        case 8:
        horonm = "Sagittarius";
        lcd.print(horonm);
        horoln = 'i';
        break;
        case 9:
        horonm = "Capricorn";
        lcd.print(horonm);
        horoln = 'j';
        break;
        case 10:
        horonm = "Aquarius";
        lcd.print(horonm);
        horoln = 'k';
        break;
        case 11:
        horonm = "Pisces";
        lcd.print(horonm);
        horoln = 'l';
        starsign = -1;
      }
    }
  }
  //once settings turned off
  lcd.noBlink(); //blink aid stops
  Serial.println(horoln); //saves last selected starsign
  }
  if (screen = 2){
    lcd.blink ();
    int init = 1;
    int timescreen = -1;
    uint8_t second, minute, hour, dayOfWeek, dayOfMonth, month;
    uint16_t year;
    readDS3231time(&sd, &mn, &hr, &dayOfWeek, &dy, &mn, &yr);
  //  yr = year; mh = month; dy = dayofMonth; hr = hour; mn = minute; sd = second;
    while(settings){
      buttonState1 = digitalRead(buttonPin1);
      if (buttonState1 != prevState1 || init) {
        if (buttonState1 == HIGH || init) {
          init = 0;
          timescreen ++;
          switch(timescreen){
            case 0:
            lcd.setCursor(2,0);
            break;
            case 1:
            lcd.setCursor(5,0);
            break;
            case 2:
            lcd.setCursor(8,0);
            break;
            case 3:
            lcd.setCursor(2,1);
            break;
            case 4:
            lcd.setCursor(5,1);
            break;
            case 5:
            lcd.setCursor(8,1);
            timescreen = -1;
          }
          //timescreen += 1
      
          }
          prevState1 = buttonState1;
      }
      
      buttonState4 = digitalRead(buttonPin4);
      if (buttonState4 != prevState4) {
        if (buttonState4 == HIGH) {
          settings = 0; //button4 turns off settings
        }
      prevState4 = buttonState4;
     }  
    
    }
    lcd.noBlink();
    RtcDateTime manual = RtcDateTime(yr,mn,dy,hr,mn,sd);
    Rtc.SetDateTime(manual);
  }

}

void scrolltext(char line2){
int stringStart, stringStop = 0;
int scrollCursor = 16;
      
}
bool isloop = 0;
void loop(){
        buttonState1 = digitalRead(buttonPin1);
        if (buttonState1 != prevState1) {
          if (buttonState1 == HIGH || isloop) {
            isloop = 0;
            screen += 1; //button cylcles screens on clock
            switch(screen){
              case 0:
              showWeather();
              break;
              case 1:
              showHoroscope();
              break;
              case 2:
              prevState1 = buttonState1;
              displayTime();
              isloop = 1; //since the previous buttonpress was done to come out of the displayTime() loop,
                          //an extra prompt is needed to loop the screen without an extra button press
              screen = -1; //loops back to first screen
            }
          }
          prevState1 = buttonState1;
        }
        
        buttonState4 = digitalRead(buttonPin4);
        if (buttonState4 != prevState4) {
          if (buttonState4 == HIGH) {
            settings = 1; //button displays settings
            prevState4 = buttonState4;
            showSettings(screen);
          }
          prevState4 = buttonState4;
        }
        receive(); //any requested data is sent
        showNewData(); //sent data displayed on bottom text
  }
