#include "gcodes/M105.h"

bool M105::match() {
    if ( GCode.HasWord('M') ) {
       return ( (int)GCode.GetWordValue('M') == 105 );
    }
    return false;
}

void M105::execute() {
    output.printf("ok C: %2.0f (%2.0f)\r\n", oven.getInput(), oven.getSetPoint() );
 
}