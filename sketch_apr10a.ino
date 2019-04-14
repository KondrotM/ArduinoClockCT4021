#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

const int buttonPin = 6;
int buttonState = 0;
int prevState = 0;
int screen = -1;
int reply;

void setup() {
  lcd.begin(16,2);
  pinMode(buttonPin, INPUT);
  lcd.print("anotha day,");
  lcd.setCursor(0,1);
  lcd.print("anotha play -MLK");
}

void loop(){
  buttonState = digitalRead(buttonPin);
  if (buttonState != prevState) {
    if (buttonState == HIGH) {
      screen += 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      switch(screen){
      case 0:
      lcd.print("happiness is");
      lcd.setCursor(0,1);
      lcd.print(" a warm gun");
      break;
      case 1:
      lcd.print("bang, bang,");
      lcd.setCursor(0,1);
      lcd.print(" shoot, shoot");
      break;
      case 2:
      lcd.print("and when I");
      lcd.setCursor(0,1);
      lcd.print(" hold you");
      break;
      case 3:
      lcd.print("in my arms,");
      lcd.setCursor(0,1);
      lcd.print(" (oh yeah)");
      break;
      case 4:
      lcd.print("and i feel");
      lcd.setCursor(0,1);
      lcd.print(" my finger");
      break;
      case 5:
      lcd.print("on your");
      lcd.setCursor(0,1);
      lcd.print(" trigger");
      break;
      case 6:
      lcd.print("i know no-");
      lcd.setCursor(0,1);
      lcd.print(" body can do");
      break;
      case 7:
      lcd.print("me no harm");
      lcd.setCursor(0,1);
      lcd.print(" (oh yeah)");
      break;
      case 8:
      lcd.print("because");
      lcd.setCursor(0,1);
      lcd.print(" happiness");
      break;
      case 9:
      lcd.print("is a warm");
      lcd.setCursor(0,1);
      lcd.print(" gun, mama");
      break;
      case 10:
      lcd.print("happiness");
      break;
      case 11:
      lcd.print("is a warm");
      lcd.setCursor(0,1);
      lcd.print(" gun,");
      break;
      case 12:
      lcd.setCursor(0,1);
      lcd.print("yes it is");
      break;
      case 13:
      screen = -1;
      break;
      
      
      }
    }
  }
prevState = buttonState;
}
