int armPosition = 0;
const int armPIDactive = 100;
const int armTolerance = 10;
const int armBrake = 255;
const int armHomeSpeed = -50;
const int armLimitPin = 24;

const double armP = 1 , armI = 0, armD = 1;
double desiredArmPosition = 0;
double armError = 0;
double armSpeed = 0;
double arm0 = 0;
PID armPID(&arm0, &armSpeed, &armError, armP, armI, armP, DIRECT);

Motor armMotor(32, 34, 5);

bool moveArm(int pos) {
  if (armPosition < pos - armTolerance) {
    if (pos - armPosition > armPIDactive) {
      armSpeed = 255;
      armPID.SetMode(MANUAL);
      return false;
    }
    else {
      armPID.SetMode(AUTOMATIC);
      armPID.Compute();
      return false;
    }
  }
  else if (armPosition > pos + armTolerance) {
    if (armPosition - pos > armPIDactive) {
      armSpeed = -255;
      armPID.SetMode(MANUAL);
      return false;
    }
    else {
      armPID.SetMode(AUTOMATIC);
      armPID.Compute();
      return false;
    }
  }
  else {
    armPID.SetMode(AUTOMATIC);
    armPID.Compute();
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

  armPID.SetMode(MANUAL);
  armPID.SetOutputLimits(-255, 255);

  armMotor.flipPolarity();
  armMotor.setCoastBrake(armBrake);

//  while (homeArm()) {}
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
