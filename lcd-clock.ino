/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * Hour button to digital pin 9
 * Minute button to digital pin 8
 * Time format button to digital pin 7

 */

// include the library code:
#include <LiquidCrystal.h>

const int hoursButt = 9;
const int minutesButt = 8;
const int timeFormatButt = 7;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int hour = 23;
int minute = 59;
int second = 58;

unsigned long prevTime = 0;
bool twentyFourHourTime = true;

int hoursButtPrevState = HIGH;
int minutesButtPrevState = HIGH;
int timeFormatButtPrevState = HIGH;


void displayTime() {
  //lcd.clear();
  // Start of "AM/PM" text
  //lcd.setCursor(9, 0);
  //lcd.print("  ");
	
	lcd.clear();
  lcd.home();
  
  // Check if less than 12 because 12 hour time
  // Needs to move range to 0-12
  // If twentyFourHourTime, don't need to do any
  // Subtraction to fit into 1-12 scale
  
  // Even if using 12 hour time, if hour <= 12
  // then then can reuse code, don't need to do
  // subtraction.
  // Special case, however, when hour = 0. It must be displayed
  // as twelve
  if (twentyFourHourTime || (hour != 0 && hour <= 12)) {
  	if (hour < 10) {
      lcd.print(0);
    }
    lcd.print(hour);
  } else {
    int displayHour;
    // 12 hour time, and hour greater than 12
    if (hour > 12) {
      displayHour = hour - 12;
    } else {
      displayHour = hour;
    }
    if (displayHour < 10) {
      lcd.print(0);
    }
    lcd.print(displayHour);
  }
  
  lcd.print(":");
  
  if (minute < 10) {
    lcd.print(0);
  }
  lcd.print(minute);
  lcd.print(":");
  if (second < 10) {
    lcd.print(0);
  }
  lcd.print(second);
  
  if (!twentyFourHourTime) {
    if (hour < 12) {
      lcd.print(" AM");
    } else {
      lcd.print(" PM");
    }
  }
}

// Roll over seconds, minutes, and hours
void rollOverTime() {
  if (second >= 60) {
    ++minute;
    second = 0;
  }
  if (minute >= 60) {
    ++hour;
    minute = 0;
  }
  if (hour >= 24) {
    hour = 0;
  }
}

void setup() {
  pinMode(hoursButt, INPUT_PULLUP);
  pinMode(minutesButt, INPUT_PULLUP);
  pinMode(timeFormatButt, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  
  //For UTC rollover for jank preprocessor time get.
  //hour += utcOffset;
  //if (hour < 0) {
  //    hour += 24;
  //  }
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  displayTime();
}

void loop() {
  // 1 second or more has passed
  if (millis() - prevTime >= 1000) {
    prevTime = millis();
    ++second;
    
    rollOverTime();
  
    displayTime();
  }
  
  int buttonState = digitalRead(hoursButt);
  if (buttonState != hoursButtPrevState) {
    hoursButtPrevState = buttonState;
    if (buttonState == LOW) {
      ++hour;
      rollOverTime();
      displayTime();
    }
  }
  
  buttonState = digitalRead(minutesButt);
  if (buttonState != minutesButtPrevState) {
    minutesButtPrevState = buttonState;
    if (buttonState == LOW) {
      ++minute;
      rollOverTime();
      displayTime();
    }
  }
  
  buttonState = digitalRead(timeFormatButt);
  if (buttonState != timeFormatButtPrevState) {
    timeFormatButtPrevState = buttonState;
    if (buttonState == LOW) {
      twentyFourHourTime = !twentyFourHourTime;
      displayTime();
    }
  }
  // Improve simulation performance on Tinkercad
  delay(5);
} 
