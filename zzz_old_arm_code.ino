//bool moveArm(int pos) {
//  int s;
//  if (armPosition < pos - tolerance) {
//    if (pos - armPosition > 100) {
//      s = 255;
//    }
//    else if (pos - armPosition > 70) {
//      s = 150;
//    }
//    else {
//      s = 100;
//    }
//    armMotor.drive(s);
////    armMotor.drive(255);
//    return false;
//  }
//  else if (armPosition > pos + tolerance) {
//    if (armPosition - pos > 100) {
//      s = -255;
//    }
//    else if (armPosition - pos > 70) {
//      s = -150;
//    }
//    else {
//      s = -100;
//    }
//    armMotor.drive(s);
////    armMotor.drive(-255);
//    return false;
//  }
//  else {
//    armMotor.drive(0);
//    return true;
//  }
//}
