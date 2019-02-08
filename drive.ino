const float dualRate = .35;
const int MAX_VALUE = 172;
const int MIN_VALUE = 12;

int forward = 0;
int rotate = 0;
int strafe = 0;

void drive(){
  forward = Xbox.getButtonPress(R2, 0) - Xbox.getButtonPress(L2, 0);
  strafe = rightXaxis(0);

  if (Xbox.getButtonPress(R1, 0)) {
    strafe = 255;
  }
  if (Xbox.getButtonPress(L1, 0)) {
    strafe = -255;
  }

  //rotate dual rate
  if (abs(forward) <= abs(strafe)) {
    rotate = leftXaxis(0) * ((1 - dualRate) * ((float)abs(strafe) / 255.0 - 1) + 1);
  }
  else {
    rotate = leftXaxis(0) * ((1 - dualRate) * ((float)abs(forward) / 255.0 - 1) + 1);
  }

  //CCW is positive
  int FLmotor = -forward - strafe - rotate;
  int FRmotor = +forward - strafe - rotate;
  int RLmotor = -forward + strafe - rotate;
  int RRmotor = +forward + strafe - rotate;

  FLmotor = reMap(FLmotor);
  FRmotor = reMap(FRmotor);
  RLmotor = reMap(RLmotor);
  RRmotor = reMap(RRmotor);

  FL.write(FLmotor);
  FR.write(FRmotor);
  RL.write(RLmotor);
  RR.write(RRmotor);
}

void killDrive() {
  FL.write(92);
  FR.write(92);
  RL.write(92);
  RR.write(92);
}

int reMap(int val){
  if (val >= 255){
    return MAX_VALUE;
  }
  else if (val <= -255){
    return MIN_VALUE;
  }
  else{
    return map(val, -255, 255, MIN_VALUE, MAX_VALUE);
  }
}
