#include <DFRobot_VL6180X.h>
#include <MPU6050_light.h>
#include <Wire.h>
#include "i2c_devs.h"

TwoWire I2C = TwoWire(0);
DFRobot_VL6180X VL6180X(TOF_1_IIC_ADDRESS, &I2C);
MPU6050 mpu(I2C);


void I2C_devs_init(){
  I2C.begin(I2C_SDA, I2C_SCL, 400000);  

  if(VL6180X.begin()){
    Serial.println("TOF range sensor OK.");
    VL6180X.rangeSetInterMeasurementPeriod(30);    
  }else{
    Serial.println("ERROR setting up TOF range sensor.");
  }
    
  if(mpu.begin() == 0){
    Serial.println("Calculating GYRO offsets, please do not move.");
    delay(1000);
    mpu.calcOffsets(); 
    Serial.println("GYRO setup done.");
  }else{
    Serial.println("ERROR setting up GYRO sensor.");
  }  
  
}

uint8_t TOF_get_range(){
  uint8_t range = VL6180X.rangePollMeasurement();
  uint8_t status = VL6180X.getRangeResult();
  
  if(status == VL6180X_NO_ERR){
    return range;
  }
  return 0;
}

void GYRO_get_rotation(int16_t* xyz_buffer){
  mpu.update();

  float x = mpu.getAngleX();
  xyz_buffer[0] = (int16_t)(x * 100);
  float y = mpu.getAngleY();
  xyz_buffer[1] = (int16_t)(y * 100);
  float z = mpu.getAngleZ();
  xyz_buffer[2] = (int16_t)(z * 100);
}



