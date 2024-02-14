#ifndef OLED_H
#define OLED_H

bool OLED_init(void);
void OLED_clear(void);
void OLED_displayOn(void);
void OLED_displayOff(void);
void OLED_small_eyes(void);
void OLED_print(char* string, uint8_t row, uint8_t column);
void OLED_debug(char* msg);

#endif
