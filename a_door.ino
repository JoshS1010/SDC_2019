int doorPosition = 0;
const int doorPIDactive = 100;
const int doorTolerance = 10;
const int doorBrake = 255;
const int doorHomeSpeed = -255;
const int doorLimitPin = 26;

bool doorHomed = false;

const double doorP = .2 , doorI = 0, doorD = 0;
double desiredDoorPosition = 0;
double doorError = 0;
double doorSpeed = 0;
double door0 = 0;
PID doorPID(&door0, &doorSpeed, &doorError, doorP, doorI, doorP, DIRECT);

Motor doorMotor(30, 28, 4);

bool moveDoor(int pos) {
  int s;
  if (doorPosition < pos - doorTolerance) {
    doorMotor.drive(255);
    return false;
  }
  else if (doorPosition > pos + doorTolerance) {
    doorMotor.drive(-255);
    return false;
  }
  else {
    doorMotor.drive(0);
    return true;
  }
}

bool homeDoor() {
  Serial.println("homing door");
  static bool hit = false;
  if (!doorHomed){
    if (digitalRead(doorLimitPin) == LOW) {
      hit = true;
    }
    if (digitalRead(doorLimitPin) == HIGH && !hit) {
      doorMotor.drive(doorHomeSpeed);
      return true;
    }
    else if (digitalRead(doorLimitPin) == LOW && hit) {
      doorMotor.drive(-doorHomeSpeed / 2);
      return true;
    }
    else {
      doorMotor.drive(0);
      doorPosition = 0;
      hit = false;
      doorHomed = true;
      return false;
    }
  }
}

void doorInitialize(){
  pinMode(18, INPUT_PULLUP);           // door encoder
  pinMode(19, INPUT_PULLUP);           // door encoder

  attachInterrupt(4, doorEncoder2, RISING);    //door encoder
  attachInterrupt(5, doorEncoder3, RISING);    //door encoder

  pinMode(doorLimitPin, INPUT_PULLUP);    // door limit switch

  doorPID.SetMode(MANUAL);
  doorPID.SetOutputLimits(-255, 255);

  doorMotor.setCoastBrake(doorBrake);
  doorMotor.flipPolarity();
  while (homeDoor()) {}
}

void doorEncoder2() {
  if (digitalRead(18) == LOW) {
    doorPosition++;
  } else {
    doorPosition--;
  }
}

void doorEncoder3() {
  if (digitalRead(19) == LOW) {
    doorPosition--;
  } else {
    doorPosition++;
  }
}

void doorDrive(int s){
  Serial.println(doorPosition);
  doorMotor.drive(s);
}
