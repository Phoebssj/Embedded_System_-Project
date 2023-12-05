#include "Motor_.h"

 Motor:: Motor(){
    pin_1 = 0;
    pin_2 = 0;
    enable_ = 0;
    spd_  = 0;

  }

 Motor::Motor(byte pin1, byte pin2, byte enable){
    pin_1 = pin1;
    pin_2 = pin2;
    enable_ = enable;
    spd_  = 0;

    pinMode(pin_1, OUTPUT);
    pinMode(pin_2, OUTPUT);
    pinMode(enable_, OUTPUT);


 }

   // Time to move


  void Motor :: move(Motor::MotorDirection dir){

    switch(dir){
      case Stop:
        digitalWrite(pin_1,LOW);
        digitalWrite(pin_2,LOW);
        digitalWrite(enable_,spd_);
      break;

      case Forward:
      digitalWrite(pin_1,HIGH);
      digitalWrite(pin_2,LOW);
      analogWrite(enable_,spd_);

      break;
      case Reverse:
      digitalWrite(pin_1,LOW);
      digitalWrite(pin_2,HIGH);
      analogWrite(enable_,spd_);






    }//end of switch

// we could make turn functions here to do the turing instead?
// Maybe turing left or right.


  }//end of move
