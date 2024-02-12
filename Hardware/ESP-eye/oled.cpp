#include <Arduino.h>
#include "oled.h"

/* Using SH1106 OLED display at 0x3C*/

#define OLED_address      0x3c
#define OLED_WIDTH        128
#define OLED_HEIGHT       64
#define OLED_ROW_COUNT    8
#define OLED_BUFFER_SIZE (OLED_WIDTH * OLED_ROW_COUNT)

#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
#define SSD1306_SETCONTRAST 0x81

#define WIRE_MAX          32

int16_t window_x1, ///< Dirty tracking window minimum x
      window_y1,     ///< Dirty tracking window minimum y
      window_x2,     ///< Dirty tracking window maximum x
      window_y2;     ///< Dirty tracking window maximum y
uint8_t _page_start_offset = 0;

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

static void sendData(unsigned char* data, uint8_t data_len) 
{
  
  I2CWire.beginTransmission(OLED_address); // begin transmitting
  I2CWire.write(0x40);//data mode
  I2CWire.write(data, data_len);
  I2CWire.endTransmission();    // stop transmitting
}

static void setXY(uint8_t row, uint16_t col)
{
  
  sendcommand(0xb0+row);                //set page address
  sendcommand(8*col&0x0f);       //set low col address
  sendcommand(0x10+((8*col>>4)&0x0f));  //set high col address
  // sendcommand(0x21);      // Set column command
  // sendcommand(0);         // Start column
  // sendcommand(OLED_WIDTH);// End column

  // sendcommand(0x22);            // Set page command
  // sendcommand(row);             // Start page
  // sendcommand(OLED_ROW_COUNT-1);// End page
}

static void writePixel(int16_t x, int16_t y, uint16_t color) {
  // uint16_t idx = x*OLED_ROW_COUNT + (y / 8);
  // uint8_t bit = y % 8;

  // if(c) {
  //   buffer[idx] |= (1 << bit);
  // } else {
  //   buffer[idx] &= ~(1 << bit);
  // }
  if ((x >= 0) && (x < OLED_WIDTH) && (y >= 0) && (y < OLED_HEIGHT)) { 
    // adjust dirty window
    window_x1 = min(window_x1, x);
    window_y1 = min(window_y1, y);
    window_x2 = max(window_x2, x);
    window_y2 = max(window_y2, y);

    if(color) {
      buffer[x + (y / 8) * OLED_WIDTH] |= (1 << (y & 7));
    }else{
      buffer[x + (y / 8) * OLED_WIDTH] &= ~(1 << (y & 7));
    } 
  }
}

// static void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint8_t color) {

//   int16_t steep = abs(y1 - y0) > abs(x1 - x0);
//   if (steep) {
//     _swap_int16_t(x0, y0);
//     _swap_int16_t(x1, y1);
//   }

//   if (x0 > x1) {
//     _swap_int16_t(x0, x1);
//     _swap_int16_t(y0, y1);
//   }

//   int16_t dx, dy;
//   dx = x1 - x0;
//   dy = abs(y1 - y0);

//   int16_t err = dx / 2;
//   int16_t ystep;

//   if (y0 < y1) {
//     ystep = 1;
//   } else {
//     ystep = -1;
//   }

//   for (; x0 <= x1; x0++) {
//     if (steep) {
//       writePixel(y0, x0, color);
//     } else {
//       writePixel(x0, y0, color);
//     }
//     err -= dy;
//     if (err < 0) {
//       y0 += ystep;
//       err += dx;
//     }
//   }
// }

// static void writeFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color) { 
//   writeLine(x, y, x, y + h - 1, color);
// }

// static void writeFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color) {  
//   writeLine(x, y, x + w - 1, y, color);
// }

// static void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
//   for (int16_t i = x; i < x + w; i++) {
//     writeFastVLine(i, y, h, color);
//   }
// }

// static void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color) {
//   // Update in subclasses if desired!
//   if (x0 == x1) {
//     if (y0 > y1)
//       _swap_int16_t(y0, y1);      
//     writeFastVLine(x0, y0, y1 - y0 + 1, color);
//   } else if (y0 == y1) {
//     if (x0 > x1)
//       _swap_int16_t(x0, x1);
//     writeFastHLine(x0, y0, x1 - x0 + 1, color);
//   } else {
//     writeLine(x0, y0, x1, y1, color);
//   }
// }

// static void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) {
//   int16_t f = 1 - r;
//   int16_t ddF_x = 1;
//   int16_t ddF_y = -2 * r;
//   int16_t x = 0;
//   int16_t y = r;

//   writePixel(x0, y0 + r, color);
//   writePixel(x0, y0 - r, color);
//   writePixel(x0 + r, y0, color);
//   writePixel(x0 - r, y0, color);

//   while (x < y) {
//     if (f >= 0) {
//       y--;
//       ddF_y += 2;
//       f += ddF_y;
//     }
//     x++;
//     ddF_x += 2;
//     f += ddF_x;

//     writePixel(x0 + x, y0 + y, color);
//     writePixel(x0 - x, y0 + y, color);
//     writePixel(x0 + x, y0 - y, color);
//     writePixel(x0 - x, y0 - y, color);
//     writePixel(x0 + y, y0 + x, color);
//     writePixel(x0 - y, y0 + x, color);
//     writePixel(x0 + y, y0 - x, color);
//     writePixel(x0 - y, y0 - x, color);
//   }
// }

// static void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint8_t color) {

//   int16_t f = 1 - r;
//   int16_t ddF_x = 1;
//   int16_t ddF_y = -2 * r;
//   int16_t x = 0;
//   int16_t y = r;
//   int16_t px = x;
//   int16_t py = y;

//   delta++; // Avoid some +1's in the loop

//   while (x < y) {
//     if (f >= 0) {
//       y--;
//       ddF_y += 2;
//       f += ddF_y;
//     }
//     x++;
//     ddF_x += 2;
//     f += ddF_x;
//     // These checks avoid double-drawing certain lines, important
//     // for the SSD1306 library which has an INVERT drawing mode.
//     if (x < (y + 1)) {
//       if (corners & 1)
//         writeFastVLine(x0 + x, y0 - y, 2 * y + delta, color);
//       if (corners & 2)
//         writeFastVLine(x0 - x, y0 - y, 2 * y + delta, color);
//     }
//     if (y != py) {
//       if (corners & 1)
//         writeFastVLine(x0 + py, y0 - px, 2 * px + delta, color);
//       if (corners & 2)
//         writeFastVLine(x0 - py, y0 - px, 2 * px + delta, color);
//       py = y;
//     }
//     px = x;
//   }
// }

// static void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color) {
//   writeFastVLine(x0, y0 - r, 2 * r + 1, color);
//   fillCircleHelper(x0, y0, r, 3, 0, color);
// }

// static void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color) {
//   int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
//   if (r > max_radius)
//     r = max_radius;

//   fillRect(x + r, y, w - 2 * r, h, color);
//   // draw four corners
//   fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
//   fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
// }


void OLED_display() { 
  int row, x, i;
  for(row = 0; row < OLED_ROW_COUNT; ++row)
  {	
    setXY(row,0);    
    {
      for(x=0;x<128; x+=16)  
      {
          sendData(buffer[row*x + x + i*16], 16);
      }
    }
  }

  // yield();
  // uint8_t *ptr = buffer;
  // uint8_t dc_byte = 0x40;
  // uint8_t pages = OLED_HEIGHT / 8;
  // uint8_t bytes_per_page = OLED_WIDTH;

  // uint8_t first_page = window_y1 / 8;
  // uint8_t page_start = min(bytes_per_page, (uint8_t)window_x1);
  // uint8_t page_end = (uint8_t)max((int)0, (int)window_x2);

  // for (uint8_t p = first_page; p < pages; p++) {
  //   uint8_t bytes_remaining = bytes_per_page;
  //   ptr = buffer + (uint16_t)p * (uint16_t)bytes_per_page;
  //   // fast forward to dirty rectangle beginning
  //   ptr += page_start;
  //   bytes_remaining -= page_start;
  //   // cut off end of dirty rectangle
  //   bytes_remaining -= (OLED_WIDTH - 1) - page_end;

  //   uint16_t maxbuff = 16;

  //   uint8_t cmd[] = {
  //       0x00, (uint8_t)(SH110X_SETPAGEADDR + p),
  //       (uint8_t)(0x10 + ((page_start + _page_start_offset) >> 4)),
  //       (uint8_t)((page_start + _page_start_offset) & 0xF)};

  //   I2CWire.write(cmd, 4);

  //   while (bytes_remaining) {
  //     uint8_t to_write = min(bytes_remaining, (uint8_t)maxbuff);
  //     I2CWire.write(ptr, to_write);
  //     ptr += to_write;
  //     bytes_remaining -= to_write;
  //   }
  // }
  // // reset dirty window
  // window_x1 = 1024;
  // window_y1 = 1024;
  // window_x2 = -1;
  // window_y2 = -1;
  // yield();
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
  window_x1 = 0;
  window_y1 = 0;
  window_x2 = OLED_WIDTH - 1;
  window_y2 = OLED_HEIGHT - 1;
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
    sendcommand(0xd5);
    sendcommand(0x80);
    sendcommand(0xa8);
    sendcommand(0x3f);
    sendcommand(0xd3);
    sendcommand(0x00);
    sendcommand(0x40);  
    sendcommand(0xad);
    sendcommand(0x8b);
    sendcommand(0xA1);
    sendcommand(0xC8);
    sendcommand(0xDA);
    sendcommand(0x12);
    sendcommand(0x81);
    sendcommand(0xff);
    sendcommand(0xD9);
    sendcommand(0x1F);
    sendcommand(0xDB);
    sendcommand(0x40);
    sendcommand(0x33);
    sendcommand(0xA6);
    sendcommand(0x20);
    sendcommand(0x10);
    sendcommand(0xa4);
  }  

  delay(100); 

  OLED_displayOn();

  if (init_ok){
    Serial.println("OLED display setup done.");
  }else{    
    Serial.println("ERROR setting up OLED display.");
  } 
 
}

void OLED_small_eyes(){
  memset(buffer, 0, OLED_BUFFER_SIZE); 
  OLED_displayOff(); 

  
  // setXY(0, 0);  
  // for(uint8_t idx = 0; idx < 128; ++idx){
  //   sendData(127-idx);
  // }

  OLED_display();
  OLED_displayOn();
}