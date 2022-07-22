#include "MQ135.h"
#include <Adafruit_BMP085.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial sim(9, 10);
String number = "+8801717081691";
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define light 13
#define pump 4
const int AirValue = 550;
const int WaterValue = 470;

int soilMoistureValue = 0;
int soilmoisturepercent = 0;
Adafruit_BMP085 bmp;
Servo myservo;
int pos = 0;
void setup() {
  lcd.init();
  pinMode(pump, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(light, OUTPUT);
  myservo.attach(10);
  myservo.write(90);
}
void loop() {
  int r = button_read();
  if (r != 0) {
    if (r == 1) soil();
  }

  r = button_read_2();
  if (r != 0) {
    if (r == 1) gsm();
  }
}
void motor() {
  digitalWrite(pump, HIGH);
  delay(500);
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(10);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(10);
  }
}
void motoroff() {
  digitalWrite(pump, LOW);
  myservo.write(90);
}
int button_read() {
  int cl = 0;
p:  int t = 0;
  if (digitalRead(2) == 0) {
    digitalWrite(light, HIGH);
    while (digitalRead(2) == 0) {
      delay(1); t++;
    }
    digitalWrite(light, LOW);
    if (t > 15) {
      t = 0; cl++;
      while (digitalRead(2) == HIGH) {
        delay(1); t++;
        if (t > 1000) return cl;
      }
      goto p;
    }
  }
  return cl;
}
int button_read_2() {
  int cl = 0;
p:  int t = 0;
  if (digitalRead(12) == 0) {
    digitalWrite(light, HIGH);
    while (digitalRead(12) == 0) {
      delay(1); t++;
    }
    digitalWrite(light, LOW);
    if (t > 15) {
      t = 0; cl++;
      while (digitalRead(12) == HIGH) {
        delay(1); t++;
        if (t > 1000) return cl;
      }
      goto p;
    }
  }
  return cl;
}
