#include "MPU6050_light.h"
#include "gyro.h"
#include "config.h"

MPU6050 mpu(I2CWire);
static bool init_ok = false;

void GYRO_init(){   
  if(mpu.begin() == 0){
    init_ok = true;
    Serial.println("Calculating GYRO offsets, please do not move.");
    delay(1000);
    mpu.calcOffsets(); 
    Serial.println("GYRO setup done.");
  }else{
    Serial.println("ERROR setting up GYRO sensor.");
  }    
}


void GYRO_get_rotation(int16_t* xyz_buffer){
  if(init_ok){
    mpu.update();

    float x = mpu.getAngleX();
    xyz_buffer[0] = (int16_t)(x * 100);
    float y = mpu.getAngleY();
    xyz_buffer[1] = (int16_t)(y * 100);
    float z = mpu.getAngleZ();
    xyz_buffer[2] = (int16_t)(z * 100);
  }
}



