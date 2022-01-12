#include "gcodes/M500_503.h"

bool M500_503::match() {
    if ( GCode.HasWord('M') ) {
        int c = (int)GCode.GetWordValue('M');
        return ( c == 500 || c == 501 || c == 502 || c == 503 );
    }
    return false;
}

void M500_503::execute() {
    int c = (int)GCode.GetWordValue('M');
    switch(c) {
        case 500: 
            settings.saveSettings();
            output.printf("// Settings saved\r\n");
            break;

        case 501:
            settings.loadSettings();
            output.printf("// Settings loaded\r\n");
            break;

        case 502:
            output.printf("// Settings reset\r\n");
            settings.resetSettings();
            break;
    }

    PidCalibration& pidCal = settings.getPidCalibration();
    KProbeCalibration& probeCal = settings.getProbeCalibration();
    output.printf("// Toaster oven settings:\r\n");
    output.printf("//   PidController: Kp = %0.2f Ki = %0.2f Kd = %0.2f\r\n", 
        pidCal.Kp, pidCal.Ki, pidCal.Kd);
    output.printf("//   KTypeProbe: A: (temp:%0.2f, adc:%i) B: (temp:%0.2f, adc:%i)\r\n",
        probeCal.tempOffsetA, probeCal.adcA,
        probeCal.tempOffsetB, probeCal.adcB);
    output.printf("ok\r\n");
}