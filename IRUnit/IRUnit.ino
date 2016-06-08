#include <Wire.h>
#include "DisableTWIPullUp.h"

/*
 * Taken van deze sketch:
 * - TSOPS uitlezen (met registers)
 * - Positie en afstand schatten / Jerklock Holmes uithangen
 * - Data doorgeven met I2C
 * - Kaas maken
 * En als die dingen geprogrammeerd zijn, zal eindelijk de messias op aarde komen.
 */

void setup()
{
  Wire.begin();
  DisableTWIPullUp();
}

void loop()
{
  
}
