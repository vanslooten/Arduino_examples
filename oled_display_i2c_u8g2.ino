/*
 * https://home.et.utwente.nl/slootenvanf/2019/04/04/displays/
 * 
 * Learn more about the use of library u8g2:
 * https://github.com/olikraus/u8g2/wiki/u8x8reference
 * 
 * Can be used in combi with EVShield (in hardware i2c mode)
 * 
 * Connections (hardware i2c):
 * Display: Arduino:
 * GND      GND
 * VCC      VCC (3.3 or 5V)
 * SDA      SDA (pin A4)
 * SCL      SCL (pin A5)
 */
#include <U8g2lib.h>

// software i2c:
//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(7, 8, U8X8_PIN_NONE);
// hardware i2c:
U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE);

void setup() {
  display.begin();
  display.setPowerSave(0);

  display.setFont(u8x8_font_pxplusibmcgathin_f);
  display.drawString(0,0,"Hello World v1!");
  display.drawString(0,3,"1234567890123456");
  display.drawString(0,7,"Hello World 2019");

  // displaying variable on line 1:
  int speed = 30;
  display.drawString(0,1,"Speed: ");
  display.setCursor(7, 1);
  display.print(speed);
}

void loop() {
}
