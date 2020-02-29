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
