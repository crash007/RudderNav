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
#define KI 0.0005
#define KD 0.005
#define ALPHA 0.1 

// Global variables
extern float referenceHeading;
extern float previousHeading;
extern float currentHeading;
extern float heading;
extern float error;
extern float position;

float lowPassFilter(float currentHeading, float previousHeading) {
    return ALPHA * currentHeading + (1 - ALPHA) * previousHeading;
}

float getTiltCompensatedHeading(LSM303 &compass) {
    // Read accelerometer and magnetometer data
    LSM303::vector<short> accelRaw = compass.a;
    LSM303::vector<short> magRaw = compass.m;

    // Convert raw data to float
    LSM303::vector<float> accel = {static_cast<float>(accelRaw.x), static_cast<float>(accelRaw.y), static_cast<float>(accelRaw.z)};
    LSM303::vector<float> mag = {static_cast<float>(magRaw.x), static_cast<float>(magRaw.y), static_cast<float>(magRaw.z)};

    // Normalize accelerometer data
    float normAccel = sqrt(accel.x * accel.x + accel.y * accel.y + accel.z * accel.z);
    float ax = accel.x / normAccel;
    float ay = accel.y / normAccel;
    float az = accel.z / normAccel;

    // Calculate pitch and roll
    float pitch = asin(-ax);
    float roll = asin(ay / cos(pitch));

    // Tilt-compensate magnetometer data
    float mx = mag.x * cos(pitch) + mag.z * sin(pitch);
    float my = mag.x * sin(roll) * sin(pitch) + mag.y * cos(roll) - mag.z * sin(roll) * cos(pitch);

    // Calculate heading
    float heading = atan2(my, mx) * 180.0 / M_PI;

    // Correct for declination (example declination angle, adjust accordingly)
    float declinationAngle = 1.0; // in degrees
    heading += declinationAngle;

  /*   // Normalize to 0-360 degrees
    if (heading < 0) {
        heading += 360;
    }
    if (heading >= 360) {
        heading -= 360;
    } */

    return heading;
}

float getMedianHeading(LSM303 &compass, int numReadings) {
    float readings[numReadings];

    // Take the specified number of readings
    for (int i = 0; i < numReadings; i++) {
        compass.read();
        float heading = compass.heading();
        if(heading > 180) {
            heading -= 360;
        }
        readings[i] = heading;
        delay(1); // Short delay between readings
    }

    // Sort the readings array
    for (int i = 0; i < numReadings - 1; i++) {
        for (int j = i + 1; j < numReadings; j++) {
            if (readings[i] > readings[j]) {
                float temp = readings[i];
                readings[i] = readings[j];
                readings[j] = temp;
            }
        }
    }

/* Serial.print("Min heading: ");
Serial.print(readings[0]);
Serial.print(" Max heading: ");
Serial.println(readings[numReadings - 1]); */
    // Return the median value
    return readings[numReadings / 2];
}


#endif // MAIN_H