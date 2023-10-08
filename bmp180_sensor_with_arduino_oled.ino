#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "U8glib.h"
Adafruit_BMP085 bmp;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST); // Fast I2C / TWI
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC

float pressure = 0.0;
float tempC = 0.0;
float altitude = 0.0;
void BmpSensorRead(float* pressure, float* tempC, float* altitude);
void DisplayPresTemp(float* pressure, float* tempC, float* altitude);

void setup(void)
{
  Serial.begin(9600);

  // assign default color value
  if (u8g.getMode() == U8G_MODE_R3G3B2)
  {
    u8g.setColorIndex(255);     // white
  }
  else if (u8g.getMode() == U8G_MODE_GRAY2BIT)
  {
    u8g.setColorIndex(3);         // max intensity
  }
  else if (u8g.getMode() == U8G_MODE_BW)
  {
    u8g.setColorIndex(1);         // pixel on
  }
  else if (u8g.getMode() == U8G_MODE_HICOLOR)
  {
    u8g.setHiColorByRGB(255, 255, 255);
  }

  for (int a = 0; a < 30; a++)
  {
    u8g.firstPage();

    do
    {
      u8g.setFont(u8g_font_fub11);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      u8g.drawStr(4, a, "BMP180 Sensor");
    }
    while (u8g.nextPage());
  }

  delay(3000);

  if (!bmp.begin())
  {
    u8g.firstPage();

    do
    {
      u8g.setFont(u8g_font_fub11);
      u8g.setFontRefHeightExtendedText();
      u8g.setDefaultForegroundColor();
      u8g.setFontPosTop();
      u8g.drawStr(4, 0, "BMP085 Sensor");
      u8g.drawStr(4, 20, " ERROR!");
    }
    while (u8g.nextPage());

    Serial.println("BMP085 sensor, ERROR!");

    while (1) {}
  }
}
void loop(void)
{
  BmpSensorRead(&pressure, &tempC, &altitude);
  DisplayPresTemp(&pressure, &tempC, &altitude);
  delay(1000);
}
void DisplayPresTemp(float* pressure, float* tempC, float* altitude)
{
  u8g.firstPage();

  do
  {
    u8g.setFont(u8g_font_fub11);
    u8g.setFontRefHeightExtendedText();
    u8g.setDefaultForegroundColor();
    u8g.setFontPosTop();
    u8g.drawStr(2, 0, "Pressure");
    u8g.setPrintPos(75, 0);
    u8g.print(*pressure);
    u8g.drawStr(4, 20, "Temp C");
    u8g.setPrintPos(75, 20);
    u8g.print(*tempC);
    u8g.drawStr(4, 40, "Altitude");
    u8g.setPrintPos(75, 40);
    u8g.print(*altitude);
  }
  while (u8g.nextPage());
}
void BmpSensorRead(float* pressure, float* tempC, float* altitude)
{
  *tempC = bmp.readTemperature();
  Serial.print("Temperature = ");
  Serial.print(*tempC);
  Serial.println(" *C");

  *pressure = bmp.readPressure() / 100.0;
  Serial.print("Pressure = ");
  Serial.print(*pressure / 100.0);
  Serial.println(" hPa");

  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  *altitude = bmp.readAltitude();
  Serial.print("Altitude = ");
  Serial.print(*altitude);
  Serial.println(" meters");
}
