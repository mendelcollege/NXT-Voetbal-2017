#ifndef __SENSORS_H__
#define __SENSORS_H__

#include <Wire.h>
#include <NewPing.h>
#include <IRSeeker.h>
#include <Adafruit_L3GD20.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_9DOF.h>
#include <Adafruit_Sensor.h>

//Compass
sensors_event_t mag_event;
sensors_vec_t  orientationvector;
int orient;

//IR
struct InfraredResult irdata;
char baldir;
int balldist;

//Ultrasone
#define MAXUSVAL 300
#define AUTOCENTER

#define USLPIN
#define USBPIN
#define USRPIN

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
    mag.getEvent(&mag_event);
    InfraredSeeker.Initialize();
    usl0 = usl.ping_median();
    usb0 = usb.ping_median();
    usr0 = usr.ping_median();
    #ifdef AUTOCENTER
    xcenter = (usl0 + usr0) >> 1;
    #endif
}

void UpdateSensorValues();
{
    int uslval, usbval, usrval;
    uslval = usl.pin_cm();
    usbval = usb.pin_cm();
    usrval = usr.pin_cm();
    #ifdef AUTOCENTER
    if(uslval > usrval) xpos = uslval - xcenter;
    else xpos = xcenter - usrval;
    #else
    if(uslval > usrval) xpos = uslval - usl0;
    else xpos = usr0 - usrval;
    #endif
    orient = magGetOrientation ( sensors_axis_t axis, sensors_event_t *event, sensors_vec_t *mag_orientation )
    dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)
    orient = (int) orientasion.pitch; //Which one of them is the question. Pitch, yawn or azimsdjfhskdhf?
    while(orient < -180) orient += 360;
    while(orient > 180) orient -=360;
}

#endif
