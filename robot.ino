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


int pinLB=A3;     // Direction of left back
int pinLF=A2;      // Direction of left forward

int pinRB=A1;    // Direction of right back
int pinRF=A0;    // Direction of right forward

const int MIN_DISTANCE = 40;
Echo echoLeft= Echo(8, 9);
Echo echoRight = Echo(4, 6);
Gyro gyro;
void setup() {
  
  echoLeft.setup();
  echoRight.setup();
  gyro.setup();
  Serial.begin(9600); // Starts the serial communication
  
  digitalWrite(pinRF,HIGH); 
  digitalWrite(pinLF,HIGH);

//   delay(10000);
//  digitalWrite(pinRF,LOW); 
//  digitalWrite(pinLB,LOW);
 
}

class Buzzer {
  private:
    int buzzerPin;
  public:
    Buzzer(int pin){
      buzzerPin = pin;
    }

    void setup(){
      pinMode(buzzerPin, OUTPUT);
    }

    void on(int freq){
      tone(buzzerPin, freq);
    }

    void off(){
      noTone(buzzerPin);
    }
  
};

Buzzer buzzer(2);
int rotationMode = 0;
double angelZ = 0;
void loop() {

  gyro.loop();
  Serial.print(echoLeft.measureDistance());
  Serial.print(" - ");
  Serial.println(echoRight.measureDistance());
//  delay(100);
//  Serial.println(echo1.averageDistance() + );
//  Serial.println("-");
//  Serial.println(echo2.averageDistance());
//  delay(500);
  int distanceLeft = echoLeft.averageDistance();//echoFront.averageDistance();//
  int distanceRight = echoRight.averageDistance();//echoFront.averageDistance();//
  Serial.print(distanceLeft);
  Serial.print(" ");
  Serial.println(distanceRight);
  
  if(rotationMode==0){
    
    if(distanceLeft<MIN_DISTANCE){
      rotationMode=1;
      angelZ = gyro.getRotationZ();
      //rotation starting
      digitalWrite(pinRF,LOW); 
      digitalWrite(pinLF,LOW);
//      delay(2000);
//      digitalWrite(pinRF,HIGH); 
      digitalWrite(pinRF,LOW);
      digitalWrite(pinRB,HIGH);

    }
    else if(distanceRight<MIN_DISTANCE){
      rotationMode=1;
      angelZ = gyro.getRotationZ();
      //rotation starting
      digitalWrite(pinRF,LOW); 
      digitalWrite(pinLF,LOW);
//      delay(2000);
//      digitalWrite(pinRF,HIGH); 
      digitalWrite(pinLF,LOW);
      digitalWrite(pinLB,HIGH);
    }
    else{
      digitalWrite(pinRF,LOW); 
      digitalWrite(pinLF,LOW);
      delay(20);
      digitalWrite(pinRF,HIGH); 
      digitalWrite(pinLF,HIGH);
    }
  }
  else {

    if(distanceLeft<MIN_DISTANCE || abs(angelZ - gyro.getRotationZ())<90){
//      buzzer.on(400);
    }
    else if(distanceRight<MIN_DISTANCE || abs(angelZ - gyro.getRotationZ())<90){
//      buzzer.on(800);
    }
    else {
      rotationMode=0;

//      buzzer.off();
      //normal mode
      digitalWrite(pinLB,LOW);
      digitalWrite(pinRB,LOW);
      digitalWrite(pinRF,HIGH); 
      digitalWrite(pinLF,HIGH);
    }
  }
  
  
}
