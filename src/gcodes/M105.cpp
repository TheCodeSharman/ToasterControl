#include "gcodes/M105.h"

bool M105::match() {
    if ( GCode.HasWord('M') ) {
       if ( (int)GCode.GetWordValue('M') == 105 ) {
            return GCode.HasWord('T');
       }
    }
    return false;
}

void M105::execute() {
    double temp = GCode.GetWordValue('T');
    output.printf("Target Temperature = %f °C\r\n", temp);
}