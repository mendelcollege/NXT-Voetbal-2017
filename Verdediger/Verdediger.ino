#include <NewPing.h>
#include <Grove_I2C_Motor_Driver.h>

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

enum motor
{
    M1 = 0,
    M2 = 1,
    M3 = 2,
    M4 = 3
    MOTORS = 4;
};

//#define md1 Motor;
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
}

void LinearDrive(const char x, const char y, const char rot)
{
    int motorpwr[MOTORS], maxpwr;
    motorpwr[M1] =  x + y;
    motorpwr[M2] =  x - y;
    motorpwr[M3] = -x - y;
    motorpwr[M4] = -x + y;
    maxpwr = abs(x) + abs(y);
    if(maxpwr > 100) for(byte i = M1; i <= M4; i++) motorpwr[i] = motorpwr[i] * 100 / maxpwr;
    if(rot + maxpwr > 100) rot = 100 - maxpwr;
    if(rot - maxpwr < -100) rot = -100 - maxpwr;
    for(byte i = M1; i <= M4; i++) motorpwr[i] += rot;
    md1.speed(motorpwr[M1], motorpwr[M4]);
    md2.speed(motorpwr[M2], motorpwr[M3]);
}

void RotationalDrive(int x, int y, const char rot)
{
    int motorpwr[MOTORS], maxpwr, minpwr;
    motorpwr[M1] =  x + y;
    motorpwr[M2] =  x - y;
    motorpwr[M3] = -x - y;
    motorpwr[M4] = -x + y;
    maxpwr = abs(x) + abs(y);
    min = ((r < l) ? r : l);
    min = ((min < b) ? min : b);
    if(max + rot > 100 || min - rot < -100)
    {
        r = r * 100 / ((rot >> 7) ? (-100 + rot) : (100 - rot));
        l = l * 100 / ((rot >> 7) ? (-100 + rot) : (100 - rot));
        b = b * 100 / ((rot >> 7) ? (-100 + rot) : (100 - rot));
    }
    r += rot;
    l += rot;
    b += rot;
    OnRev(MOTORRIGHT, r);
    OnRev(MOTORLEFT, l);
    OnRev(MOTORBACK, b);
}

void setup()
{
    Serial.begin(9600);
    md1.begin(0x0f);
    md2.begin(0x0e);
}

void loop()
{
  // put your main code here, to run repeatedly:

}
