#include <Arduino.h>
#include "oled.h"

#define OLED_address      0x3c
#define OLED_WIDTH        128
#define OLED_HEIGHT       64
#define OLED_PAGE_COUNT   8
#define OLED_BUFFER_SIZE (OLED_WIDTH * OLED_PAGE_COUNT)

#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_SETCONTRAST 0x81

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif

uint8_t buffer[OLED_BUFFER_SIZE] = {0};
uint8_t cursor_x = 0;
uint8_t cursor_y = 0;
static bool init_ok = true;
 

static void sendcommand(unsigned char com)
{
  I2CWire.beginTransmission(OLED_address);     
  I2CWire.write(0x80);                          //command mode
  I2CWire.write(com);
  uint8_t error = I2CWire.endTransmission();    
  if (error != 0){
    init_ok = false;
  }
}

static void sendData(unsigned char data) 
{
  
  I2CWire.beginTransmission(OLED_address); // begin transmitting
  I2CWire.write(0x40);//data mode
  I2CWire.write(data);
  I2CWire.endTransmission();    // stop transmitting
}

static void setXY(uint8_t row, uint16_t col)
{
  sendcommand(0xb0+row);                //set page address
  sendcommand(8*col&0x0f);       //set low col address
  sendcommand(0x10+((8*col>>4)&0x0f));  //set high col address
}

static void writePixel(int16_t x, int16_t y, uint16_t c) {
  uint16_t idx = x + (y / 8) * OLED_WIDTH;
  uint8_t bit = y % 8;

  if(c) {
    buffer[idx] |= (1 << bit);
  } else {
    buffer[idx] &= ~(1 << bit);
  }
}

static void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint8_t color) {

  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1) {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      writePixel(y0, x0, color);
    } else {
      writePixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

static void writeFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color) { 
  writeLine(x, y, x, y + h - 1, color);
}

static void writeFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color) {  
  writeLine(x, y, x + w - 1, y, color);
}

static void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
  for (int16_t i = x; i < x + w; i++) {
    writeFastVLine(i, y, h, color);
  }
}

static void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color) {
  // Update in subclasses if desired!
  if (x0 == x1) {
    if (y0 > y1)
      _swap_int16_t(y0, y1);      
    writeFastVLine(x0, y0, y1 - y0 + 1, color);
  } else if (y0 == y1) {
    if (x0 > x1)
      _swap_int16_t(x0, x1);
    writeFastHLine(x0, y0, x1 - x0 + 1, color);
  } else {
    writeLine(x0, y0, x1, y1, color);
  }
}

static void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  writePixel(x0, y0 + r, color);
  writePixel(x0, y0 - r, color);
  writePixel(x0 + r, y0, color);
  writePixel(x0 - r, y0, color);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    writePixel(x0 + x, y0 + y, color);
    writePixel(x0 - x, y0 + y, color);
    writePixel(x0 + x, y0 - y, color);
    writePixel(x0 - x, y0 - y, color);
    writePixel(x0 + y, y0 + x, color);
    writePixel(x0 - y, y0 + x, color);
    writePixel(x0 + y, y0 - x, color);
    writePixel(x0 - y, y0 - x, color);
  }
}

static void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint8_t color) {

  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  int16_t px = x;
  int16_t py = y;

  delta++; // Avoid some +1's in the loop

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // These checks avoid double-drawing certain lines, important
    // for the SSD1306 library which has an INVERT drawing mode.
    if (x < (y + 1)) {
      if (corners & 1)
        writeFastVLine(x0 + x, y0 - y, 2 * y + delta, color);
      if (corners & 2)
        writeFastVLine(x0 - x, y0 - y, 2 * y + delta, color);
    }
    if (y != py) {
      if (corners & 1)
        writeFastVLine(x0 + py, y0 - px, 2 * px + delta, color);
      if (corners & 2)
        writeFastVLine(x0 - py, y0 - px, 2 * px + delta, color);
      py = y;
    }
    px = x;
  }
}

static void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) {
  writeFastVLine(x0, y0 - r, 2 * r + 1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

static void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color) {
  int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
  if (r > max_radius)
    r = max_radius;

  fillRect(x + r, y, w - 2 * r, h, color);
  // draw four corners
  fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
  fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}


void OLED_display() { 
  unsigned char i,k;
  for(k=0;k<OLED_PAGE_COUNT;k++)
  {	
    setXY(k,0);    
    {
      for(i=0;i<128;i++)  
      {
        sendData(buffer[k*i + i]);
      }
    }
  }
}

//==========================================================//
// Resets display depending on the actual mode.
void OLED_resetDisplay(void)
{
  OLED_displayOff();
  OLED_clearDisplay();
  OLED_display();
  OLED_displayOn();
}

//==========================================================//
// Turns display on.
void OLED_displayOn(void)
{
  sendcommand(0xaf);        //display on
}

//==========================================================//
// Turns display off.
void OLED_displayOff(void)
{
  sendcommand(0xae);    //display off
}

//==========================================================//
// Clears the display by sendind 0 to all the screen map.
void OLED_clearDisplay(void)
{
  memset(buffer, 0, OLED_BUFFER_SIZE);
}

void OLED_FillDisplay(void)
{
  memset(buffer, 0xff, OLED_BUFFER_SIZE);
}

//==========================================================//
// Inits oled and draws logo at startup
void OLED_init(void)
{
  memset(buffer, 0, OLED_BUFFER_SIZE); 
  
  sendcommand(0xae);		//display off
  if (init_ok){
    sendcommand(0xa6);            //Set Normal Display (default)

    sendcommand(0xAE);             //DISPLAYOFF
    sendcommand(0xD5);            //SETDISPLAYCLOCKDIV
    sendcommand(0x80);            // the suggested ratio 0x80
    sendcommand(0xA8);            //SSD1306_SETMULTIPLEX
    sendcommand(0x3F);
    sendcommand(0xD3);            //SETDISPLAYOFFSET
    sendcommand(0x0);             //no offset
    sendcommand(0x40 | 0x0);      //SETSTARTLINE
    sendcommand(0x8D);            //CHARGEPUMP
    sendcommand(0x14);
    sendcommand(0x20);             //MEMORYMODE
    sendcommand(0x00);             //0x0 act like ks0108
    
    //sendcommand(0xA0 | 0x1);      //SEGREMAP   //Rotate screen 180 deg
    sendcommand(0xA0);
    
    //sendcommand(0xC8);            //COMSCANDEC  Rotate screen 180 Deg
    sendcommand(0xC0);
    
    sendcommand(0xDA);            //0xDA
    sendcommand(0x12);           //COMSCANDEC
    sendcommand(0x81);           //SETCONTRAS
    sendcommand(0xCF);           //
    sendcommand(0xd9);          //SETPRECHARGE 
    sendcommand(0xF1); 
    sendcommand(0xDB);        //SETVCOMDETECT                
    sendcommand(0x40);
    sendcommand(0xA4);        //DISPLAYALLON_RESUME        
    sendcommand(0xA6);        //NORMALDISPLAY             

    sendcommand(0x2e);            // stop scroll
    sendcommand(0xa0);		//seg re-map 0->127(default)
    sendcommand(0xa1);		//seg re-map 127->0
    sendcommand(0xc8);
    // delay(1000);
    sendcommand(0x20);            //Set Memory Addressing Mode
    sendcommand(0x00);            //Set Memory Addressing Mode ab Horizontal addressing mode
    OLED_display();
    OLED_displayOn();
  }

  if (init_ok){
    Serial.println("OLED display setup done.");
  }else{    
    Serial.println("ERROR setting up OLED display.");
  } 
}

void OLED_small_eyes(){
  memset(buffer, 0, OLED_BUFFER_SIZE); 
  OLED_displayOff(); 
  // fillRoundRect(5, 28, 48, 8, 4, 1);
  // writeLine(0, 0, 127, 63, 1);
  for(int i = 0; i < 127; ++i){
    writePixel(i, 32, 1);
  }
  OLED_display();
  OLED_displayOn();
}