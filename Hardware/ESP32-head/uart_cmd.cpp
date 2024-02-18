#include <Arduino.h>
#include "uart_cmd.h"

#define BUF_LEN       2048
#define RX_TIMEOUT    200
#define BUF_MASK      (BUF_LEN-1)

static uint8_t buffer[BUF_LEN];
static uint16_t rd_idx = 0;
static uint16_t wr_idx = 0;
static uint32_t last_rx_time = 0;

static void increment_rdx(){
  rd_idx++;
  rd_idx &= BUF_MASK;
} 

static void increment_wrx(){
  rd_idx++;
  rd_idx &= BUF_MASK;
  last_rx_time = millis();
} 

uint16_t UART_getRxLen(){
  return ((wr_idx - rd_idx) & BUF_MASK);
}


/** @brief Checks serial port for incomming data and populates the circular buffer.
 *  If no data was received in RX_TIMEOUT ms, the buffer is cleared to cleanup failed messages.
 */
void UART_process(){
  while(Serial.available()){
    if(UART_getRxLen() < (BUF_LEN-1)){
      buffer[wr_idx] = Serial.read();
      increment_wxr();      
    }
  }

  if((millis() - last_rx_time) > RX_TIMEOUT){
    rd_idx = wr_idx;
  }
}

/** @brief Copies received data to a specified output buffer.
 *  @param uint8_t* outBuf: Output buffer to copy data to.
 *  @param int len: Length of data to copy.
 *  @return bool: true if OK, false is not enough data is received.
*/
bool UART_copyMgs(uint8_t* outBuf, uint16_t len, bool is_ascii){

  for(uint16_t i = 0; i < len; ++i){
    if(rd_idx == wr_idx){
      return false;
    }
    outBuf[i] = buffer[rd_idx];
    increment_rdx();
    if(is_ascii && (outBuf[i] == 0)){
      return true;
    }    
  } 
  
  return true;
}

/** @brief Checks the circular buffer for a valid command.
 *  @return received command if any, 0 if none.
 */
uint8_t UART_checkCmd(){
  if(UART_getRxLen() > 2){
    if((buffer[rd_idx] == UCMD_START_0) && (buffer[rd_idx+1] == UCMD_START_1)){      
      uint8_t cmd = buffer[rd_idx + 2];
      rd_idx += 3;
      rd_idx &= BUF_MASK;
      return cmd;
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


