/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
*
* by Dejan Nedelkovski,
* www.HowToMechatronics.com
*
*/

// defines pins numbers
//const int trigPin = 8;
//const int echoPin = 9;

//const int trigPin = 2;
//const int echoPin = 4;

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
Echo echoLeft= Echo(8, 9);
AsyncAction& echoLeftAsync = AsyncBuilder::start()
            .action([] () -> void {  echoLeft.step1();}, 2)
            .action([] () -> void {  echoLeft.step2();}, 10)
            .action([] () -> void {  echoLeft.measureDistance();}, 10)
            .loop().build();

Echo echoRight = Echo(4, 6);
AsyncAction& echoRightAsync = AsyncBuilder::start()
            .action([] () -> void {  echoRight.step1();}, 2)
            .action([] () -> void {  echoRight.step2();}, 10)
            .action([] () -> void {  echoRight.measureDistance();}, 10)
            .loop().build();



Gyro gyro;
Buzzer buzzer(2);
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



AsyncAction& test = AsyncBuilder::start().action([] () -> void {  Serial.println(1);}, 1000).action([] () -> void {  Serial.println(2);}, 1000).loop().build();
//void loop() {
//  test.on();
//  AsyncBuilder::run();
////  Serial.println("x");
//  
//}
long lastMillis = millis();
void loop() {
  long millis2 = millis();
  Serial.print(millis2-lastMillis);
  Serial.print('-');
  lastMillis = millis2;
//  return;
  gyro.loop();
  AsyncBuilder::run();
//  test.on();
//  echoLeft.measureDistance();
//  echoRight.measureDistance();
//  Serial.print(echoLeft.measureDistance());
////  Serial.print(" - ");
//  Serial.println(echoRight.measureDistance());
//  delay(100);
//  Serial.println(echo1.averageDistance() + );
//  Serial.println("-");
//  Serial.println(echo2.averageDistance());
//  delay(500);
  int distanceLeft = echoLeft.averageDistance();//echoFront.averageDistance();//
  int distanceRight = echoRight.averageDistance();//echoFront.averageDistance();//
//  Serial.print(distanceLeft);
//  Serial.print(' ');
//  Serial.println(distanceRight);
  
  if(rotationMode==0){
    
    if(distanceLeft<MIN_DISTANCE){
      rotationMode=1;
      angelZ = gyro.getRotationZ();
      //rotation starting
      driveFront.off();
      driveRight.on();
//      digitalWrite(pinRF,LOW); 
//      digitalWrite(pinLF,LOW);
////      delay(2000);
////      digitalWrite(pinRF,HIGH); 
//      digitalWrite(pinRF,LOW);
//      digitalWrite(pinRB,HIGH);

    }
    else if(distanceRight<MIN_DISTANCE){
      rotationMode=1;
      angelZ = gyro.getRotationZ();
      //rotation starting
      driveFront.off();
      driveLeft.on();
//      digitalWrite(pinRF,LOW); 
//      digitalWrite(pinLF,LOW);
////      delay(2000);
////      digitalWrite(pinRF,HIGH); 
//      digitalWrite(pinLF,LOW);
//      digitalWrite(pinLB,HIGH);
    }
    else{
//      driveLeft.off();
//      driveRight.off();
//      driveFront.on();
//      digitalWrite(pinRF,LOW); 
//      digitalWrite(pinLF,LOW);
//      delay(20);
//      digitalWrite(pinRF,HIGH); 
//      digitalWrite(pinLF,HIGH);
    }
  }
  else {

    if(distanceLeft<MIN_DISTANCE || abs(angelZ-gyro.getRotationZ())<45){
//      buzzer.on(400);
        buzzerAsync.on();

    }
    else if(distanceRight<MIN_DISTANCE || abs(angelZ-gyro.getRotationZ())<45){
//      buzzer.on(800);
        buzzerAsync.on();
    }
    else {
//      if(rotationMode==1){
//        digitalWrite(pinLB,LOW);
//        digitalWrite(pinRB,LOW);
//        digitalWrite(pinRF,HIGH); 
//        digitalWrite(pinLF,HIGH);
////        delay(3000);
//        rotationMode =2;
//      }
      rotationMode=0;
        
//      buzzer.off();
        buzzerAsync.off();
      //normal mode
      driveLeft.off();
      driveRight.off();
      driveFront.on();
//      digitalWrite(pinLB,LOW);
//      digitalWrite(pinRB,LOW);
//      digitalWrite(pinRF,HIGH); 
//      digitalWrite(pinLF,HIGH);
    }
  }
  
  
}
