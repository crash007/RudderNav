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
    void setTunings(float kp, float ki, float kd, float Ts) {
        this->KP = kp;
        this->KI = ki;
        this->KD = kd;
        this->Ts = Ts;
    }
private:
    long lastUpdated;
    float KP;
    float KI;
    float KD;
    float errorIntegral;
    float previousError;
    float Ts;
};
 
#endif