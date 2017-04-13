#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Wire.h>
#include <NewPing.h>
#include <Adafruit_L3GD20.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_9DOF.h>
#include <Adafruit_Sensor.h>
#include "IRSeeker.h"

//Compass
Adafruit_9DOF dof = Adafruit_9DOF();
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(40273);
int orient;

//IR
struct InfraredResult irdata;
char balldir;
int balldist;

//Ultrasone
#define MAXUSVAL 300
#define AUTOCENTER

#define USLPIN 0
#define USBPIN 1
#define USRPIN 2

NewPing usl(USLPIN, USLPIN, MAXUSVAL);
NewPing usb(USBPIN, USBPIN, MAXUSVAL);
NewPing usr(USRPIN, USRPIN, MAXUSVAL);

int xpos, ypos, usl0, usb0, usr0, xcenter;

//Field dimensions (in cm)
#define STADIUMWIDTH 182
#define STADIUMLENGTH 244

#define FIELDWIDTH 122
#define FIELDLENGTH 184

#define GOALWIDTH 60
#define GOALWALLDIST 61
#define GOALDEPTH 10

//Sensor global
void SetupSensors()
{
    InfraredSeeker::Initialize();
    if(!mag.begin())
    {
        /* There was a problem detecting the LSM303 ... check your connections */
        Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
        while(1);
    }
    usl0 = usl.ping_median();
    usb0 = usb.ping_median();
    usr0 = usr.ping_median();
    #ifdef AUTOCENTER
    xcenter = (usl0 + usr0) >> 1;
    #endif
}

void UpdateSensorValues()
{
    int uslval, usbval, usrval;
    sensors_event_t mag_event;
    sensors_vec_t orientationvector;
    
    mag.getEvent(&mag_event);
    if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientationvector))
    {
        orient = (int) orientationvector.heading; //Which one of them is the question. Pitch, yawn or azimsdjfhskdhf?
        while(orient < -180) orient += 360;
        while(orient > 180) orient -=360;
    }

    irdata = InfraredSeeker::ReadAC();
    balldir = irdata.Direction;
    balldist = irdata.Strength;
    
    uslval = usl.ping_cm();
    usbval = usb.ping_cm();
    usrval = usr.ping_cm();
    #ifdef AUTOCENTER
    if(uslval > usrval) xpos = uslval - xcenter;
    else xpos = xcenter - usrval;
    #else
    if(uslval > usrval) xpos = uslval - usl0;
    else xpos = usr0 - usrval;
    #endif
}

#endif
