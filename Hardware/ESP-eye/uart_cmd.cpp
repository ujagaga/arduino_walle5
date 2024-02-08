#include <Arduino.h>
#include "uart_cmd.h"

int UART_checkCmd(){
  if (Serial.available() > 2) {
    if(Serial.read() == UCMD_START_0){      
      if(Serial.read() == UCMD_START_1){
        return Serial.read();
      }
    }
  }
  return UCMD_NONE;
}

void UART_sendResponse(int cmd, uint8_t* buf, size_t len){  
  Serial.write(UCMD_START_0);
  Serial.write(UCMD_START_1);  
  Serial.write(cmd);
  if(cmd > 0){
    Serial.write((uint8_t*)&len, 4);
    Serial.write(buf, len);
  }  
}
