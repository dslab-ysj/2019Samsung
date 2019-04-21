/****************************************************************************
*
* Copyright 2019 DS LAB All Rights Reserved.
*
* Filename: 4.ex_IR.ino
* Author: sj.yang
* Release date: 2019/04/09
* Version: 1.0
*
****************************************************************************/
#define ADC_PORT_IR 0

int PORT_IR = ADC_PORT_IR;
int PIN_IR;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("HexT IR Sensor Test");  
  if (PIN_IR==0)
  {
    PIN_IR = A0;
  }
  else 
  {
    Serial.println("Please connecting IR Sensor to ADC0 Port");
  }
}

void loop()
{
  int ValADC = analogRead(PIN_IR);
  Serial.print("ADC Value of IR Sensor :");
  Serial.println(ValADC);
  delay(1000);
}
