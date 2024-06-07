#include <Arduino.h>
#include "Pid.h"
 
Pid::Pid(float kp, float ki, float kd, long ts){
 KP = kp;
 KI = ki;
 KD = kd;
 
 errorIntegral = 0;
 Ts = ts;
 previousError = 0;
}

float Pid::update(float error) {
    // Calculate time elapsed since last update in seconds
    float deltaTimeSeconds = (millis() - lastUpdated) / 1000.0;
    lastUpdated = millis();  // Update the timestamp for the next iteration

    // Update integral of the error
    errorIntegral += error * deltaTimeSeconds;

    // Calculate derivative of the error
    float errorDerivative = (error - previousError) / deltaTimeSeconds;
    previousError = error;

    // Compute PID output
    float output = KP * error + KI * errorIntegral + KD * errorDerivative;
    
    return output;
}


void Pid::timeToWait() {
    long timeToWait = Ts * 1000 - (millis() - lastUpdated);
   
    if (timeToWait > 0) {
       //Serial.print("Time to wait: ");
       //Serial.println(timeToWait);
       delay(timeToWait);
    }
}