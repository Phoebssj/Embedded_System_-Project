// irremote.h
// Des: irremote setup
// Audit 11/28/23
// ------------------------------------------------------------

//irremote header
//#include "irremote_config.h"
#include <IRremote.h>
#define IR_USE_AVR_TIMER2

//#include <IRremote.h>

// define glboal constants and types (structs/enums)
const byte IR_RECEIVE_PIN = 4; // Would need to be change to 12 for the signal.

#ifndef IRREMOTE_CONTROLS
#define IRREMOTE_CONTROLS

class Irremote_commands{
  public:
    enum ircommand{
        power = 0,
        vol_up = 1,
        F_Stop = 2,
        goBck = 4,
        ppause = 5,
        goFwd = 6,
        down = 8,
        vol_- = 9,
        up = 10,
        bnt_0 = 12,
        eq = 13,
        rept = 14,
        bnt_1 = 16,
        bnt_2 =17,
        bnt_3 = 18,
        bnt_4 = 20,
        bnt_5 = 21,
        bnt_6 = 22,
        bnt_7 = 24,
        bnt_8 = 25,
        bnt_9 = 26
    };
  //====================================================
  //Constructors:
  //=====================================================
  IRremote_commands();
  //=======================================================
  //Methods
  //=======================================================
  void irconfig(){};
  ircommand readIrcommand(){return rslt;}
  void printcontrol(unsigned int r);




}
