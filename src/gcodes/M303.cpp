#include "gcodes/M303.h"

bool M303::match() {
    if ( GCode.HasWord('M') ) {
       if ( (int)GCode.GetWordValue('M') == 303 ) {
            return GCode.HasWord('S');
       }
    }
    return false;
}

void M303::execute() {
    double temp = GCode.GetWordValue('S');
    if ( oven.getIsTuning() ) {
        if ( temp > 0 ) {
            output.printf("ERROR: PID is already autotuning!\r\n");
            return;
        }
    }
    if ( temp > 0 ) {
        oven.startAutoTune(temp);
        output.printf("; Started PID auto tune\r\n");
    } else {
        oven.cancelAutoTune();
        output.printf("; Cancelled PID auto tune\r\n");
    }
}