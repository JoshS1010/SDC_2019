#define armScore      -50   //done
#define armStorage    430   //done   
#define armGatherHigh 300   //done
#define armGatherLow  400   //done
#define armBlock      520   //done

#define doorScore       320
#define doorClosed      0
#define doorHomeValue   200

bool first = true;

const int doorShutThreshold = 5;
const int countHold = 2000;

bool gameStart = false;

void armAndDoor() {
  if (forward != 0){
    gameStart = true;
  }
  if (gameStart == false){
    armHold(armStorage);
    bumper.write(bumperUp);
    Serial.println(armPosition);
  }
  else{
    if (first == true){
      first = false;
      armPosition = armStorage;
      driveInitialize();
    }
    bumper.write(bumperDown);
    if (Xbox.getButtonPress(B, 0)){         //score balls
      moveArm(armScore);
      openDoor();
    }
    else if (forward < doorShutThreshold){  //close door under deceleration
      if(moveArm(armBlock)){
      }
      armCalibrate();
      if (armPosition > doorHomeValue){
        closeDoor();
      }
    }
    else if (Xbox.getButtonPress(A, 0)){    //get big balls
      moveArm(armGatherHigh);
      if (armPosition > doorHomeValue){
        closeDoor();
      }
    }
    else {                                  //get small balls
      moveArm(armGatherLow);
      if (armPosition > doorHomeValue){
        closeDoor();
      }
    }
  }
}

void armHold(int val){
  static int previousVal = -1000;
  static int count = 0;
  if (previousVal != val){
    count = 0;
    previousVal = val;
  }
  if (desiredArmPosition == val && count < countHold){
    count = count + 1;
  }
  if (count >= countHold){
    armDrive(0);
  }
  else{
    moveArm(val);
  }
}
