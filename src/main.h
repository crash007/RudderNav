#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Rudder.h>
#include <Wire.h>
#include <Pid.h>
#include <LSM303.h>

// Pin definitions
#define LED_BUILTIN 2
#define MOVE_OUT_PIN 14
#define MOVE_IN_PIN 12

// Sampling time
#define TS 0.5

// PID constants
#define KP 0.03
#define KI 0.005
#define KD 0.005
#define ALPHA 0.1 

// Function prototypes
float getMedianHeading(LSM303 &compass, int numReadings);
float lowPassFilter(float currentHeading, float previousHeading);
float getTiltCompensatedHeading(LSM303 &compass);

#endif // MAIN_H