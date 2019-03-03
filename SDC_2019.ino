#include <Servo.h>
#include <XBOXRECV.h>
//#include <Wire.h>
#include <PID_v1.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
//#include <utility/imumaths.h>
Adafruit_BNO055 bno = Adafruit_BNO055(55);

USB Usb;
XBOXRECV Xbox(&Usb);

bool headingStarted = true;

const int FLpin = 40, FRpin = 42, RLpin = 44, RRpin = 46; 

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

  Serial.println("Orientation Sensor Test"); Serial.println("");
  /* Initialise the sensor */
  if (!bno.begin())
  {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    headingStarted = false;
  }

  delay(1000);

  bno.setExtCrystalUse(true);

  driveInitialize();
  armInitialize();
}

void loop() {

  Usb.Task();

  if (Xbox.XboxReceiverConnected) {
    if (Xbox.Xbox360Connected[0]) {
      drive();
      armAndDoor();

    }
    else killDrive(); //kill drive motors if controller disconnects
  }
  else killDrive(); //kill drive motors if receiver disconnects
}
