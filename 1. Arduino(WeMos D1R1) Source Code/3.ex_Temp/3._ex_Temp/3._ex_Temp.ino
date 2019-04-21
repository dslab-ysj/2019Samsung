/****************************************************************************
*
* Copyright 2019 DS LAB All Rights Reserved.
*
* Filename: 3.ex_Temp.ino
* Author: sj.yang
* Release date: 2019/04/03
* Version: 1.0
*
****************************************************************************/
#define ADC_PORT_TEMP 0

int PORT_TEMP = ADC_PORT_TEMP;

int PIN_TEMP;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("HexT Temperature Sensor Test");  
  if (PORT_TEMP==0)
  {
    PIN_TEMP = A0;
  }
  else 
  {
    Serial.println("Please connecting Temp. Sensor to ADC0 Port");
  }
}

void loop()
{
  float ValTemp = analogRead(PIN_TEMP);
  ValTemp = ValTemp * 320/1023-50;
  Serial.print("Current Temp :");
  Serial.print(ValTemp,2);
  Serial.println(" C");
  delay(1000);
}
