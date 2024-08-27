#include <Arduino.h>

#define AGC_PIN PB1
HardwareSerial uart2(PA3, PA2);

void setup() 
{
  pinMode(AGC_PIN, INPUT);
  uart2.begin(115200);
  uart2.printf("Clock speed: %d Hz\n", SystemCoreClock);
}

void loop() 
{
  uint16_t adcValue=analogRead(AGC_PIN);
  uart2.println(adcValue);
  delay(1000);
  
}
