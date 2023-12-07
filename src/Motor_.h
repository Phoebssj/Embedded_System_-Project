//DC Motors headers

// define class Motor
//    properties: pin1, pin2, enable, speed
//    methods: constructors, getter/setter for speed, run

#ifndef MOTOR_FUNCTIONS
#define MOTOR_FUNCTIONS

#include <Arduino.h>

class Motor{
private:
  byte pin_1;
  byte pin_2;
  byte enable_;
  int spd_; //I'm just gonna leave it at spd for now


public:
  enum MotorDirection{
    Stop,
    Forward,
    Reverse


  };
Motor();
Motor(byte pin1, byte pin2, byte enable);
 //Getter Setters
int getSpeed(){return spd_;}
void setSpeed(int speed){spd_ = speed;}

//Move Function
void move(Motor::MotorDirection dir);


 };

#endif
