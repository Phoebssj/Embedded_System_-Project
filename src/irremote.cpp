// irremote.cpp
// Desc: Implementation of the irremote
// Audit: 11/28/23
//
// ------------------------------------------------------------


//#include "irremote_config.h" Commented out the ir configs as the constants are now in the irremote.h header file
#include "irremote.h"
#include <IRremote.h>



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
    
    };
	
	void IRremote_commands::irconfigs(){ //one-time set-up
		IrReceiver.begin(IR_RECEIVE_PIN,true);
	}

  
	// For the irremote to read into the enum
	ircommand::IRremote_commands readIrcommand(){
		  if(IrReceiver.decode()){     
			rslt = static_cast<ircommand>(IrReceiver.decodedIRData.command); // We want to read the decoded result
		  }
		
		return rslt;
