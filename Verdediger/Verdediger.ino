#include <Wire.h>
#include <NewPing.h>
#include <Grove_I2C_Motor_Driver.h>


#define Cereal Serial


//Driving
#define MOTORDRIVER1ADRESS 0x0f
#define MOTORDRIVER2ADRESS 0x0e

/* 
 *    M1             M2 
 *      \           /
 *      (1)       (1)
 *      md1       md2
 *      (2)       (2)
 *      /           \
 *    M4             M3
 * 
 * Positive rotational value should corespond to:
 *  - Counterclockwise wheelrotation
 *  - Clockwise robot rotation
 * 
 */

//Drive Stuff
enum motor
{
    M1 = 0,
    M2 = 1,
    M3 = 2,
    M4 = 3,
    MOTORS = 4
};

enum drivetype
{
    PROPORTIONAL,
    LINEAR,
    ROTATIONAL,
    DIRECTIONAL,
    COMPASS
};

struct drivestate
{
    char x;
    char y;
    char rot;
    byte pwr;
    int dir;
    byte type;
} currentdrivestate;

I2CMotorDriver md1;
I2CMotorDriver md2;

void ProportionalDrive(const char x, const char y, const char rot, const char pwr)
{
    int motorpwr[MOTORS], maxpwr = 0;
    motorpwr[M1] =  x + y + rot;
    motorpwr[M2] =  x - y + rot;
    motorpwr[M3] = -x - y + rot;
    motorpwr[M4] = -x + y + rot;
    maxpwr = + abs(x) + abs(y) + abs(rot);
    for(byte i = M1; i <= M4; i++) motorpwr[i] = motorpwr[i] * pwr / maxpwr;
    md1.speed(motorpwr[M1], motorpwr[M4]);
    md2.speed(motorpwr[M2], motorpwr[M3]);
    currentdrivestate.x = x;
    currentdrivestate.y = y;
    currentdrivestate.rot = rot;
    currentdrivestate.pwr = pwr;
    currentdrivestate.type = PROPORTIONAL;
}

void LinearDrive(const char x, const char y, char rot)
{
    int motorpwr[MOTORS], maxpwr;
    motorpwr[M1] =  x + y;
    motorpwr[M2] =  x - y;
    motorpwr[M3] = -x - y;
    motorpwr[M4] = -x + y;
    maxpwr = abs(x) + abs(y);
    if(maxpwr > 100) for(byte i = M1; i <= M4; i++) motorpwr[i] = motorpwr[i] * 100 / maxpwr;
    else
    {
        if(rot + maxpwr > 100) rot = 100 - maxpwr;
        if(rot - maxpwr < -100) rot = -100 + maxpwr;
        for(byte i = M1; i <= M4; i++) motorpwr[i] += rot;   
    }
    md1.speed(motorpwr[M1], motorpwr[M4]);
    md2.speed(motorpwr[M2], motorpwr[M3]);
    currentdrivestate.x = x;
    currentdrivestate.y = y;
    currentdrivestate.rot = rot;
    currentdrivestate.type = LINEAR;
}

void RotationalDrive(int x, int y, const char rot)
{
    int motorpwr[MOTORS], maxpwr, minpwr;
    motorpwr[M1] =  x + y;
    motorpwr[M2] =  x - y;
    motorpwr[M3] = -x - y;
    motorpwr[M4] = -x + y;
    maxpwr = abs(x) + abs(y);
    if(maxpwr + rot > 100 || -maxpwr + rot < -100) for(byte i = M1; i <= M4; i++) motorpwr[i] = motorpwr[i] * 100 / ((rot >> 7) ? (-100 + rot) : (100 + rot)) + rot; //Can u follow it?
    else for(byte i = M1; i <= M4; i++) motorpwr[i] += rot;
    md1.speed(motorpwr[M1], motorpwr[M4]);
    md2.speed(motorpwr[M2], motorpwr[M3]);
    currentdrivestate.x = x;
    currentdrivestate.y = y;
    currentdrivestate.rot = rot;
    currentdrivestate.type = ROTATIONAL;
}

void DirectionalDrive(int dir, const char dirvel, const char rotvel, const byte power)
{
    int x, y;
    float dirrad = dir * PI / 180;
    x = (sin(dirrad) * dirvel);
    y = (cos(dirrad) * dirvel);
    ProportionalDrive(x, y, rotvel, power);
    currentdrivestate.dir = dir;
    currentdrivestate.type = DIRECTIONAL;
}

void CompassDrive(int reldir, const char dirvel, const char rotvel, const byte power)
{
    int x, y;
    reldir -= orient;
    while(reldir <= -180) reldir += 360;
    while(reldir >   180) reldir -= 360;
    DirectionalDrive(reldir, dirvel, rotvel, power);
    currentdrivestate.dir = reldir;
    currentdrivestate.type = COMPASS;
}

void setup()
{
    Cereal.begin(9600);
    Wire.begin();
    md1.begin(0x0f);
    md2.begin(0x0e);
}

void loop()
{
  // put your main code here, to run repeatedly:

}
