#include "PidController.h"
#include <functional>

PidController::PidController( Sensor& input, ControlledDevice& output, MultiTask& tasks )
        : input( input ), output( output), tasks(tasks) {
    PidCalibration defaultCal;
    setPidCalibration(defaultCal);
    resetParameters();
}

void PidController::resetParameters() {
    outputValue = 0;
    I = 0;
    P = 0;
    D = 0;
    error = 0;
    lastError = 0;
}

void PidController::process() {
    
    inputValue = input.readSensor();
    error = setPoint - inputValue;

    P = calibration.Kp * error;
    I = I + calibration.Ki * error;
    D = calibration.Kd * (error - lastError);
    lastError = error;

    outputValue = P + I + D;
    if ( outputValue > outputMax ) outputValue = outputMax;
    if ( outputValue < outputMin ) outputValue = outputMin; 
    output.setValue(outputValue);
}

void PidController::setPidCalibration( const PidCalibration& calibration ) {
    this->calibration = calibration;        
}

void PidController::start( double setPoint ) {
    setSetPoint(setPoint);
    if ( processLoop == NULL ) {
        processLoop = tasks.every(10,std::bind(&PidController::process,this));
    } else {
        processLoop->setPeriod(10);
    }
}

void PidController::stop() {
    if ( processLoop != NULL ) {
        processLoop->setPeriod(0); 
    }
    setSetPoint(0);
    resetParameters();
    output.setValue(outputValue);
}