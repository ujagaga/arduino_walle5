#include "camera_ctrl.h"
#include "uart_cmd.h"
#include "IR_ctrl.h"
#include "i2c_devs.h"
#include "tof_range.h"
#include "gyro.h"
#include "oled.h"


void setup() {  
  Serial.begin(1000000);  
  
  CAM_init();

  I2C_devs_init();
  TOF_init();
  GYRO_init();
  OLED_init();
  OLED_small_eyes();

  pinMode(LED_GPIO_NUM, OUTPUT);
  digitalWrite(LED_GPIO_NUM, LOW);

  pinMode(IR_LED_GPIO_NUM, OUTPUT);
  digitalWrite(IR_LED_GPIO_NUM, LOW);  
}

void loop() {  
  int cmd = UART_checkCmd();
  uint8_t response = 0;
  
  switch(cmd){
    case UCMD_FLASH_ON:
      {
        digitalWrite(LED_GPIO_NUM, HIGH);      
        UART_sendResponse(UCMD_FLASH_ON, &response, 1);
      }
      break;

    case UCMD_FLASH_OFF:
      {
        digitalWrite(LED_GPIO_NUM, LOW);
        UART_sendResponse(UCMD_FLASH_OFF, &response, 1);
      }
      break;

    case UCMD_IR_ON:
      {
        pinMode(IR_LED_GPIO_NUM, OUTPUT);
        digitalWrite(LED_GPIO_NUM, HIGH);
        UART_sendResponse(UCMD_IR_ON, &response, 1);
      }
      break;

    case UCMD_IR_OFF:
      pinMode(IR_LED_GPIO_NUM, OUTPUT);
      digitalWrite(LED_GPIO_NUM, LOW);
      UART_sendResponse(UCMD_IR_OFF, &response, 1);
      break;

    case UCMD_TV:
      {
        IRCTRL_send(IR_TV_ON_OFF);
        UART_sendResponse(UCMD_TV, &response, 1);
      }
      break; 

    case UCMD_CAM_SNAPSHOT:
      {
        CAM_capture();
      }
      break; 

    case UCMD_GET_RANGE_FRONT:
      {
        uint8_t range = TOF_get_range();
        UART_sendResponse(UCMD_GET_RANGE_FRONT, &range, 1);
      }
      break;

    case UCMD_GET_ROTATION:
      {
        int16_t xyz_buffer[3] = {0};
        GYRO_get_rotation(xyz_buffer);
        UART_sendResponse(UCMD_GET_ROTATION, (uint8_t*)&xyz_buffer, 6);
      }
      break;

    default:
      {
        response = 1;
        UART_sendResponse(cmd, &response, 1);
      }
      break;
  }

}
