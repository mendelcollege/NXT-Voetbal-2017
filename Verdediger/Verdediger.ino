#define Cereal Serial //I am very funny

#include <Wire.h>
#include "Sensors.h"
#include "Driving.h"


bool motorenabled;
bool transmissionenabled;
bool sleep;

void setup()
{
    Cereal.begin(9600);
    SetupSensors();
    SetupMotors();
}

void loop()
{
    UpdateSensorValues();
    if(UCSR0A & _BV(TXC0))
    {
        TransmitSensorValues();
    }
}
