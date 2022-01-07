#include "gcodes/M104.h"

bool M104::match() {
    if ( GCode.HasWord('M') ) {
       if ( (int)GCode.GetWordValue('M') == 104 ) {
            return GCode.HasWord('S');
       }
    }
    return false;
}

void M104::execute() {
    double temp = GCode.GetWordValue('S');
    output.printf("Target Temperature = %f °C\r\n", temp);
}