//#include <PWM_Motor.h>
#include <Servo.h>
#include <XBOXRECV.h>
#include <Wire.h>

USB Usb;
XBOXRECV Xbox(&Usb);

Servo FL;
Servo FR;
Servo RL;
Servo RR;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));

  FL.attach(2);
  FR.attach(3);
  RL.attach(4);
  RR.attach(5);
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
