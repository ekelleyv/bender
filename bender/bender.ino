#include <Servo.h>
#include <LiquidCrystal.h>

// topServo (pours beer)
Servo topServo;  // create servo object to control a servo
const int TOP_SERVO_PIN = 9;
const int TOP_SERVO_LEVEL_ANGLE = 140;
const int TOP_SERVO_POUR_MAX_ANGLE = 30;
int topServoPosition = TOP_SERVO_LEVEL_ANGLE;

// bottomServo (rotates boom)
Servo bottomServo;
const int BOTTOM_SERVO_PIN = 10;
const int BOTTOM_SERVO_DRINKING_POSITION = 150;
const int BOTTOM_SERVO_READY_POSITION = 40;
int bottomServoPosition = BOTTOM_SERVO_READY_POSITION;


// screen
const int LCD_RS = 12;
const int LCD_ENABLE = 11;
const int LCD_D4 = 5;
const int LCD_D5 = 4;
const int LCD_D6 = 3;
const int LCD_D7 = 7;
LiquidCrystal LCD(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// button
const int BUTTON_PIN = 2;
int buttonState = 0;

void setup() {
  topServo.attach(TOP_SERVO_PIN);
  bottomServo.attach(BOTTOM_SERVO_PIN);

  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT);

  // init positions
  printToBanner("PRESS FOR BEER");
  Serial.println("Setting servo to initial position");
  topServo.write(TOP_SERVO_LEVEL_ANGLE);
  bottomServo.write(BOTTOM_SERVO_READY_POSITION);
  delay(1000);
  Serial.println("Set to initial position");

}

int moveTo(Servo servo, int currentPosition, int desiredPosition) {
  Serial.println("===========");
  Serial.println(currentPosition);
  Serial.println(desiredPosition);
  Serial.println("===========");
  int rate = 1;
  if (currentPosition < desiredPosition) {
    Serial.println("current less than");
    while (currentPosition < desiredPosition) {
      Serial.println(currentPosition);
      servo.write(currentPosition);
      currentPosition = currentPosition + rate;
      delay(30);
    }
  }
  else if (currentPosition > desiredPosition) {
    Serial.println("current greater than");
    while (currentPosition > desiredPosition) {
      Serial.println(currentPosition);
      servo.write(currentPosition);
      currentPosition = currentPosition - rate;
      delay(30);
    }
  }
  return currentPosition;
}

void printToBanner(char text[]) {
  LCD.begin(16, 2);
  LCD.print(text);
}

void beerCountdown(int drinkingTime) {
  printToBanner("TIME FOR BEER IN");
  LCD.setCursor(0, 1);

  for( int i = 3; i > 0; i-- ){
      LCD.setCursor(0, 1);
      LCD.print(i);
      delay(1000);
   }
   printToBanner("BEER TIME");
}

void pourBeer() {
  // Set to max pour
  // init
  printToBanner("DISPENSING BEER");
  bottomServoPosition = moveTo(bottomServo, bottomServoPosition, BOTTOM_SERVO_DRINKING_POSITION);
  topServoPosition = moveTo(topServo, topServoPosition, (TOP_SERVO_POUR_MAX_ANGLE - TOP_SERVO_LEVEL_ANGLE) * 0.75 + TOP_SERVO_LEVEL_ANGLE);

  printToBanner("BEER DISPENSED");
  topServoPosition = moveTo(topServo, topServoPosition, TOP_SERVO_LEVEL_ANGLE);
  bottomServoPosition = moveTo(bottomServo, bottomServoPosition, BOTTOM_SERVO_READY_POSITION);
}

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH) {
    pourBeer();
    delay(1000);
    printToBanner("YUM");
    delay(1000);
    printToBanner("PRESS FOR BEER");
  }
}

