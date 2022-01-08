#include "PidController.h"

PidController::PidController( Sensor& input, ControlledDevice& output, PidCalibration& calibration )
        : input( input ), output( output), calibration(calibration),
          pid(&inputValue,&outputValue,&setPoint, calibration.Kp, calibration.Ki, calibration.Kd, calibration.P_MODE, DIRECT ) {
}

void PidController::process() {
    // still read input value even if we are disabled for monitoring purposes
    inputValue = input.readSensor();
    if ( pid.Compute() ) {
        output.setValue(outputValue);
    }
}

void PidController::setPidCalibration( PidCalibration& calibration ) {
    this->calibration = calibration;
    pid.SetTunings(
        calibration.Kp,
        calibration.Ki,
        calibration.Kd,
        calibration.P_MODE
    );
}

void PidController::start( double setPoint ) {
    setSetPoint(setPoint);
    pid.SetMode(AUTOMATIC);
}

void PidController::stop() {
    pid.SetMode(MANUAL);
    setSetPoint(setPoint);
    output.setValue(0);
}