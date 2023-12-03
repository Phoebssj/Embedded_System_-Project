//DC Motors headers 

// define class Motor
//    properties: pin1, pin2, enable, speed
//    methods: constructors, getter/setter for speed, run

//configs for DC Motor


//Constants that will be define to be implemented 

//Constants for Left Motor
const int H1A = 4;
const int H2A = 3;
const int H12EN = 10;

//Constants for Right Motor
const int H3A = 5;
const int H4A = 6;
const int H34EN = 11;
#ifndef MOTOR_FUNCTIONS
#define MOTOR_FUNCTIONS

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
 //Getter Setters 
int getSpeed(){return spd_;}
void setSpeed(int speed){spd_ = speed;}

//Move Function
void move(Motor::MotorDirection dir);
  
  
 }

