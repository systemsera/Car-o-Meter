/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/Chris/Desktop/Msc/IoT/Car-o-Meter/src/Car-o-Meter.ino"

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_LIS3DH.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_Sensor.h>

/*
 * Project Car-o-Meter
 * Description:
 * Author:
 * Date:
 */

void setup();
void loop();
void showStats();

Adafruit_LIS3DH lis = Adafruit_LIS3DH();
sensors_event_t event;

const int buttonPin = D6;
const int ledPin = D7;

int xAcceleration;
int yAcceleration;
int zAcceleration;

int countX, countY, countZ;

int eventX[1000];
int eventY[1000];
int eventZ[1000];

int pastX = 0;
int pastY = 0;
int pastZ = 0;

bool onDrive = true;

char xStr[10];
char yStr[10];
char zStr[10];

char countXstr[10];
char countYstr[10];
char countZstr[10];

char meanXstr[10];
char meanYstr[10];
char meanZstr[10];

#line 9 "c:/Users/Chris/Desktop/Msc/IoT/Car-o-Meter/src/Car-o-Meter.ino"

void setup() {
  // Put initialization like pinMode and begin functions here.
if (! lis.begin(0x18)) {  
        Particle.publish("ev","LIS3DH configuration error");
    }
    
    lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
    Particle.publish("ev","LIS3DH configured successfully");
    
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);

    delay(1000);
}

void loop() {

  while (onDrive == true){
    
    lis.getEvent(&event);
    xAcceleration = event.acceleration.x;
    yAcceleration = event.acceleration.y;
    zAcceleration = (event.acceleration.z)-1;
    
    sprintf(xStr, "%d",xAcceleration);
    sprintf(yStr, "%d",yAcceleration);
    sprintf(zStr, "%d",zAcceleration);

    int buttonState=digitalRead(buttonPin);
    
    if (buttonState==LOW){
        digitalWrite(ledPin, HIGH);
        onDrive = false;
        showStats();
    }else{
        digitalWrite(ledPin, LOW);
    }

    if (xAcceleration != pastX){
        Particle.publish("X",xStr);
          if(xAcceleration < 0){
          eventX[countX] = xAcceleration*(-1);
        } else {
          eventX[countX] = xAcceleration;
        }
        countX++;

        } else if (yAcceleration != pastY){
          Particle.publish("Y",yStr);
            if (yAcceleration < 0){
              eventY[countY] = yAcceleration*(-1);
            } else {
              eventY[countY] = yAcceleration;
            }
            countY++;

            } else if (zAcceleration != pastZ){
              Particle.publish("Z",zStr);
                if (zAcceleration < 0){
                  eventZ[countZ] = zAcceleration*(-1);
                } else {
                  eventZ[countZ] = zAcceleration;
                }
                countZ++;
      } else {
      return;
    }

    delay(50);
  }
}

void showStats(){

      int sumX = 0;
      int sumY = 0;
      int sumZ = 0;
      int score = 0;

      char sumXstr[10];
      char sumYstr[10];
      char sumZstr[10];

      char scoreStr[10];

      for (int i = 0; i <= countX; i++){
          sumX += eventX[i];
      }
      for (int k = 0; k <= countY; k++){
          sumY += eventY[k];
      }
      for (int p = 0; p <= countZ; p++){
          sumZ += eventZ[p];
      }

      sprintf(sumXstr, "%d",sumX);
      sprintf(sumYstr, "%d",sumY);
      sprintf(sumZstr, "%d",sumZ);
      sprintf(countXstr, "%d",countX);
      sprintf(countYstr, "%d",countY);
      sprintf(countZstr, "%d",countZ);
      sprintf(meanXstr, "%f",countX/(float)sumX);
      sprintf(meanYstr, "%f",countY/(float)sumY);
      sprintf(meanZstr, "%f",countZ/(float)sumZ);
      sprintf(scoreStr, "%f",((countX/(float)sumX)+(countY/(float)sumY)+(countZ/(float)sumZ))/3);
        Particle.publish("Drive has ended.");
        Particle.publish("X Events:",countXstr);
        Particle.publish("Y Events:",countYstr);
          delay(1000);
          Particle.publish("Z Events:",countZstr);
          Particle.publish("Sum of X:",sumXstr);
          Particle.publish("Sum of Y:",sumYstr);
            delay(1000);
            Particle.publish("Sum of Z:",sumZstr);
            Particle.publish("Your X Acceleration Score:",meanXstr);
            Particle.publish("Your Y Acceleration Score: ",meanYstr);
              delay(1000);
              Particle.publish("Your Z Acceleration Score: ",meanZstr);
              Particle.publish("Your Total Score:",scoreStr);
  }