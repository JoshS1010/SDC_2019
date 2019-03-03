#include <Motor.h>

int armPosition = 0;
const int tolerance = 10;
const int armBrake = 255;
const int armHomeSpeed = -50;
const int armLimitPin = 24;

Motor armMotor(32, 34, 5);

void armAndDoor() {
  Serial.println (armPosition);
  if (moveArm(400)) {
    //    Serial.println("done");
  }
}

bool moveArm(int pos) {
  int s;
  if (armPosition < pos - tolerance) {
    if (pos - armPosition > 100) {
      s = 255;
    }
    else if (pos - armPosition > 70) {
      s = 150;
    }
    else {
      s = 100;
    }
    armMotor.drive(s);
//    armMotor.drive(255);
    return false;
  }
  else if (armPosition > pos + tolerance) {
    if (armPosition - pos > 100) {
      s = -255;
    }
    else if (armPosition - pos > 70) {
      s = -150;
    }
    else {
      s = -100;
    }
    armMotor.drive(s);
//    armMotor.drive(-255);
    return false;
  }
  else {
    armMotor.drive(0);
    return true;
  }
}

bool homeArm() {
  static bool hit = false;
  static bool lift = false;
  if (digitalRead(armLimitPin) == LOW) {
    hit = true;
  }
  if (digitalRead(armLimitPin) == LOW && lift) {
    armMotor.drive(0);
    armPosition = 0;
    hit = false;
    lift = false;
    return false;
  }
  if (digitalRead(armLimitPin) == HIGH && !hit) {
    armMotor.drive(armHomeSpeed);
    return true;
  }
  else if (digitalRead(armLimitPin) == LOW && !lift) {
    armMotor.drive(-armHomeSpeed / 2);
    return true;
  }
  else {
    lift = true;
    armMotor.drive(armHomeSpeed / 2);
    return true;
  }
}

void armInitialize(){
  pinMode(2, INPUT_PULLUP);           // arm encoder
  pinMode(3, INPUT_PULLUP);           // arm encoder

  attachInterrupt(0, armEncoder0, RISING);    //arm encoder
  attachInterrupt(1, armEncoder1, RISING);    //arm encoder

  pinMode(armLimitPin, INPUT_PULLUP);    // arm limit switch

  armMotor.flipPolarity();
  armMotor.setCoastBrake(armBrake);

  while (homeArm()) {}
}

void armEncoder0() {
  if (digitalRead(3) == LOW) {
    armPosition++;
  } else {
    armPosition--;
  }
}

void armEncoder1() {
  if (digitalRead(2) == LOW) {
    armPosition--;
  } else {
    armPosition++;
  }
}
