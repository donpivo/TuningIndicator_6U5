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
#define CENTER_X 62
#define CENTER_Y 74
#define OUTER_RADIUS 52
#define INNER_RADIUS 27

Arduino_DataBus *bus = new Arduino_HWSPI(PIN_DC, PIN_CS);
Arduino_GFX *gfx = new Arduino_GC9107(bus, PIN_RES, 0 /* rotation */, true /* IPS */);

void drawLine(uint16_t angle, uint16_t lineColor);

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


  for(uint16_t i = 0; i < 270; i++)
  {
    drawLine(i,BRIGHTGREEN);
  }

  for(uint16_t i = 270; i < 360; i++)
  {
    drawLine(i,DARKGREEN);
  }
  
  delay(1000);


  for (uint16_t i = 0; i < 1024; i++)
  {
    analogWrite(PIN_BACKLIGHT, uint16_t(i*i/4112));
    delay(5);
  }  
  digitalWrite(PIN_BACKLIGHT,HIGH);

  


  
}

void loop()
{
  for(uint16_t i = 0; i <= 45; i++)
  {
    drawLine(270+i,BRIGHTGREEN);
    drawLine(360-i,BRIGHTGREEN);
    delay(10);
  }
  for(uint16_t i = 0; i <= 45; i++)
  {
    drawLine(315-i,DARKGREEN);
    drawLine(315+i,DARKGREEN);
    delay(10);
  }
  delay(1000);
}


void drawLine(uint16_t angle, uint16_t lineColor)
{
  double sinVal = sin(angle*0.0174533);
  double cosVal = cos(angle*0.0174533);
  gfx->drawLine(CENTER_X+sinVal*INNER_RADIUS, CENTER_Y+cosVal*INNER_RADIUS, CENTER_X+sinVal*OUTER_RADIUS, CENTER_Y+cosVal*OUTER_RADIUS, lineColor);
  gfx->drawLine(CENTER_X+sinVal*INNER_RADIUS, CENTER_Y+1+cosVal*INNER_RADIUS, CENTER_X+sinVal*OUTER_RADIUS, CENTER_Y+1+cosVal*OUTER_RADIUS, lineColor);
}