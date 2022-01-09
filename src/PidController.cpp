#include "PidController.h"

PidController::PidController( Sensor& input, ControlledDevice& output )
        : input( input ), output( output), 
          pid(&inputValue,&outputValue,&setPoint, 0, 0, 0, P_ON_E, DIRECT ),
          pidAutoTune(&inputValue,&outputValue) {
    PidCalibration defaultCal;
    setPidCalibration(defaultCal);
}

void PidController::process() {
    // still read input value even if we are disabled for monitoring purposes
    inputValue = input.readSensor();
    if ( isTuning ) {
        // returns true when auto tune is complete
        int status = pidAutoTune.Runtime();
        if ( status == -1 ) {
            isTuning = false;
            setPidCalibration( { (float)pidAutoTune.GetKp(), 
                                 (float)pidAutoTune.GetKi(),
                                 (float)pidAutoTune.GetKd(),
                                 calibration.P_MODE });
            output.setValue(0);
        } else if ( status == 1 ) {
            output.setValue(outputValue);
        }
    } else {
        if ( pid.Compute() ) {
            output.setValue(outputValue);
        }
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

void PidController::startAutoTune( double setPoint ) {
    // firstly stop the controller if it is running
    // FIXME: is it better to wait for it to cool down?
    if ( pid.GetMode() == AUTOMATIC ) {
        stop();
    }
    
    outputValue=100; // Starting value?
    pidAutoTune.SetSetPoint(setPoint);
    pidAutoTune.SetNoiseBand(0.5); // 1 degrees noise
    pidAutoTune.SetOutputStep(200); // tune step
    pidAutoTune.SetLookbackSec(60*2); // 60 seconds look back?
    pidAutoTune.SetControlType(1);
    isTuning = true;
}

void PidController::cancelAutoTune() {
    pidAutoTune.Cancel();
    isTuning = false;
}

void PidController::stop() {
    pid.SetMode(MANUAL);
    setSetPoint(setPoint);
    output.setValue(0);
}