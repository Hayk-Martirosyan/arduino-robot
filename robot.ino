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



class Echo{
  private:
    int trigPin;
    int echoPin;
    const static int n=5;
    int distances[n]={/*0,0,0,0,0,*/0,0,0,0,1000}; 
    int current = 0;
   public:
    Echo(int trigPin, int echoPin){
      this->trigPin = trigPin;
      this->echoPin = echoPin;
    }

    void setup(){
      pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
      pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    }

    int measureDistance(){
      // Clears the trigPin
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      
      // Reads the echoPin, returns the sound wave travel time in microseconds
      int duration = pulseIn(echoPin, HIGH, 50000);
      if(duration<=0){
        return 0;
      }
      // Calculating the distance
      int distance= duration*0.034/2;
      
      //calculate average of last 10 records
      current=(current+1)%n;
      distances[current]=distance;
      
      // Prints the distance on the Serial Monitor
//      Serial.print("Distance: ");
//      Serial.println(distance);
       return distance;
    }

    int averageDistance(){
      int sum = 0;
      for(int i=0; i<n; i++){
        sum+=distances[i];
      }
      return sum/n;
    }

  
};




int pinLB=A3;     // Direction of left back
int pinLF=A2;      // Direction of left forward

int pinRB=A1;    // Direction of right back
int pinRF=A0;    // Direction of right forward

const int MIN_DISTANCE = 60;
Echo echoFront= Echo(8, 9);
Echo echoRight = Echo(4, 6);

void setup() {
  echoFront.setup();
  echoRight.setup();
  Serial.begin(9600); // Starts the serial communication
  digitalWrite(pinRF,HIGH); 
  digitalWrite(pinLF,HIGH);

//   delay(10000);
//  digitalWrite(pinRF,LOW); 
//  digitalWrite(pinLB,LOW);
 
}


int rotationMode = 0;

void loop() {
  
  
  Serial.print(echoFront.measureDistance());
  Serial.print("  ");
  Serial.println(echoRight.measureDistance());
//  delay(100);
//  Serial.println(echo1.averageDistance() + );
//  Serial.println("-");
//  Serial.println(echo2.averageDistance());
//  delay(500);
  int distance = min(echoFront.averageDistance(), echoRight.averageDistance());//echoFront.averageDistance();//
  Serial.println(distance);
  if(rotationMode==0){
    
    if(distance<MIN_DISTANCE){
      rotationMode=1;

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

    if(distance<MIN_DISTANCE*2){
      
    }
    else {
      rotationMode=0;
      //normal mode
      digitalWrite(pinLB,LOW);
      digitalWrite(pinRB,LOW);
      digitalWrite(pinRF,HIGH); 
      digitalWrite(pinLF,HIGH);
    }
  }
  
}
