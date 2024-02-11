#ifndef OLED_H
#define OLED_H

#include "i2c_devs.h"

void OLED_drawPixel(int16_t, int16_t, uint16_t);
void OLED_display(void);
void OLED_resetDisplay(void);
void OLED_displayOn(void);
void OLED_displayOff(void);
void OLED_clearDisplay(void);
void OLED_init(void);
void OLED_small_eyes(void);
#endif
