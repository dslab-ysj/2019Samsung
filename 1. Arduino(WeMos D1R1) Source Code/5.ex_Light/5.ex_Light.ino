/****************************************************************************
*
* Copyright 2019 DS LAB All Rights Reserved.
*
* Filename: 5.ex_Light.ino
* Author: sj.yang
* Release date: 2019/04/09
* Version: 1.0
*
****************************************************************************/
#define ADC_PORT_LIGHT 0

int PORT_LIGHT = ADC_PORT_LIGHT;
int PIN_LIGHT;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("HexT Light Sensor Test");  
  if (PIN_LIGHT==0)
  {
    PIN_LIGHT = A0;
  }
  else 
  {
    Serial.println("Please connecting Light Sensor to ADC0 Port");
  }
}

void loop()
{
  int ValADC = analogRead(PIN_LIGHT);
  Serial.print("ADC Value of Light Sensor :");
  Serial.println(ValADC);
  delay(1000);
}
