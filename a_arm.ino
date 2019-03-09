int armPosition = 0;
const int armPIDactive = 50;
const int armTolerance = 10;
const int armBrake = 255;
const int armHomeSpeed = -70;
const int armLimitPin = 24;

bool armHomed = false;

const double armP = 1.3 , armI = 1, armD = 1;
double desiredArmPosition = 0;
double armError = 0;
double armSpeed = 0;
double arm0 = 0;
PID armPID(&arm0, &armSpeed, &armError, armP, armI, armP, DIRECT);

Motor armMotor(32, 34, 5);

bool moveArm(int pos) {
  Serial.print(armPosition);
  Serial.print("    ");
  Serial.print(desiredArmPosition);
  Serial.print("    ");
  Serial.println(armSpeed);
  armMotor.drive(armSpeed);
  desiredArmPosition = pos;
  armError = desiredArmPosition - armPosition;
//  Serial.print(armSpeed);
  if (armPosition < pos - armTolerance) {
    if (pos - armPosition > armPIDactive) {
      armSpeed = 255;
//      Serial.println("   full speed");
    }
    else {
      armPID.Compute();
//      Serial.println("   PID Mode");
    }
    return false;
  }
  else if (armPosition > pos + armTolerance) {
    if (armPosition - pos > armPIDactive) {
      armSpeed = -255;
      Serial.println("   full speed");
    }
    else {
      armPID.Compute();
      Serial.println("   PID Mode");
    }
    return false;
  }
  else {
    return true;
  }
}

bool homeArm() {
  Serial.println("homing arm");
  static bool hit = false;
  if (!armHomed){
    if (digitalRead(armLimitPin) == LOW) {
      hit = true;
    }
    if (digitalRead(armLimitPin) == HIGH && !hit) {
      armMotor.drive(armHomeSpeed);
      return true;
    }
    else if (digitalRead(armLimitPin) == LOW && hit) {
      armMotor.drive(-armHomeSpeed * 2/3);
      return true;
    }
    else {
      armMotor.drive(0);
      armPosition = 0;
      hit = false;
      armHomed = true;
      return false;
    }
  }
}

void armInitialize(){
  pinMode(2, INPUT_PULLUP);           // arm encoder
  pinMode(3, INPUT_PULLUP);           // arm encoder

  attachInterrupt(0, armEncoder0, RISING);    //arm encoder
  attachInterrupt(1, armEncoder1, RISING);    //arm encoder

  pinMode(armLimitPin, INPUT_PULLUP);    // arm limit switch

  armPID.SetMode(AUTOMATIC);
  armPID.SetOutputLimits(-255, 255);

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

void armDrive(int s){
  Serial.println(armPosition);
  armMotor.drive(s);
}
