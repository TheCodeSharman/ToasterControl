#include "gcodes/M305.h"

bool M305::match() {
    if ( GCode.HasWord('M') ) {
       return ( (int)GCode.GetWordValue('M') == 306 );
    }
    return false;
}

void M305::execute() {
    KProbeCalibration& calibration = probe.getProbeCalibration();
    if ( GCode.HasWord('A'))
        calibration.tempOffsetA = GCode.GetWordValue('A');
    if ( GCode.HasWord('B'))
        calibration.adcA = (int)GCode.GetWordValue('B');
    if ( GCode.HasWord('C'))
        calibration.tempOffsetB = GCode.GetWordValue('C');
    if ( GCode.HasWord('D'))
        calibration.adcB = (int)GCode.GetWordValue('D');
    probe.setProbeCalibration(calibration);

    output.printf("; Oven chamber K Type probe\r\n");
    output.printf(";   M305 A%0.2f B%i C%0.2f D%i\r\n",
            calibration.tempOffsetA,
            calibration.adcA,
            calibration.tempOffsetB,
            calibration.adcB );
    output.printf("ok\r\n");
}