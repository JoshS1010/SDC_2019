#define armScore      0
#define armStorage    300
#define armGatherHigh 350
#define armGatherLow  400
#define armBlock      500

#define doorScore     300
#define doorClosed     0

const int doorShutThreshold = 100;

bool gameStart = false;

void armAndDoor() {
  moveArm(desiredArmPosition);
  if (forward != 0){
    gameStart = true;
  }
  if (gameStart == false){
    desiredArmPosition = armStorage;
    desiredDoorPosition = doorClosed;
  }
  else{
    Serial.println(desiredDoorPosition);
    //arm and door code here
    if (Xbox.getButtonPress(B, 0)){ //score balls
      desiredArmPosition = armScore;
      moveDoor(doorScore);
      doorHomed = false;
    }
    else if (forward < doorShutThreshold){ //close door under deceleration
      desiredArmPosition = armBlock;
      homeDoor();
    }
    else if (Xbox.getButtonPress(A, 0)){ //get big balls
      desiredArmPosition = armGatherHigh;
      homeDoor();
    }
    else {
      desiredArmPosition = armGatherLow;
      homeDoor();
    }
  }
}
