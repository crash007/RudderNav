#ifndef Rudder_h
#define Rudder_h
 
#include <Arduino.h>
 
 
class Rudder
{
public:
	Rudder(int starboardPin, int BaboardPin);  
	void moveToPosition(long position); //position in time in millis .
	long moveStarboard(long time); //time in millis in starboard direction
	long moveBaboard(long time); //time in millis in badboard direction
	void resetPosition(){
		lastPosition = 0;
	}
    
private:
	uint8_t STARBOARD_PIN;
    uint8_t BABOARD_PIN;
    long lastPosition;
    
	void pinLow(int pin, long time){
		Serial.print("pin: ");
		Serial.println(pin);
		Serial.print("time: ");
		Serial.println(time);
		digitalWrite(pin, LOW);
    	delay(abs(time));
    	digitalWrite(pin, HIGH);
	}
};
 
#endif