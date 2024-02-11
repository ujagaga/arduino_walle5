#include "i2c_devs.h"

TwoWire I2CWire = TwoWire(0);


void I2C_devs_init(){
  I2CWire.begin(I2C_SDA, I2C_SCL, 400000);    
}



