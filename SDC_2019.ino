#include <Servo.h>
#include <XBOXRECV.h>
#include <Wire.h>

#include <PID_v1.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
Adafruit_BNO055 bno = Adafruit_BNO055(55);

USB Usb;
XBOXRECV Xbox(&Usb);

Servo FL;
Servo FR;
Servo RL;
Servo RR;

const double headingP = 2.5 , headingI = .75, headingD = 0, pidHeading = 0;
bool headingStarted = true;
double pidCorrection = 0;
double headingErr = 0;
double desiredHeading = 0;
PID headingPID(&headingErr, &pidCorrection, &pidHeading, headingP, headingI, headingD, DIRECT);

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.println(F("\r\nXbox Wireless Receiver Library Started"));

  FL.attach(40);
  FR.attach(42);
  RL.attach(44);
  RR.attach(46);

  Serial.println("Orientation Sensor Test"); Serial.println("");
  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    headingStarted = false;
    //    while(1);
  }

  delay(1000);

  bno.setExtCrystalUse(true);

  headingPID.SetMode(AUTOMATIC);
  headingPID.SetOutputLimits(-255, 255);
}

void loop() {

  Usb.Task();

  if (Xbox.XboxReceiverConnected) {
    if (Xbox.Xbox360Connected[0]) {
      drive();




    }
    else killDrive(); //kill drive motors if controller disconnects
  }
  else killDrive(); //kill drive motors if receiver disconnects
}
