#include "camera_ctrl.h"
#include "uart_cmd.h"
#include "IR_ctrl.h"
#include "tof_range.h"
#include "gyro.h"
#include "oled.h"
#include "config.h"

TwoWire I2CWire = TwoWire(0);


void I2C_devs_init(){
  /* Arduino Wire library for esp32 has 128 byte buffer. We need at least 130 for OLED row 
  or we would need to make the display code more complicated to take buffer size in consideration. */
  I2CWire.setBufferSize(256);
  I2CWire.begin(I2C_SDA, I2C_SCL, 400000);    
}

void setup() {  
  Serial.begin(1000000);

  char err_log[] = {'E', ':', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 0};  
  bool error_flag = false;

  if(!CAM_init()){
    err_log[2] = 'C';
    error_flag = true;
  }
  I2C_devs_init();
  if(!TOF_init()){
    err_log[3] = 'T';
    error_flag = true;
  }
  if(!GYRO_init()){
    err_log[4] = 'G';
    error_flag = true;
  }
  if(OLED_init()){   
    if(error_flag){
      OLED_debug(err_log);
    }    
  }

  pinMode(LED_GPIO_NUM, OUTPUT);
  digitalWrite(LED_GPIO_NUM, LOW);

  pinMode(IR_LED_GPIO_NUM, OUTPUT);
  digitalWrite(IR_LED_GPIO_NUM, LOW);  
}

void loop() {  
  UART_process();

  uint8_t cmd = UART_checkCmd();
  
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

    case UCMD_PRINT_TEXT:
      {
        char msg[OLED_MAX_STR_LEN] = {0};
        if(UART_copyMgs(msg, OLED_MAX_STR_LEN, true)){
          OLED_displayOff();
          OLED_print(msg);  
          OLED_displayOn();
          UART_sendResponse(cmd, &response, 1);
        }else{
          response = 1;
          UART_sendResponse(cmd, &response, 1);
        }
      }
      break;

    case UCMD_PRINT_GRAPHIC:
      {
        uint8_t* oled_buffer = OLED_getBuffer();   
        if(UART_copyMgs(oled_buffer, OLED_BUFFER_SIZE, false)){
          OLED_displayOff();
          OLED_display();  
          OLED_displayOn();
          UART_sendResponse(cmd, &response, 1);
        }else{
          response = 1;
          UART_sendResponse(cmd, &response, 1);
        }
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
