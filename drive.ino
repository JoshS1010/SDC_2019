const float dualRate = .25;
const int MAX_VALUE = 170;  //max 172
const int MIN_VALUE = 10;   //min 12

int forward = 0;
int rotate = 0;
int strafe = 0;

Servo FL;
Servo FR;
Servo RL;
Servo RR;

const double headingP = 2.0 , headingI = .5, headingD = 0, pidHeading = 0;
double pidCorrection = 0;
double headingErr = 0;
double desiredHeading = 0;
PID headingPID(&headingErr, &pidCorrection, &pidHeading, headingP, headingI, headingD, DIRECT);

unsigned long headingMillis = 0;
const long headingInterval = 5;

bool turning = false;
unsigned long turningMillis = 0;
const unsigned long turningInterval = 250;

sensors_event_t event;

void drive(){

  headingCorrection();

  //drive
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
    rotate = (leftXaxis(0) + pidCorrection) * ((1 - dualRate) * ((float)abs(strafe) / 255.0 - 1) + 1);
  }
  else {
    rotate = (leftXaxis(0) + pidCorrection) * ((1 - dualRate) * ((float)abs(forward) / 255.0 - 1) + 1);
  }

  if (leftXaxis(0) != 0){
    turning = true;
    headingPID.SetMode(MANUAL);
    pidCorrection = 0;
    turningMillis = millis();
  }

  /***************************************************************************************************************/
  unsigned long currentMillis = millis();
  if (currentMillis - turningMillis >= turningInterval && turning){
    turning = false;
    setHeading();
    headingPID.SetMode(AUTOMATIC);
  }
  
  //CCW is positive
  int FLmotor = -forward - strafe - rotate;
  int FRmotor = +forward - strafe - rotate;
  int RLmotor = -forward + strafe - rotate;
  int RRmotor = +forward + strafe - rotate;

//  Serial.print(headingErr);
//  Serial.print("   ");
//  Serial.println(pidCorrection);
  
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
  FL.write(reMap(0));
  FR.write(reMap(0));
  RL.write(reMap(0));
  RR.write(reMap(0));
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

float headingError(){
  float degRight, degLeft;
  float actualHeading = event.orientation.x;
  if (actualHeading > desiredHeading){
    degRight = 360 - actualHeading + desiredHeading;
  }
  else {
    degRight = desiredHeading - actualHeading;
  }
  degLeft = 360 - degRight;

  if (degRight < degLeft){
    return -degRight;
  }
  else {
    return degLeft;
  }
}

float setHeading(){
  desiredHeading = event.orientation.x;
}

void headingCorrection(){
  if(headingStarted){
    unsigned long currentMillis = millis();
    if (currentMillis - headingMillis >= headingInterval){ //update current heading every headingInterval ms
      bno.getEvent(&event);
      headingErr = headingError();
      headingPID.Compute();
    }
  }
}

void driveInitialize(){
  FL.attach(FLpin);
  FR.attach(FRpin);
  RL.attach(RLpin);
  RR.attach(RRpin);

  headingPID.SetMode(AUTOMATIC);
  headingPID.SetOutputLimits(-255, 255);
}
