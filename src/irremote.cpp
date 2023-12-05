// irremote.cpp
// Desc: Implementation of the irremote
// Audit: 11/28/23
//
// ------------------------------------------------------------

// Include the *definitions* of IRremote library functions, i.e., their actual
// implementations. We must include this before including our own "irremote.h"
// because <IRremote.hpp> contains a header guard, so attempting to re-include
// it afterwards would do nothing.
// NOTE: Make sure to define the same macros, e.g. `IR_USE_AVR_TIMER2`, in both
// "irremote.h" and "irremote.cpp" to avoid One Definition Rule violations.
#define IR_USE_AVR_TIMER2
#include <IRremote.hpp>

// We've already included <IRremote.hpp> once, so including it again in our own
// header is harmless.
#include "irremote.h"



void IRremote_commands::printcontrol(unsigned int rslt_){
switch(rslt_){

      case power:
      Serial.println("power");//serialEventRun
        break;
      case vol_up:
        Serial.println("Volume_Up");
        break;
      case F_Stop:
      Serial.println("function_stop");
        break;
      case goBck:
      Serial.println("go Back ");
        break;
      case ppause:
      Serial.println("play/pause");


    //I only did it for 5 I plan to do the rest later

    }
}

  void IRremote_commands::irconfigs(){ //one-time set-up
    IrReceiver.begin(IR_RECEIVE_PIN,true);
  }


  // For the irremote to read into the enum
  IRremote_commands::ircommand IRremote_commands::readIrcommand(){
    ircommand rslt = ir_invalid;
      if(IrReceiver.decode()){
      // We want to read the decoded result
      rslt = static_cast<ircommand>(IrReceiver.decodedIRData.command);
      IrReceiver.resume();
      }

    return rslt;
  }
