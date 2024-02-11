#ifndef TOF_RANGE_H
#define TOF_RANGE_H

#include "i2c_devs.h"

#define TOF_1_IIC_ADDRESS     0x29

void TOF_init(void);
uint8_t TOF_get_range(void);

#endif