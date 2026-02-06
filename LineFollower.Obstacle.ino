#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 9;
const int echoPin = 10;

const int servoPin = 6;
Servo myServo;

const int in1 = 2;
const int in2 = 3;
const int ena = 5;
const int in3 = 4;
const int in4 = 7;
const int enb = 11;

const int L = A1;
const int C = A2;
const int R = A3;

const int buzzerPin = 8;

int black = 1;
int white = 0;
const int safeDistance = 15;
const int speed = 120;
const int rspeed = 80;
const int avoidForwardTime = 400;
const int pivotTime = 400;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb, OUTPUT);

  pinMode(L, INPUT);
  pinMode(C, INPUT);
  pinMode(R, INPUT);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  myServo.attach(servoPin);
  myServo.write(90);

  stop();
}

void loop() {
  long dist = getDistance();

  int l = digitalRead(L);
  int c = digitalRead(C);
  int r = digitalRead(R);

  if (dist > 0 && dist <= safeDistance) {
    digitalWrite(buzzerPin, HIGH);
    stop();
    delay(50);
    avoidObstacle();
    digitalWrite(buzzerPin, LOW);
    return;
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  if (l == white && c == black && r == white) forward();
  else if (l == white && c == white && r == black) right();
  else if (l == black && c == white && r == white) left();
  else if (l == white && c == black && r == black) right();
  else if (l == black && c == black && r == white) left();
  else if (l == black && c == black && r == black) forward();
  else if (l == white && c == white && r == white) stop();

  delay(10);
}

void forward() {
  lcd.clear();
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ena, speed);
  analogWrite(enb, speed);
  lcd.setCursor(1, 0);
  lcd.print("Moving Forward!");
}

void left() {
  lcd.clear();
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ena, speed);
  analogWrite(enb, rspeed);
  lcd.setCursor(1, 0);
  lcd.print("Moving Left!");
}

void right() {
  lcd.clear();
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ena, rspeed);
  analogWrite(enb, speed);
  lcd.setCursor(1, 0);
  lcd.print("Moving Right!");
}

void stop() {
  lcd.clear();
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ena, 0);
  analogWrite(enb, 0);
  lcd.setCursor(1, 0);
  lcd.print("Stop!");
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2;
}

void avoidObstacle() {
  stop();
  delay(200);

  right();
  delay(pivotTime);

  forward();
  delay(avoidForwardTime);
  stop();

  while (true) {
    int l = digitalRead(L);
    int c = digitalRead(C);
    int r = digitalRead(R);

    if (l == black || c == black || r == black) break;

    left();
    delay(10);
  }
  stop();
}
