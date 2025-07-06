/*
6U5 Digital Replica by retronics.no
*/

#include <Arduino.h> 
#include <Arduino_GFX_Library.h> //https://github.com/moononournation/Arduino_GFX | GFX Library for Arduino@^1.4.7

#define PIN_BACKLIGHT PB0
#define PIN_DC PB11 
#define PIN_CS PA4  
#define PIN_RES PB2
// PIN_SCL A5
// PIN_SDA A7
#define GREEN_BRIGHT RGB565(0, 255, 80)
#define GREEN_MEDIUM RGB565(0, 180, 60)
#define GREEN_DARK RGB565(0, 80, 35)
#define CENTER_X 63
#define CENTER_Y 75
#define OUTER_RADIUS 52
#define INNER_RADIUS 24
#define PIN_AGC PB1
#define BRIGHTNESS 255 // Max 255

Arduino_DataBus *bus = new Arduino_HWSPI(PIN_DC, PIN_CS);
Arduino_GFX *gfx = new Arduino_GC9107(bus, PIN_RES, 0 /* rotation */, true /* IPS */, GC9107_TFTWIDTH, GC9107_TFTHEIGHT, 0, 0, 0, 0);
HardwareSerial uart2(PA3, PA2);
uint8_t oldHalfAngle;
uint32_t lastDisplayUpdate=millis();
double agcFiltered;
uint32_t lastDirectionChange=millis();
uint8_t fadeInCounter=0;
uint32_t fadeInLastChange=millis();

void drawLine(uint16_t angle, uint16_t lineColor);

void setup(void)
{
  analogWriteResolution(16);
  uart2.begin(115200);
  pinMode(PIN_AGC, INPUT);
  pinMode(PIN_BACKLIGHT, OUTPUT);
  digitalWrite(PIN_BACKLIGHT,LOW);
  uart2.println("Digital 6U5 replacement.");

  //Display startup
  if (!gfx->begin())
  {
    uart2.println("gfx->begin() failed!");
  }
  gfx->fillScreen(BLACK);
  for(int16_t i = 46; i < 315; i++)
  {
    drawLine(i,GREEN_MEDIUM);
  }
  drawLine(315, GREEN_BRIGHT);
  drawLine(45, GREEN_BRIGHT);
  for(int16_t i = 316; i < 404; i++)
  {
    drawLine(i,GREEN_DARK);
  }
  delay(1000);
}

void loop()
{
  //Fade in
  if((fadeInCounter<BRIGHTNESS)&&(millis()-fadeInLastChange)>(120-(fadeInCounter/4)))
  {
    fadeInCounter++;
    analogWrite(PIN_BACKLIGHT, fadeInCounter*fadeInCounter);
    fadeInLastChange=millis();
  }
  //AGC measurement, filtering and mapping to display angle
  uint16_t agcRaw=analogRead(PIN_AGC);
  agcFiltered=(9.0*agcFiltered+agcRaw)/10.0;
  uint8_t newHalfAngle=(uint8_t)(((agcFiltered/11)+15.0*log10(1+agcFiltered))/3);
  newHalfAngle=newHalfAngle>45?45:newHalfAngle;

  //Display update
  if(newHalfAngle>oldHalfAngle)
  {
    for(int8_t i=oldHalfAngle; i<newHalfAngle; i++)
    {
      drawLine(315+i,GREEN_MEDIUM);
      drawLine(405-i,GREEN_MEDIUM);
    }
    drawLine(315+newHalfAngle,GREEN_BRIGHT);
    drawLine(405-newHalfAngle,GREEN_BRIGHT);
  }
  else if(newHalfAngle<oldHalfAngle)
  {
    for(int8_t i=oldHalfAngle; i>newHalfAngle; i--)
    {
      drawLine(315+i,GREEN_DARK);
      drawLine(405-i,GREEN_DARK);
    }
    drawLine(315+newHalfAngle,GREEN_BRIGHT);
    drawLine(405-newHalfAngle,GREEN_BRIGHT);
  }
  oldHalfAngle=newHalfAngle;
}


void drawLine(uint16_t angle, uint16_t lineColor)
{
  double sinVal = sin(angle*0.0174533);
  double cosVal = cos(angle*0.0174533);
  gfx->drawLine(CENTER_X+sinVal*INNER_RADIUS, CENTER_Y+cosVal*INNER_RADIUS, CENTER_X+sinVal*OUTER_RADIUS, CENTER_Y+cosVal*OUTER_RADIUS, lineColor);
  gfx->drawLine(CENTER_X+sinVal*INNER_RADIUS, CENTER_Y+1+cosVal*INNER_RADIUS, CENTER_X+sinVal*OUTER_RADIUS, CENTER_Y+1+cosVal*OUTER_RADIUS, lineColor);
}
