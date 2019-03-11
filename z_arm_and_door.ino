#define armScore      -50   //done
#define armStorage    400   //done   
#define armGatherHigh 330   //done
#define armGatherLow  400   //done
#define armBlock      520   //done

#define doorScore       320
#define doorClosed      0
#define doorHomeValue   200

const int doorShutThreshold = 5;

bool gameStart = false;

void armAndDoor() {
  moveArm(desiredArmPosition);
  if (forward != 0){
    gameStart = true;
  }
  if (gameStart == false){
    desiredArmPosition = armStorage;
  }
  else{
    //arm and door code here
    if (Xbox.getButtonPress(B, 0)){ //score balls
      moveArm(armScore);
      openDoor();
    }
    else if (forward < doorShutThreshold){ //close door under deceleration
      if(moveArm(armBlock)){
      }
      armCalibrate();
      if (armPosition > doorHomeValue){
        closeDoor();
      }
    }
    else if (Xbox.getButtonPress(A, 0)){ //get big balls
      moveArm(armGatherHigh);
      if (armPosition > doorHomeValue){
        closeDoor();
      }
    }
    else {
      moveArm(armGatherLow);
      if (armPosition > doorHomeValue){
        closeDoor();
      }
    }
  }
}
