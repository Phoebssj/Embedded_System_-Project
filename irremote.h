//irremote header
#include "irremote_config.h"
#include <IRremote.h>

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
      //  goFwd = 6,
     //   down = 8,
     //   vol_- = 9,
      //  up = 10,
      //  bnt_0 = 12,
      //  eq = 13,
      //  rept = 14,
      //  bnt_1 = 16,
     //   bnt_2 =17,
     //   bnt_3 = 18,
    //    bnt_4 = 20,
    //    bnt_5 = 21,
    //    bnt_6 = 22,
    //    bnt_7 = 24,
     //   bnt_8 = 25,
    //    bnt_9 = 26
  	};
	//====================================================
	//Constructors:
	//=====================================================
	IRremote_commands();
	//=======================================================
	//Methods
	//=======================================================
	void printcontrol(unsigned int rslt);

	
	
}