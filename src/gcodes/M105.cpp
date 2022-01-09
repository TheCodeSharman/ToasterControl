#include "gcodes/M105.h"

bool M105::match() {
    if ( GCode.HasWord('M') ) {
       return ( (int)GCode.GetWordValue('M') == 105 );
    }
    return false;
}

void M105::execute() {
    output.printf("ok C: %0.0f (%0.0f) ; coldJunction = %i adc = %i isTuning=%i\r\n", 
        oven.getInput(), 
        oven.getSetPoint(),
        probe.getColdJunction()/1000,
        probe.getProbeAdc(),
        oven.getIsTuning() );
}