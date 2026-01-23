// Display Library for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: these e-papers require 3.3V supply AND data lines!
//
// based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
// Controller: IL0373 : http://www.e-paper-display.com/download_detail/downloadsId=535.html
//
// Author: Jean-Marc Zingg
// Optimized version
//
// Library: https://github.com/ZinggJM/GxEPD2

#include "GxEPD2_350c.h"

static const uint8_t lut_bwr_350_part[] = {
0x01, 0x10, 0x18, 0x01, 0x32, 0x02, 0x01, 0x10, 0x08, 0x01, 0x03, 0x02,
0x01, 0x01, 0x01, 0x0A, 0x05, 0x0A, 0x0F, 0x01, 0x01, 0x01, 0x14, 0x0F,
0x02, 0x02, 0x01, 0x14, 0x01, 0x02, 0x02, 0x04, 0x02, 0x14, 0x12, 0x01,
0x0A, 0x0F, 0x01, 0x02, 0x01, 0x01, 0x07, 0x05, 0x00, 0x22, 0x02, 0x01,
0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x50, 0x18, 0x01,
0x72, 0x02, 0x01, 0x10, 0x88, 0x01, 0x43,
0x02, 0x01, 0x01, 0x01, 0x4A, 0x85, 0x4A,
0x8F, 0x01, 0x01, 0x01, 0x94, 0x8F, 0x82,
0x42, 0x01, 0x14, 0x01, 0x82, 0x42, 0x84,
0x42, 0x14, 0x12, 0x01, 0x4A, 0x4F, 0x01,
0x02, 0x01, 0x01, 0x07, 0x85, 0x00, 0xE2,
0x02, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x50, 0x58, 0x81,
0x72, 0x02, 0x01, 0x10, 0x88, 0x01, 0x43,
0x42, 0x01, 0x01, 0x01, 0x4A, 0x85, 0x4A,
0x8F, 0x01, 0x01, 0x01, 0x94, 0x8F, 0x82,
0x42, 0x01, 0x14, 0x01, 0x82, 0x42, 0x84,
0x42, 0x14, 0x12, 0x01, 0x4A, 0x4F, 0x01,
0x02, 0x01, 0x01, 0x07, 0x85, 0x00, 0x22,
0x02, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x50, 0x58, 0x01,
0x72, 0x02, 0x01, 0x10, 0x88, 0x81, 0x43,
0x02, 0x01, 0x01, 0x01, 0x4A, 0x85, 0x4A,
0x8F, 0x01, 0x01, 0x01, 0x94, 0x8F, 0x82,
0x42, 0x01, 0x14, 0x01, 0x82, 0x42, 0x84,
0x42, 0x14, 0x12, 0x01, 0x4A, 0x4F, 0x01,
0x82, 0x01, 0x01, 0x07, 0x05, 0x00, 0x22,
0x42, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01,
0x02, 0x00, 0x00,
0x22, 0x17, 0x41, 0xA8, 0x32, 0x50
};

GxEPD2_350c::GxEPD2_350c(int16_t cs, int16_t dc, int16_t rst, int16_t busy) :
  GxEPD2_EPD(cs, dc, rst, busy, LOW, 20000000, WIDTH, HEIGHT, panel, hasColor, hasPartialUpdate, hasFastPartialUpdate)
{
}

void GxEPD2_350c::clearScreen(uint8_t value)
{
  clearScreen(value, 0xFF);
}

void GxEPD2_350c::clearScreen(uint8_t black_value, uint8_t color_value)
{
  _initial_write = false; // initial full screen buffer clean done
  _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  
  // Optimization: Pre-calculate total bytes to avoid repeated calc in loop
  uint32_t total_bytes = uint32_t(WIDTH) * uint32_t(HEIGHT) / 8;

  _writeCommand(0x10);
  for (uint32_t i = 0; i < total_bytes; i++)
  {
    _writeData(black_value);
  }
  
  _writeCommand(0x13);
  for (uint32_t i = 0; i < total_bytes; i++)
  {
    _writeData(color_value);
  }
  
  _Update_Part();
  _writeCommand(0x92); // partial out
}

void GxEPD2_350c::writeScreenBuffer(uint8_t value)
{
  writeScreenBuffer(value, 0xFF);
}

void GxEPD2_350c::writeScreenBuffer(uint8_t black_value, uint8_t color_value)
{
  _initial_write = false; // initial full screen buffer clean done
  _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(0, 0, WIDTH, HEIGHT);
  
  uint32_t total_bytes = uint32_t(WIDTH) * uint32_t(HEIGHT) / 8;

  _writeCommand(0x10);
  for (uint32_t i = 0; i < total_bytes; i++)
  {
    _writeData(black_value);
  }
  _writeCommand(0x13);
  for (uint32_t i = 0; i < total_bytes; i++)
  {
    _writeData(color_value);
  }
  _writeCommand(0x92); // partial out
}

void GxEPD2_350c::writeImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, NULL, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_350c::writeImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImage(0x10, black, x, y, w, h, invert, mirror_y, pgm);
  _writeImage(0x13, color, x, y, w, h, invert, mirror_y, pgm);
}

// Optimization: Unified logic and better loop arithmetic
void GxEPD2_350c::_writeImage(uint8_t command, const uint8_t* bitmap, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
  
  // Optimization: use yield() instead of delay(1) for faster return on ESP
#if defined(ESP8266) || defined(ESP32)
  yield(); 
#else
  delay(1);
#endif

  int16_t wb = (w + 7) / 8; // width bytes, bitmaps are padded
  x -= x % 8; // byte boundary
  w = wb * 8; // byte boundary
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;

  _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(command);

  // Optimization: Handle NULL bitmap separately to avoid check inside loop
  if (!bitmap) 
  {
      uint8_t data = invert ? 0x00 : 0xFF; // If no bitmap, assume white (0xFF), invert becomes black (0x00)
      for (int16_t i = 0; i < h1; i++) {
        for (int16_t j = 0; j < w1 / 8; j++) {
           _writeData(data);
        }
      }
  } 
  else 
  {
      // Optimization: Hoist Y-axis calc and mirror logic out of inner loop
      for (int16_t i = 0; i < h1; i++)
      {
        int32_t row_offset; // Use int32 to prevent overflow on large images
        if (mirror_y) {
           row_offset = (int32_t)(h - 1 - (i + dy)) * wb + dx / 8;
        } else {
           row_offset = (int32_t)(i + dy) * wb + dx / 8;
        }

        for (int16_t j = 0; j < w1 / 8; j++)
        {
          uint8_t data;
          int32_t idx = row_offset + j; // Simple addition inside the tight loop

          if (pgm)
          {
  #if defined(__AVR) || defined(ESP8266) || defined(ESP32)
            data = pgm_read_byte(&bitmap[idx]);
  #else
            data = bitmap[idx];
  #endif
          }
          else
          {
            data = bitmap[idx];
          }
          if (invert) data = ~data;
          _writeData(data);
        }
      }
  }

  _writeCommand(0x92); // partial out
#if defined(ESP8266) || defined(ESP32)
  yield();
#else
  delay(1);
#endif
}

void GxEPD2_350c::writeImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                 int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, NULL, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_350c::writeImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                 int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  _writeImagePart(0x10, black, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  _writeImagePart(0x13, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
}

void GxEPD2_350c::_writeImagePart(uint8_t command, const uint8_t* bitmap, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                  int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (_initial_write) writeScreenBuffer(); // initial full screen buffer clean
  
#if defined(ESP8266) || defined(ESP32)
  yield();
#else
  delay(1);
#endif

  if ((w_bitmap < 0) || (h_bitmap < 0) || (w < 0) || (h < 0)) return;
  if ((x_part < 0) || (x_part >= w_bitmap)) return;
  if ((y_part < 0) || (y_part >= h_bitmap)) return;
  int16_t wb_bitmap = (w_bitmap + 7) / 8; // width bytes, bitmaps are padded
  x_part -= x_part % 8; // byte boundary
  w = w_bitmap - x_part < w ? w_bitmap - x_part : w; // limit
  h = h_bitmap - y_part < h ? h_bitmap - y_part : h; // limit
  x -= x % 8; // byte boundary
  w = 8 * ((w + 7) / 8); // byte boundary, bitmaps are padded
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  int16_t w1 = x + w < int16_t(WIDTH) ? w : int16_t(WIDTH) - x; // limit
  int16_t h1 = y + h < int16_t(HEIGHT) ? h : int16_t(HEIGHT) - y; // limit
  int16_t dx = x1 - x;
  int16_t dy = y1 - y;
  w1 -= dx;
  h1 -= dy;
  if ((w1 <= 0) || (h1 <= 0)) return;

  if (!_using_partial_mode) _Init_Part();
  _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _writeCommand(command);

  // Optimization: Handle NULL bitmap separately
  if (!bitmap) 
  {
      uint8_t data = invert ? 0x00 : 0xFF;
      for (int16_t i = 0; i < h1; i++) {
        for (int16_t j = 0; j < w1 / 8; j++) {
           _writeData(data);
        }
      }
  } 
  else 
  {
      // Optimization: Hoist calculations out of inner loop
      for (int16_t i = 0; i < h1; i++)
      {
        int32_t row_offset; // int32 for safety
        if (mirror_y) {
           row_offset = (int32_t)(h_bitmap - 1 - (y_part + i + dy)) * wb_bitmap + x_part / 8 + dx / 8;
        } else {
           row_offset = (int32_t)(y_part + i + dy) * wb_bitmap + x_part / 8 + dx / 8;
        }

        for (int16_t j = 0; j < w1 / 8; j++)
        {
          uint8_t data;
          int32_t idx = row_offset + j; // Fast addition only
          
          if (pgm)
          {
  #if defined(__AVR) || defined(ESP8266) || defined(ESP32)
            data = pgm_read_byte(&bitmap[idx]);
  #else
            data = bitmap[idx];
  #endif
          }
          else
          {
            data = bitmap[idx];
          }
          if (invert) data = ~data;
          _writeData(data);
        }
      }
  }

  _writeCommand(0x92); // partial out
  
#if defined(ESP8266) || defined(ESP32)
  yield();
#else
  delay(1);
#endif
}

void GxEPD2_350c::writeNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  if (data1)
  {
    writeImage(data1, x, y, w, h, invert, mirror_y, pgm);
  }
}

void GxEPD2_350c::drawImage(const uint8_t bitmap[], int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_350c::drawImagePart(const uint8_t bitmap[], int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(bitmap, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_350c::drawImage(const uint8_t* black, const uint8_t* color, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImage(black, color, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_350c::drawImagePart(const uint8_t* black, const uint8_t* color, int16_t x_part, int16_t y_part, int16_t w_bitmap, int16_t h_bitmap,
                                int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeImagePart(black, color, x_part, y_part, w_bitmap, h_bitmap, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_350c::drawNative(const uint8_t* data1, const uint8_t* data2, int16_t x, int16_t y, int16_t w, int16_t h, bool invert, bool mirror_y, bool pgm)
{
  writeNative(data1, data2, x, y, w, h, invert, mirror_y, pgm);
  refresh(x, y, w, h);
}

void GxEPD2_350c::refresh(bool partial_update_mode)
{
  if (partial_update_mode) refresh(0, 0, WIDTH, HEIGHT);
  else _Update_Full();
}

void GxEPD2_350c::refresh(int16_t x, int16_t y, int16_t w, int16_t h)
{
  // intersection with screen
  int16_t w1 = x < 0 ? w + x : w; // reduce
  int16_t h1 = y < 0 ? h + y : h; // reduce
  int16_t x1 = x < 0 ? 0 : x; // limit
  int16_t y1 = y < 0 ? 0 : y; // limit
  w1 = x1 + w1 < int16_t(WIDTH) ? w1 : int16_t(WIDTH) - x1; // limit
  h1 = y1 + h1 < int16_t(HEIGHT) ? h1 : int16_t(HEIGHT) - y1; // limit
  if ((w1 <= 0) || (h1 <= 0)) return; 
  // make x1, w1 multiple of 8
  w1 += x1 % 8;
  if (w1 % 8 > 0) w1 += 8 - w1 % 8;
  x1 -= x1 % 8;
  _Init_Part();
  if (usePartialUpdateWindow) _writeCommand(0x91); // partial in
  _setPartialRamArea(x1, y1, w1, h1);
  _Update_Part();
  if (usePartialUpdateWindow) _writeCommand(0x92); // partial out
}

void GxEPD2_350c::powerOff()
{
  _PowerOff();
}

void GxEPD2_350c::hibernate()
{
  _PowerOff();
  if (_rst >= 0)
  {
    _writeCommand(0x07); // deep sleep
    _writeData(0xA5);    // check code
    _hibernating = true;
  }
}

void GxEPD2_350c::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = (x + w - 1) | 0x0007; // byte boundary inclusive (last byte)
  uint16_t ye = y + h - 1;
  x &= 0xFFF8; // byte boundary
  _writeCommand(0x90); // partial window
  //_writeData(x / 256);
  _writeData(x % 256);
  //_writeData(xe / 256);
  _writeData(xe % 256);
  _writeData(y / 256);
  _writeData(y % 256);
  _writeData(ye / 256);
  _writeData(ye % 256);
  _writeData(0x01); // don't see any difference
  //_writeData(0x00); // don't see any difference
}

void GxEPD2_350c::_PowerOn()
{
  if (!_power_is_on)
  {
    _writeCommand(0x04);
    _waitWhileBusy("_PowerOn", power_on_time);
  }
  _power_is_on = true;
}

void GxEPD2_350c::_PowerOff()
{
  _writeCommand(0x50);
  _writeData(0xf7); // border floating
  _writeCommand(0x02); // power off
  _waitWhileBusy("_PowerOff", power_off_time);
  _power_is_on = false;
}

void GxEPD2_350c::_InitDisplay()
{
  if (_hibernating) _reset();
  _writeCommand(0x06);
  _writeData (0x17);
  _writeData (0x17);
  _writeData (0x17);
  //_writeCommand(0x04);
  //_waitWhileBusy("_wakeUp Power On");
  _writeCommand(0x00);     //panel setting
  _writeData(0x0f);    //LUT from OTP£¬128x296
  _writeData(0x0d);     //VCOM to 0V fast
  _writeCommand(0x50);
  _writeData(0x77);
  _writeCommand(0x61);
  _writeData (0xB8);
  _writeData (0x01);
  _writeData (0x80);
}

void GxEPD2_350c::_Init_Full()
{
  _InitDisplay();
  _PowerOn();
}

void GxEPD2_350c::_Init_Part()
{
  _InitDisplay();
  _writeCommand(0X50);
  _writeData(0xF7);    //WBmode:VBDF 17|D7 VBDW 97 VBDB 57   WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
  _PowerOn();
}

void GxEPD2_350c::_Update_Full()
{
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Full", full_refresh_time);
}

void GxEPD2_350c::_writePartialLut()
{
  _writeCommand(0x32);
  const uint16_t lut_len = sizeof(lut_bwr_350_part);
  for (uint16_t i = 0; i < lut_len; i++)
  {
    _writeData(lut_bwr_350_part[i]);
  }
}

void GxEPD2_350c::_Update_Part()
{
  _writePartialLut();
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("_Update_Part", partial_refresh_time);
}