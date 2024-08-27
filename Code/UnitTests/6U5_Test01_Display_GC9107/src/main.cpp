#include <Arduino.h>
#include <Arduino_GFX_Library.h>

#define PIN_BACKLIGHT PB0
#define PIN_DC PA1
#define PIN_CS PA4
#define PIN_RES A0
// PIN_SCL A5
// PIN_SDA A7

#define BRIGHTGREEN RGB565(0, 255, 75)
#define DARKGREEN RGB565(0, 100, 30)

Arduino_DataBus *bus = new Arduino_HWSPI(PIN_DC, PIN_CS);
Arduino_GFX *gfx = new Arduino_GC9107(bus, PIN_RES, 2 /* rotation */, true /* IPS */);


void setup(void)
{
  Serial.begin(115200);
  Serial.println("GC9107 Display Test");
  pinMode(PIN_BACKLIGHT, OUTPUT);
  digitalWrite(PIN_BACKLIGHT,LOW);


  // Init Display
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }

  gfx->fillScreen(BLACK);
 
  gfx->setCursor(10, 42);

  for (uint8_t i = 0; i < 25; i++)
  {
    gfx->drawCircle(61, 51, 51-i, BRIGHTGREEN);
    gfx->drawCircle(62, 51, 51-i, BRIGHTGREEN);
  }


  
  
  for (uint16_t i = 0; i < 1024; i++)
  {
    analogWrite(PIN_BACKLIGHT, uint8_t(i*i/4112));
    delay(5);
  }  

  
}

void loop()
{

}
