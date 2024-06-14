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







