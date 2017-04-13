#include <Wire.h>
#include "Sensors.h"
#include "Driving.h"


#define Cereal Serial

void setup()
{
    Cereal.begin(9600);
    SetupSensors();
    md1.begin(0x0f);
    md2.begin(0x0e);
}

void loop()
{
  // put your main code here, to run repeatedly:

}
