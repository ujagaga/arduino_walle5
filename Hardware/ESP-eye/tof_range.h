#ifndef TOF_RANGE_H
#define TOF_RANGE_H

#define I2C_SDA               15
#define I2C_SCL               14
#define TOF_1_IIC_ADDRESS     0x29

bool TOF_init(void);
uint8_t TOF_get_range(void);

#endif