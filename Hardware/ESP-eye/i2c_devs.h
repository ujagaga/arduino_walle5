#ifndef I2C_DEVS_H
#define I2C_DEVS_H

#include <Wire.h>

#define I2C_SDA               15
#define I2C_SCL               14
#define TOF_1_IIC_ADDRESS     0x29

extern TwoWire I2CWire;

void I2C_devs_init(void);


#endif