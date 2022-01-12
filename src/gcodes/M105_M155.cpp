#include "gcodes/M105_M155.h"

#include <functional>

bool M105_M155::match() {
    if ( GCode.HasWord('M') ) {
        const int c = (int)GCode.GetWordValue('M');
       if ( c == 105 ) 
        return true;

       if ( c == 155 && GCode.HasWord('S') )
        return true;
    }
    return false;
}

void M105_M155::execute() {
   const int c = (int)GCode.GetWordValue('M');
   if ( c == 155 ) {
       // set the period to 0 to stop the auto reporting
       double period = GCode.GetWordValue('S')*1000.0;
       if ( reportingCallback == NULL ) {
            reportingCallback = multitask.every( period, std::bind(&M105_M155::reportTemperatures, this) );
       } else {
            reportingCallback->setPeriod(period);
       }
   } else {
        reportTemperatures();
   }
   output.printf("ok\r\n");
}

void M105_M155::reportTemperatures() {
 output.printf("T: %0.2f (%0.0f) ; coldJunction = %0.2f adc = %i output = %0.2f error = %0.2f\r\n", 
        oven.getInput(), 
        oven.getSetPoint(),
        probe.getColdJunction()/1000.0,
        probe.getProbeAdc(),
        oven.getOutput(),
        oven.getError());
}