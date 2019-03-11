const int doorBrake = 255;
const int doorSpeed = 255;
const int doorClosedLimitPin = 26;
const int doorOpenedLimitPin = 6;

Motor doorMotor(30, 28, 4);

void doorInitialize(){
  pinMode(doorClosedLimitPin, INPUT_PULLUP);
  pinMode(doorOpenedLimitPin, INPUT_PULLUP);

  doorMotor.setCoastBrake(doorBrake);
  doorMotor.flipPolarity();
  while (!closeDoor()) {}
}

bool closeDoor(){
  if (digitalRead(doorClosedLimitPin) == LOW){
    doorMotor.drive(0);
    return true;
  }
  else{
    doorMotor.drive(-doorSpeed);
    return false;
  }
}

bool openDoor(){
  if (digitalRead(doorOpenedLimitPin) == LOW){
    doorMotor.drive(0);
    return true;
  }
  else{
    doorMotor.drive(doorSpeed);
    return false;
  }
}
