#include "gcodes/M306.h"

bool M306::match() {
    if ( GCode.HasWord('M') ) {
       return ( (int)GCode.GetWordValue('M') == 306 );
    }
    return false;
}

void M306::execute() {
    KProbeCalibration& calibration = probe.getProbeCalibration();
    if ( GCode.HasWord('A'))
        calibration.tempOffsetA = (int)GCode.GetWordValue('A');
    if ( GCode.HasWord('B'))
        calibration.adcA = (int)GCode.GetWordValue('B');
    if ( GCode.HasWord('C'))
        calibration.tempOffsetB = (int)GCode.GetWordValue('C');
    if ( GCode.HasWord('D'))
        calibration.adcB = (int)GCode.GetWordValue('D');
    probe.setProbeCalibration(calibration);

    output.printf("; Oven chamber K Type probe\r\n");
    output.printf("   M306 A%i B%i C%i D%i\r\n",
            calibration.tempOffsetA,
            calibration.adcA,
            calibration.tempOffsetB,
            calibration.adcB );
}