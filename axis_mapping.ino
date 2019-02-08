#define AXIS_MAX_VALUE 255
#define DEADZONE 10000

int leftXaxis(int controller){
  if (Xbox.getAnalogHat(LeftHatX, controller) < -DEADZONE){
    return map(Xbox.getAnalogHat(LeftHatX, controller), -DEADZONE, -32768, 0, -AXIS_MAX_VALUE);
  }
  else if (Xbox.getAnalogHat(LeftHatX, controller) > DEADZONE){
    return map(Xbox.getAnalogHat(LeftHatX, controller), DEADZONE, 32767, 0, AXIS_MAX_VALUE);
  }
  else{
    return 0;
  }
}

int leftYaxis(int controller){
  if (Xbox.getAnalogHat(LeftHatY, controller) < -DEADZONE){
    return map(Xbox.getAnalogHat(LeftHatY, controller), -DEADZONE, -32768, 0, -AXIS_MAX_VALUE);
  }
  else if (Xbox.getAnalogHat(LeftHatY, controller) > DEADZONE){
    return map(Xbox.getAnalogHat(LeftHatY, controller), DEADZONE, 32767, 0, AXIS_MAX_VALUE);
  }
  else{
    return 0;
  }
}

int rightXaxis(int controller){
  if (Xbox.getAnalogHat(RightHatX, controller) < -DEADZONE){
    return map(Xbox.getAnalogHat(RightHatX, controller), -DEADZONE, -32768, 0, -AXIS_MAX_VALUE);
  }
  else if (Xbox.getAnalogHat(RightHatX, controller) > DEADZONE){
    return map(Xbox.getAnalogHat(RightHatX, controller), DEADZONE, 32767, 0, AXIS_MAX_VALUE);
  }
  else{
    return 0;
  }
}

int rightYaxis(int controller){
  if (Xbox.getAnalogHat(RightHatY, controller) < -DEADZONE){
    return map(Xbox.getAnalogHat(RightHatY, controller), -DEADZONE, -32768, 0, -AXIS_MAX_VALUE);
  }
  else if (Xbox.getAnalogHat(RightHatY, controller) > DEADZONE){
    return map(Xbox.getAnalogHat(RightHatY, controller), DEADZONE, 32767, 0, AXIS_MAX_VALUE);
  }
  else{
    return 0;
  }
}
