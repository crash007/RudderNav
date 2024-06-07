#include "main.h"


LSM303 compass;

Rudder rudder(MOVE_OUT_PIN,MOVE_IN_PIN);

Pid regulator(KP, KI , KD, TS);

float referenceHeading = 0;
float previousHeading = 0;

void setup() {

  Serial.begin(9600);

   // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

//   pinMode(MOVE_IN_PIN,OUTPUT);
//   pinMode(MOVE_OUT_PIN,OUTPUT);
  Wire.begin();
  Serial.print("init: ");
  
  Serial.println(compass.init());
  compass.enableDefault();
  Serial.print("type: ");
  Serial.println(compass.getDeviceType());
 /*  compass.readMag();
 snprintf(report, sizeof(report), "A: %6d %6d %6d    M: %6d %6d %6d",
    compass.a.x, compass.a.y, compass.a.z,
    compass.m.x, compass.m.y, compass.m.z);
  Serial.println(report); */

  //init reference heading
  //compass.read();
  referenceHeading = getMedianHeading(compass, 15);
  previousHeading = referenceHeading;
  Serial.print("Reference heading: ");
  Serial.println(referenceHeading);
  
  rudder.moveBaboard(6000);
  rudder.moveStarboard(3000);

  rudder.resetPosition();
  regulator.start();
}




void loop() {
  

    Serial.print("Heading: ");

    float currentHeading = getMedianHeading(compass, 10);
    float heading = lowPassFilter(currentHeading, previousHeading);
    float error = heading - referenceHeading;
    Serial.print("Error: ");
    Serial.println(error);
    float position = regulator.update(error);
   
 
    Serial.println(heading);
    previousHeading = heading;

    Serial.print("Position: ");
    Serial.println(position);
  
    rudder.moveToPosition(position*1000);

   
   regulator.timeToWait();
   
   //delay(1000);
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