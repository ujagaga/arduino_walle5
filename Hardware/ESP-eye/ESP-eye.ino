#include "camera_ctrl.h"
#include "uart_cmd.h"
#include "IR_ctrl.h"


void setup() {  
  Serial.begin(1000000);  
  
  CAM_init();

  pinMode(LED_GPIO_NUM, OUTPUT);
  digitalWrite(LED_GPIO_NUM, LOW);

  pinMode(IR_LED_GPIO_NUM, OUTPUT);
  digitalWrite(IR_LED_GPIO_NUM, LOW);  
}

void loop() {  
  int cmd = UART_checkCmd();
  switch(cmd){
    case UCMD_FLASH_ON:
      digitalWrite(LED_GPIO_NUM, HIGH);
      break;

    case UCMD_FLASH_OFF:
      digitalWrite(LED_GPIO_NUM, LOW);
      break;

    case UCMD_IR_ON:
      pinMode(IR_LED_GPIO_NUM, OUTPUT);
      digitalWrite(LED_GPIO_NUM, HIGH);
      break;

    case UCMD_IR_OFF:
      pinMode(IR_LED_GPIO_NUM, OUTPUT);
      digitalWrite(LED_GPIO_NUM, LOW);
      break;

    case UCMD_TV:
      IRCTRL_send(IR_TV_ON_OFF);
      break; 

    case UCMD_CAM_SNAPSHOT:
      CAM_capture();
      break; 

    default:
      break;
  }

}
