#include <DFRobot_VL6180X.h>
#include <Wire.h>
#include "tof_range.h"

TwoWire I2C = TwoWire(0);
DFRobot_VL6180X VL6180X(TOF_1_IIC_ADDRESS, &I2C);


bool TOF_init(){
  I2C.begin(I2C_SDA, I2C_SCL, 200000);  
  bool result = VL6180X.begin();
  VL6180X.rangeSetInterMeasurementPeriod(30);
  return result;
}

uint8_t TOF_get_range(){
  uint8_t range = VL6180X.rangePollMeasurement();
  uint8_t status = VL6180X.getRangeResult();
  
  if(status == VL6180X_NO_ERR){
    return range;
  }
  return 0;
}
