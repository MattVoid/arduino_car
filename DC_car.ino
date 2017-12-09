#include <AFMotor.h>
#include <Servo.h>
#include "SR04.h"
#include "IRremote.h"

AF_DCMotor m1 (1);
AF_DCMotor m2 (2);
AF_DCMotor m3 (3);
AF_DCMotor m4 (4);

#define TRIG_PIN A1
#define ECHO_PIN A0

#define S1 16724175
#define S2 16718055
#define A 16736925
#define I 16754775
#define S 16720605
#define D 16761405
#define F 16712445

#define UNKNOWN_1 2534850111
#define UNKNOWN_2 1033561079
#define UNKNOWN_F 3622325019
#define UNKNOWN_A 5316027
#define UNKNOWN_I 2747854299
#define UNKNOWN_D 553536955
#define UNKNOWN_S 1386468383
Servo s1;

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long a;
int x;
int y;
int pos = 90;
int receiver = A2;
unsigned long val;
int set = 1;

IRrecv irrecv(receiver);
decode_results results;

void setup() {
  s1.attach(9);
  delay(1000);
  Serial.begin(9600);
  s1.write(90);
  irrecv.enableIRIn();
  pinMode(11, OUTPUT); //M1
  pinMode(3, OUTPUT); //M2
  pinMode(6, OUTPUT); //M3
  pinMode(5, OUTPUT); //M4
  digitalWrite(11, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
}

void indietro() {
  m1.run(BACKWARD);
  m1.setSpeed(250);
  m2.run(BACKWARD);
  m2.setSpeed(250);
  m3.run(BACKWARD);
  m3.setSpeed(250);
  m4.run(BACKWARD);
  m4.setSpeed(250);
  delay(1000);
}

void avanti() {
  m1.run(FORWARD);
  m1.setSpeed(250);
  m2.run(FORWARD);
  m2.setSpeed(250);
  m3.run(FORWARD);
  m3.setSpeed(250);
  m4.run(FORWARD);
  m4.setSpeed(250);
}

void destra() {
  m1.run(FORWARD);
  m1.setSpeed(250);
  m2.run(RELEASE);
  m3.run(RELEASE);
  m4.run(FORWARD);
  m4.setSpeed(250);
  delay(700);
}

void sinistra() {
  m2.run(FORWARD);
  m2.setSpeed(250);
  m1.run(RELEASE);
  m4.run(RELEASE);
  m3.run(FORWARD);
  m3.setSpeed(250);
  delay(700);
}

void fermo() {
  m1.run(RELEASE);
  m2.run(RELEASE);
  m3.run(RELEASE);
  m4.run(RELEASE);
}

void servo() {
  while (pos < 170) {
    s1.write(pos);
    pos++;
    if (pos == 170) {
      x = sr04.Distance();
      Serial.print(x);
    }
  }
  delay(500);
  while (pos > 30) {
    s1.write(pos);
    pos--;
    if (pos == 30) {
      y = sr04.Distance();
      Serial.print(y);
    }
  }
  delay(500);
  while (pos < 90) {
    s1.write(pos);
    pos++;
  }
  delay(500);
  while (pos > 90) {
    s1.write(pos);
    pos--;
  }
  if (x < y) {
    destra();
  }
  else if (y < x) {
    sinistra();
  }
  else if (y == x) {
    sinistra();
  }

}

void loop() {
  if (set == 1) {
    if (irrecv.decode(&results)) {
      val = results.value;
      Serial.println(val);
      irrecv.resume();
      switch (val) {
        case S1:
        case UNKNOWN_1: set = 1; break;
        case S2:
        case UNKNOWN_2: set = 2; break;
        default: break;
      }
    }
    avanti();
    a = sr04.Distance();
    if (pos == 90 && a < 25 && a > 9) {
      fermo();
      servo();
    }
    else if (pos == 90 && a < 10) {
      indietro();
      fermo();
      servo();
    }
  }
  else if (set == 2) {
    if (irrecv.decode(&results)) {
      val = results.value;
      Serial.println(val);
      irrecv.resume();
      switch (val) {
        case S1:
        case UNKNOWN_1: set = 1; break;
        case S2:
        case UNKNOWN_2: set = 2; break;
        case A:
        case UNKNOWN_A: avanti(); break;
        case I:
        case UNKNOWN_I: indietro(); break;
        case S:
        case UNKNOWN_S: sinistra(); break;
        case D:
        case UNKNOWN_D: destra(); break;
        case F:
        case UNKNOWN_F: fermo(); break;
        default: break;
      }
    }
  }
  Serial.print(set);
}

