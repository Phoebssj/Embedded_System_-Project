// irremote.h
// Des: irremote setup
// Audit 11/28/23
// ------------------------------------------------------------

#ifndef IRREMOTE_CONTROLS
#define IRREMOTE_CONTROLS

//irremote header
#define IR_USE_AVR_TIMER2
#include <IRremote.hpp>

// define glboal constants and types (structs/enums)
const byte IR_RECEIVE_PIN = 12;

class IRremote_commands{
  public:
    enum ircommand{
        ir_invalid = 0,
        power = 0,
        vol_up = 1,
        F_Stop = 2,
        goBck = 4,
        ppause = 5,
        goFwd = 6,
        down = 8,
        vol_down = 9,
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
  IRremote_commands() {}
  //=======================================================
  //Methods
  //=======================================================
  void irconfigs();
  ircommand readIrcommand();
  void printcontrol(unsigned int r);
};

#endif
