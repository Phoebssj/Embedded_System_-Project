#include "irremote_config.h"
#include "irremote.h"


void IRremote_commands::printcontrol(unsigned int rslt){
switch(rslt){
    	
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


}