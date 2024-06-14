#include <Arduino.h>
#include "Rudder.h"

Rudder::Rudder(int starboardPin, int baboardPin)
{
    STARBOARD_PIN = starboardPin;
    BABOARD_PIN = baboardPin;
    
    pinMode(STARBOARD_PIN, OUTPUT);
    pinMode(BABOARD_PIN, OUTPUT);
    digitalWrite(STARBOARD_PIN, HIGH);
     digitalWrite(BABOARD_PIN, HIGH);
    
    lastPosition = 0;
}

/**
 * Moves the rudder to the specified position.
 * 
 * @param position The target position in milliseconds.
 */
void Rudder::moveToPosition(long position)
{

    long deltaTime = position - lastPosition;
    Serial.print("deltaTime: ");
    Serial.println(deltaTime);

    int direction;

    if (deltaTime > 0)
    {
        direction = STARBOARD_PIN;
    }
    else
    {
        direction = BABOARD_PIN;
    }
    if(abs(deltaTime) > 200){
        pinLow(direction, deltaTime);
        lastPosition = position;
    }
    
}

long Rudder::moveStarboard(long time){
    pinLow(STARBOARD_PIN,time);
    lastPosition = lastPosition+time;
    return lastPosition;
}

long Rudder::moveBaboard(long time){
    
    pinLow(BABOARD_PIN,time);
    lastPosition = lastPosition-time;
    return lastPosition;
}