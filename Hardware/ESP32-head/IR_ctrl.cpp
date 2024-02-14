#include "camera_ctrl.h"
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "IR_ctrl.h"

uint16_t rawData_TV_on_off[21] = {890, 874,  1776, 876,  892, 876,  890, 878,  890, 876,  894, 1758,  1774, 878,  894, 1754,  894, 876,  1770, 878,  840};


void IRCTRL_send(int cmd){
  if(cmd == IR_TV_ON_OFF){
    IRsend irsend(IR_LED_GPIO_NUM);
    irsend.begin();
    irsend.sendRaw(rawData_TV_on_off, 21, 38); // Send a raw data capture at 38kHz.
    delay(1000);
    irsend.sendRaw(rawData_TV_on_off, 21, 38); 
  }
}
