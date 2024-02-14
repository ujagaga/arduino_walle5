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
  if(cmd > UCMD_NONE){ 
    uint32_t buf_len = (uint32_t)len;
    uint8_t header[7] = {UCMD_START_0, UCMD_START_1, (uint8_t)cmd, (uint8_t)len, (uint8_t)(len >> 8), (uint8_t)(len >> 16), (uint8_t)(len >> 24)};  
    
    Serial.write(header, 7);
    if(len){
      Serial.write(buf, len);
    }    
  }  
}
