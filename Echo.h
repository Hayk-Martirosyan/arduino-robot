

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
