#include "gcodes/M301.h"

bool M301::match() {
    if ( GCode.HasWord('M') ) {
       return ( (int)GCode.GetWordValue('M') == 301 );
    }
    return false;
}

void M301::execute() {
    PidCalibration& calibration = pid.getPidCalibration();
    if ( GCode.HasWord('P'))
        calibration.Kp = GCode.GetWordValue('P');
    if ( GCode.HasWord('I'))
        calibration.Ki = GCode.GetWordValue('I');
    if ( GCode.HasWord('D'))
        calibration.Kd = GCode.GetWordValue('D');
    if ( GCode.HasWord('Z'))
        calibration.P_MODE = (int)GCode.GetWordValue('Z');
    pid.setPidCalibration(calibration);

    output.printf("// Oven chamber PID\r\n");
    output.printf("//   M301 P%0.2f I%0.2f D%0.2f Z%i\r\n",
            calibration.Kp,
            calibration.Ki,
            calibration.Kd,
            calibration.P_MODE );
    output.printf("ok\r\n");
}