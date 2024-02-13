#include <DFRobot_VL6180X.h>
#include "tof_range.h"
#include "config.h"

#define TOF_1_IIC_ADDRESS     0x29

DFRobot_VL6180X VL6180X(TOF_1_IIC_ADDRESS, &I2CWire);

static bool init_ok;

void TOF_init(){
  init_ok = VL6180X.begin();
  if(init_ok){
    Serial.println("TOF range sensor OK.");
    VL6180X.rangeSetInterMeasurementPeriod(30);    
  }else{
    Serial.println("ERROR setting up TOF range sensor.");
  }
}

uint8_t TOF_get_range(){
  if(init_ok){
    uint8_t range = VL6180X.rangePollMeasurement();
    uint8_t status = VL6180X.getRangeResult();
    
    if(status == VL6180X_NO_ERR){
      return range;
    }
  }
  return 0;
}

