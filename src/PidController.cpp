#include "PidController.h"

#include <functional>
#include <numeric>

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
    lastError = 0;
    error = 0;

    errors.clear();
    errors.resize(averageN);
    slope = 0;
}

void PidController::process() {
    
    inputValue = input.readSensor();

    errors.push_back(setPoint - inputValue);
    if ( errors.size() > averageN )
        errors.pop_front();

    // Average the sensor samples, this serves to filter out sensor noise
    // and should create a smooth slope instead of being highly erratic
    // due to quantisation of the ADC.
    error = std::accumulate(errors.begin(), errors.end(), 0.0)/errors.size();
    slope = error - lastError;
    lastError = error;

    P = calibration.Kp * error;
    I = I + calibration.Ki * error;
    D = calibration.Kd * slope;

    outputValue = P + I + D;
    if ( outputValue > outputMax ) outputValue = outputMax;
    if ( outputValue < outputMin ) outputValue = outputMin; 
    output.setValue(outputValue);
}

void PidController::setPidCalibration( const PidCalibration& calibration ) {
    this->calibration = calibration;  
    I = 0; // zero the accumulated I so that a large incorrect I
           // value won't delay testing thew new value.
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