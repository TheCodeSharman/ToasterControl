#include "PidController.h"

PidController::PidController( Sensor& input, ControlledDevice& output )
        : input( input ), output( output), 
          pid(&inputValue,&outputValue,&setPoint, 0, 0, 0, P_ON_E, DIRECT )
          {
    PidCalibration defaultCal;
    setPidCalibration(defaultCal);
}

void PidController::process() {
    // still read input value even if we are disabled for monitoring purposes
    inputValue = input.readSensor();
    if ( pid.Compute() ) {
        output.setValue(outputValue);
    }
}

void PidController::setPidCalibration( const PidCalibration& calibration ) {
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
    setSetPoint(0);
    outputValue = 0;
}