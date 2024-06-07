#ifndef Pid_h
#define Pid_h
 
#include <Arduino.h>
 
class Pid
{
public:
	Pid(float kp, float ki, float kd, long Ts);  
	float update(float error); //signal in seconds
    void timeToWait();
    void start(){
        lastUpdated = millis();
    }
private:
    long lastUpdated;
    float KP;
    float KI;
    float KD;
    float errorIntegral;
    float previousError;
    long Ts;
};
 
#endif