int armPosition = 0;
const int armPIDactive = 90;
const int armTolerance = 20;
const int armBrake = 255;
const int armHomeSpeed = -100;
const int armLimitPin = 24;
const int armClosedPin = 7;
const long armClosedInterval = 50;

bool armHomed = false;

int test = 0;

const double armP = 1.3, armI = .2, armD = .2; //1.3  .2  .2  //2.05  1  0
double desiredArmPosition = 0;
double armError = 0;
double armSpeed = 0;
double arm0 = 0;
PID armPID(&arm0, &armSpeed, &armError, armP, armI, armP, DIRECT);

Motor armMotor(32, 34, 5);

bool moveArm(int pos) {         //move arm to desired position and return true when it is there
  armMotor.drive(armSpeed);
  desiredArmPosition = pos;
  armError = desiredArmPosition - armPosition;
  if (armPosition < pos - armTolerance) {
    if (pos - armPosition > armPIDactive) {
      armSpeed = 255;
    }
    else {
      armPID.Compute();
    }
    return false;
  }
  else if (armPosition > pos + armTolerance) {
    if (armPosition - pos > armPIDactive) {
      armSpeed = -255;
    }
    else {
      armPID.Compute();
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
  static bool first = true;
  if (!armHomed){
    if (digitalRead(armLimitPin) == LOW) {
      hit = true;
    }
    if (digitalRead(armLimitPin) == HIGH && !hit) {
      armMotor.drive(armHomeSpeed);
      return true;
    }
    else if (digitalRead(armLimitPin) == LOW && hit) {
      if (first){
        first = false;
        delay(200);
        armMotor.drive(0);
        delay(500);
      }
      armMotor.drive(-armHomeSpeed);
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
  pinMode(armClosedPin, INPUT_PULLUP);

  armPID.SetMode(AUTOMATIC);
  armPID.SetOutputLimits(-255, 255);

  armMotor.flipPolarity();
  armMotor.setCoastBrake(armBrake);

  while (homeArm()) {
      bumper.write(bumperUp);
    }
  while (moveArm(-40)) {
      bumper.write(bumperUp);
    }
  armDrive(0);
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

void armCalibrate(){        //recalibrate the arm every time it blocks balls
  static bool hit = false;
  static bool set = false;
  static long previousMillis = 0;
  long currentMillis = millis();
  Serial.print("test value: ");
  Serial.println(test);
  if (digitalRead(armClosedPin) == LOW){
    if (!hit){
      test = armPosition;
      previousMillis = currentMillis;
      hit = true;
    }
    if (!set){
      if (currentMillis - previousMillis >= armClosedInterval){
        armPosition = 520;
        set = true;
      }
    }
  }
  else{
    hit = false;
    set = false;
  }
}
