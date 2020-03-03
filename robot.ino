
// defines pins numbers
const int echoLeftTrigPin = 8;
const int echoLeftEchoPin = 9;

const int echoRightTrigPin = 4;
const int echoRightEchoPin = 6;

const int buzzerPin = 2;
// defines variables
#include "Gyro.h"
#include "Echo.h"
#include "Async.h"
#include "Buzzer.h"

int pinLB=A3;     // Direction of left back
int pinLF=A2;      // Direction of left forward

int pinRB=A1;    // Direction of right back
int pinRF=A0;    // Direction of right forward

const int MIN_DISTANCE = 40;
Echo echoLeft= Echo(echoLeftTrigPin, echoLeftEchoPin);
AsyncAction& echoLeftAsync = AsyncBuilder::start()
            .action([] () -> void {  echoLeft.step1();}, 2)
            .action([] () -> void {  echoLeft.step2();}, 10)
            .action([] () -> void {  echoLeft.measureDistance();}, 10)
            .loop().build();

Echo echoRight = Echo(echoRightTrigPin, echoRightEchoPin);
AsyncAction& echoRightAsync = AsyncBuilder::start()
            .action([] () -> void {  echoRight.step1();}, 2)
            .action([] () -> void {  echoRight.step2();}, 10)
            .action([] () -> void {  echoRight.measureDistance();}, 10)
            .loop().build();



Gyro gyro;
Buzzer buzzer(buzzerPin);
AsyncAction& buzzerAsync = AsyncBuilder::start().action([] () -> void {  buzzer.on(800);}, 400).action([] () -> void {  buzzer.on(600);}, 400).loop().onEnd([](){buzzer.off();}).build();
void stopDriving(){
  digitalWrite(pinRF,LOW); 
  digitalWrite(pinLF,LOW);
  digitalWrite(pinRB,LOW); 
  digitalWrite(pinLB,LOW);
}

AsyncAction& driveLeft = AsyncBuilder::start()
                      .onStart([] () -> void {  stopDriving();})
                      .action([] () -> void {  digitalWrite(pinLB,HIGH);}, 100)
                      .action([] () -> void {  digitalWrite(pinLB,LOW);}, 20)
                      .onEnd([](){stopDriving();}).loop().build();
AsyncAction& driveRight = AsyncBuilder::start()
                      .onStart([] () -> void {  stopDriving();})
                      .action([] () -> void {  digitalWrite(pinRB,HIGH);}, 100)
                      .action([] () -> void {  digitalWrite(pinRB,LOW);}, 20)
                      .onEnd([](){stopDriving();}).loop().build();

AsyncAction& driveFront = AsyncBuilder::start()
                      .onStart([] () -> void {  stopDriving();})
                      .action([] () -> void {  digitalWrite(pinLF,HIGH);digitalWrite(pinRF,HIGH);}, 100)
                      .action([] () -> void {  digitalWrite(pinLF,LOW);digitalWrite(pinRF,LOW);}, 10)
                      .onEnd([](){stopDriving();}).loop().build();
//      delay(2000);
//      digitalWrite(pinRF,HIGH); 
      
void setup() {
//  buzzerAsync.on();
  echoLeft.setup();
  echoRight.setup();
  echoLeftAsync.on();
  echoRightAsync.on();
  gyro.setup();
  Serial.begin(9600); // Starts the serial communication
  
//  digitalWrite(pinRF,HIGH); 
//  digitalWrite(pinLF,HIGH);
  driveFront.on();
//   delay(10000);
//  digitalWrite(pinRF,LOW); 
//  digitalWrite(pinLB,LOW);
 
}




int rotationMode = 0;
double angelZ = 0;
void driveAhead(){
  driveLeft.off();
  driveRight.off();
  driveFront.on();
  buzzerAsync.off();
}

void turnLeft(){
  driveFront.off();
  driveLeft.on();
  driveRight.off();
  buzzerAsync.on();
}

void turnRight(){
  driveFront.off();
  driveRight.on();
  driveLeft.off();
  buzzerAsync.on();
}

void stop(){
  driveFront.off();
  driveLeft.off();
  driveRight.off();
  buzzerAsync.off();
}



AsyncAction& test = AsyncBuilder::start().action([] () -> void {  Serial.println(1);}, 1000).action([] () -> void {  Serial.println(2);}, 1000).loop().build();
//void loop() {
//  test.on();
//  AsyncBuilder::run();
////  Serial.println("x");
//  
//}


enum RobotMode{
  ExecutingCommand, DrivingAhead, LeftTurnFromObstacle, RightTurnFromObstacle, LeftTurnReturnToDirection, RightTurnReturnToDirection
};
//enum Command{
//  None, Stop, DriveAhead, LeftTurn, RightTurn
//};
RobotMode mode = DrivingAhead;
//Command command = None;


AsyncAction& rightTurnReturn = AsyncBuilder::start()
      .action([](){driveAhead();}, 2000)
      .action([] () -> void {  turnRight(); mode=RightTurnReturnToDirection;}, 1)
      .build();

AsyncAction& leftTurnReturn = AsyncBuilder::start()
      .action([](){driveAhead();}, 2000)
      .action([] () -> void {  turnLeft(); mode=LeftTurnReturnToDirection;}, 1)
      .build();

long lastMillis = millis();
bool debugStart = true;
void loop() {
  long millis2 = millis();
//  Serial.print(millis2-lastMillis);
//  Serial.print('-');
  lastMillis = millis2;
//  return;
  gyro.loop();
  AsyncBuilder::run();
  
//  test.on();
//  Serial.println(echo1.averageDistance() + );
//  Serial.println("-");
//  Serial.println(echo2.averageDistance());
//  delay(500);
  int distanceLeft = echoLeft.averageDistance();//echoFront.averageDistance();//
  int distanceRight = echoRight.averageDistance();//echoFront.averageDistance();//
//  Serial.print(distanceLeft);
//  Serial.print(' ');
//  Serial.println(distanceRight);
//  switch(command){
//    case None:break;
//    
//    case DriveAhead:
//        driveAhead();
//        buzzerAsync.off();
//        break;
//    case LeftTurn:
//        driveFront.off();
//        driveRight.on();
//        buzzerAsync.on();
//        break;
//    case RightTurn:
//        driveFront.off();
//        driveLeft.on();
//        buzzerAsync.on();
//        break;
//    case Stop:
//        driveLeft.off();
//        driveRight.off();
//        break;
//    
//  }
//  command = None;
  
  switch(mode){
    case ExecutingCommand:break;
    
    case DrivingAhead: 
        if(distanceLeft<MIN_DISTANCE){
          turnRight();
          mode = RightTurnFromObstacle;
          angelZ = gyro.getRotationZ();
        }
        else if(distanceRight<MIN_DISTANCE){
          turnLeft();
          mode = LeftTurnFromObstacle;
          angelZ = gyro.getRotationZ();
        }
        break;
    case LeftTurnFromObstacle:
        if(distanceLeft>MIN_DISTANCE && abs(angelZ-gyro.getRotationZ())>45){
//          command = drive ahead 2s, turn right until angelZ
          rightTurnReturn.on();
          mode = ExecutingCommand;
        }
        break;
    case RightTurnFromObstacle:
        if(distanceRight>MIN_DISTANCE && abs(angelZ-gyro.getRotationZ())>45){
//          command = drive ahead 2s, turn left until angelZ
          leftTurnReturn.on();
          mode = ExecutingCommand;
        }
        break;
    case RightTurnReturnToDirection:
        if(abs(angelZ-gyro.getRotationZ())<5){
          rightTurnReturn.off();
          driveAhead();
          mode = DrivingAhead;
        }
        break;
    case LeftTurnReturnToDirection:
        if(abs(angelZ-gyro.getRotationZ())<5){
          
          leftTurnReturn.off();
          driveAhead();
          mode = DrivingAhead;
        }
        break;
        
  }
  Serial.print(mode);
  Serial.print(' ' );
  Serial.println(angelZ-gyro.getRotationZ());
//  if(rotationMode==0){
//    
//    if(distanceLeft<MIN_DISTANCE){
//      rotationMode=1;
//      angelZ = gyro.getRotationZ();
//      //rotation starting
//      driveFront.off();
//      driveRight.on();
//
//    }
//    else if(distanceRight<MIN_DISTANCE){
//      rotationMode=1;
//      angelZ = gyro.getRotationZ();
//      //rotation starting
//      driveFront.off();
//      driveLeft.on();
//    }
//    else{
//    }
//  }
//  else {
//
//    if(distanceLeft<MIN_DISTANCE || abs(angelZ-gyro.getRotationZ())<45){
//        buzzerAsync.on();
//
//    }
//    else if(distanceRight<MIN_DISTANCE || abs(angelZ-gyro.getRotationZ())<45){
//        buzzerAsync.on();
//    }
//    else {
////      if(rotationMode==1){
////        digitalWrite(pinLB,LOW);
////        digitalWrite(pinRB,LOW);
////        digitalWrite(pinRF,HIGH); 
////        digitalWrite(pinLF,HIGH);
//////        delay(3000);
////        rotationMode =2;
////      }
//      rotationMode=0;
//        
//        buzzerAsync.off();
//      //normal mode
//      driveLeft.off();
//      driveRight.off();
//      driveFront.on();
//    }
//  }
  
  
}
