#include <Wire.h> // 1
#include <TimeLib.h> // 2 
#include <RtcDS3231.h> // 3 import modules for DS3231
#include <LiquidCrystal.h>

// allows communication to DS3231 - code based from https://forum.arduino.cc/index.php?topic=398891.0
#define DS3231_I2C_ADDRESS 0x68
RtcDS3231<TwoWire> Rtc(Wire);

//sets up LCD screen - code based on built-in arduino examples
LiquidCrystal lcd(12,11,5,4,3,2); 
                                  // RS on LCD connected to pin 12
                                  // E connected to pin 11
                                  // pins 2,3,4,5 are data pins
                                  
byte decToBcd(byte val) {
  return( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val) {
  return( (val/16*10) + (val%16) );
}

char horoln = 'a'; // horoscope single character sent over serial
String horonm = "Aries"; // horoscope display name
int settings = 0; // turns settings on or off
char str[256] = "";
int starsign = 0; // shows starsign on settings select screen
int screen = -1; // Moving between screens

// Setting up time
uint16_t yr = 2019; // unsigned 16-bit integer value given by clock module
uint8_t mh = 1, // unsigned 8-bit integer values given by clock module
dy = 1, hr = 0, mn = 0, sd = 0;

// Setting up buttons
const int buttonPin1 = 6; // pin on arduino
int buttonState1 = 0; // current state
int prevState1 = 0; // previous state

const int buttonPin2 = 7;
int buttonState2 = 0;
int prevState2 = 0;

const int buttonPin3 = 8;
int buttonState3 = 0;
int prevState3 = 0;

const int buttonPin4 = 9;
int buttonState4 = 0;
int prevState4 = 0;

// Show words from serial - code based from http://forum.arduino.cc/index.php?topic=353219.0
char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

// Receive with end marker - code based from https://forum.arduino.cc/index.php?topic=288234.0
const byte numChars = 32;
char receivedChars[numChars]; // array to store receieved data

boolean newData = false; // later becomes true if any new serial data is sent

void setup() {
  Serial.begin(9600); //sets baud rate
  lcd.begin(16,2); //size of lcd
  pinMode(buttonPin1, INPUT); //sets all button pin integers to actual pins
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  Rtc.Begin(); //enables communication to Real Time Clock
  RtcDateTime manual = RtcDateTime(yr,mh,dy,hr,mn,sd); // creates RTC-compliant variable with local time
  Rtc.SetDateTime(manual); // sends local time to RTC
  
  // print startup message
  lcd.print("Matej Kondrot");
  lcd.setCursor(0,1);
  lcd.print("CT4021, :)");
}

// code based off arduino serial basics - https://forum.arduino.cc/index.php?topic=396450.0
void receive() {
  static byte ndx = 0; // byte which is used to create string
  char endMarker = '\n'; // line break 
  char rc; // individual serial character 

  if (Serial.available() > 0) { //if there is a byte available to read
    rc = Serial.read(); 
    if (rc != endMarker) { //checks if byte is \n, the end marker
      receivedChars[ndx] = rc; //appends byte to word sent, increments ndx until line break
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
}

// code based off http://combustory.com/wiki/index.php/RTC1307_-_Real_Time_Clock
// this code is not meant for the RTC module used, and can likely be optimised,
// but with minor modifications (byte variables to unsigned 8-bit integer),
// it still works and i'm afraid to break it
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
bool showtime = 1; //refreshes the screen each second rather than only on first button press
while(showtime){   // ^
  uint8_t  second, minute, hour, dayOfWeek, dayOfMonth, month;
  uint16_t year;
  
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year); // grabs time from previous function
  if (prevsec != second){
  lcd.clear();
  lcd.setCursor(0,0);
  if (hour<10){ // integers below 10 are single-digit. to make the screen look nice an extra "0" is printed to make each number two-digit.
    lcd.print("0"); //prints whole date and time, keeping each time value seperate.
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
  switch(dayOfWeek){ // prints day of the week
  case 1:
    lcd.print("Mon"); // days moved up one position to account for (possible) hardware error
    break;            // maybe it was intended this way
  case 2:
    lcd.print("Tue");
    break;
  case 3:
    lcd.print("Wed");
    break;
  case 4:
    lcd.print("Thu");
    break;
  case 5:
    lcd.print("Fri");
    break;
  case 6:
    lcd.print("Sat");
    break;
  case 7:
    lcd.print("Sun");
    break;
  }
  }
  prevsec = second; // updates each second rather than each loop
  buttonState1 = digitalRead(buttonPin1);
  if (buttonState1 != prevState1) {
    if (buttonState1 == HIGH) {
      showtime = 0; // breaks loop, next screen displayed
      }
    prevState1 = buttonState1;
  }
  buttonState4 = digitalRead(buttonPin4);
    if (buttonState4 != prevState4) {
      if (buttonState4 == HIGH) {
        settings = 1; // settings loop enabled
        prevState4 = buttonState4;
        showSettings(screen); //opens settings based on which screen is enabled (time settings in this case)
      }
      prevState4 = buttonState4;
    }
}
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
    lcd.blink (); // caret blinks to indicate settings mode
    int init = 1; // first settings option is entered immediately on startup
    int timeval = 0; // designates which time value is being edited
    int timescreen = -1; // cycles between screens
    uint8_t second, minute, hour, dayOfWeek, dayOfMonth, month;
    uint16_t year;
    readDS3231time(&sd, &mn, &hr, &dayOfWeek, &dy, &mh, &yr); // grabs temporary time settings
    while(settings){
      buttonState1 = digitalRead(buttonPin1); // cycles between screens
      if (buttonState1 != prevState1 || init) {
        if (buttonState1 == HIGH || init) {
          init = 0;
          timescreen ++;
          switch(timescreen){
            case 0:
            lcd.setCursor(2,0); // cursor positioning and blinking caret indicates which value is being edited
            timeval = 0; // used to determine which value is being edited when button3 is pressed
            break;
            case 1:
            lcd.setCursor(5,0);
            timeval = 1;
            break;
            case 2:
            lcd.setCursor(8,0);
            timeval = 2;
            break;
            case 3:
            lcd.setCursor(2,1);
            timeval = 3;
            break;
            case 4:
            lcd.setCursor(5,1);
            timeval = 4;
            break;
            case 5:
            lcd.setCursor(8,1);
            timeval = 5;
            timescreen = -1; // cycles back to the start
          }
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

     buttonState3 = digitalRead(buttonPin3);
      if (buttonState3 != prevState3) {
        if (buttonState3 == HIGH) {
          switch(timeval){ // increments selected time value
            case 0:
            lcd.setCursor(0,0); // sets cursor to overwrite previous value on screen
            hr ++; // increments value
            if (hr > 23){ // if variable is too great for datetime, cycles back to 0,
              hr = 0;
            }
            if (hr < 10){     // if number is 1 digit
              lcd.print("0"); // prints an extra digit so each number has two digits
            }
            lcd.print(hr); // prints hour number
            break; // breaks switch loop until button is pressed again
            case 1: // repeat previous case with minute value, etc.
            lcd.setCursor(3,0);
            mn ++;
            if (mn > 59){
              mn = 0;
            }
            if (mn < 10) {
              lcd.print("0");
            }
            lcd.print(mn);
            break;
            case 2:
            lcd.setCursor(6,0);
            sd ++;
            if (sd > 59){
              sd = 0;
            }
            if (sd < 10){
              lcd.print("0");
            }
            lcd.print(sd);
            break;
            case 3:
            lcd.setCursor(0,1);
            dy ++;
            if (dy > 31){
              dy = 0;
            }
            if (dy < 10){
              lcd.print("0");
            }
            lcd.print(dy);
            break;
            case 4:
            lcd.setCursor(3,1);
            mh ++;
            if (mh > 12){
              mh = 1;
            }
            if (mh < 10){
              lcd.print("0");
            }
            lcd.print(mh);
            break;
            case 5:
            lcd.setCursor(6,1);
            yr++;
            if (yr < 10){
              lcd.print("0");
            }
            lcd.print(yr);
          }
        }
      prevState3 = buttonState3;
     }  
    
    }
    // once settings have been exit
    lcd.noBlink(); // caret stops blinking
    RtcDateTime manual = RtcDateTime(yr,mh,dy,hr,mn,sd); // time set in settings becomes real local time
    Rtc.SetDateTime(manual); // new local time is sent to clock module
  }

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
