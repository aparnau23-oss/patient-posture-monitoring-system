#include <Servo.h>
#include <LiquidCrystal.h>

// LCD setup: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

Servo postureServo;

// Pin assignments
const int potPin = A0;
const int buzzerPin = 8;
const int servoPin = 9;
const int ledPin = 7;

// Posture monitoring
int postureValue = 0;
const int lowerLimit = 400;
const int upperLimit = 600;

unsigned long badPostureStart = 0;
bool inBadPosture = false;

const unsigned long alertDelay = 10000; // 10 seconds

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  postureServo.attach(servoPin);

  lcd.begin(16, 2);

  Serial.begin(9600);
}

void loop() {

  postureValue = analogRead(potPin);

  Serial.print("Posture Value: ");
  Serial.println(postureValue);

  lcd.setCursor(0, 0);
  lcd.print("Posture: ");
  lcd.print(postureValue);
  lcd.print(" ");

  // Detect bad posture
  if (postureValue < lowerLimit || postureValue > upperLimit) {

    if (!inBadPosture) {
      badPostureStart = millis();
      inBadPosture = true;
    }

    if (millis() - badPostureStart >= alertDelay) {

      digitalWrite(buzzerPin, HIGH);
      digitalWrite(ledPin, HIGH);

      postureServo.write(90);

      lcd.setCursor(0, 1);
      lcd.print("Bad posture! ");
    }
    else {

      lcd.setCursor(0, 1);
      lcd.print("Hold steady... ");
    }

  } else {

    // Good posture
    inBadPosture = false;
    badPostureStart = 0;

    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);

    postureServo.write(0);

    lcd.setCursor(0, 1);
    lcd.print("Posture OK ");
  }

  delay(200);
}
