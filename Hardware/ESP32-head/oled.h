#ifndef OLED_H
#define OLED_H

#define OLED_WIDTH        128
#define OLED_HEIGHT       64
#define OLED_ROW_COUNT    (OLED_HEIGHT/8)
#define OLED_BUFFER_SIZE  (OLED_WIDTH * OLED_ROW_COUNT)
#define OLED_MAX_STR_LEN  (OLED_BUFFER_SIZE/8)

bool OLED_init(void);
void OLED_clear(void);
void OLED_displayOn(void);
void OLED_displayOff(void);
void OLED_print(char* string);
void OLED_debug(char* msg);
uint8_t* OLED_getBuffer(void);

#endif
