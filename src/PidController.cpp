#include "PidController.h"

#include <functional>

PidController::PidController( Sensor& input, ControlledDevice& output, MultiTask& tasks )
        : input( input ), output( output), tasks(tasks) {
    PidCalibration defaultCal;
    setPidCalibration(defaultCal);
    
}

void PidController::resetParameters() {
    outputValue = 0;
    I = 0;
    P = 0;
    D = 0;
    error = 0;
    lastError = 0;
    slopes.clear();
    slopes.resize(averageSlopeN);
    slope = 0;
}

void PidController::process() {
    
    inputValue = input.readSensor();
    error = setPoint - inputValue;

    // use the mean of the last slopes 
    slope = 0.0;
    for( auto &s : slopes )
      slope += s;
    slope = slope / slopes.size();
    
    P = calibration.Kp * error;
    I = I + calibration.Ki * error;
    D = calibration.Kd * slope;

    slopes.pop_front();
    slopes.push_back( error - lastError );
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
    resetParameters();
    setSetPoint(setPoint);
    
    if ( processLoop == NULL ) {
        processLoop = tasks.every(samplePeriod,std::bind(&PidController::process,this));
    } else {
        processLoop->setPeriod(samplePeriod);
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