/****************************************************************************
*
* Copyright 2019 DS LAB All Rights Reserved.
*
* Filename: 1.ex_LED.ino
* Author: sj.yang
* Release date: 2019/04/03
* Version: 1.0
*
****************************************************************************/
#define GPIO_PORT_LED 1

int PORT_LED = GPIO_PORT_LED;
int PIN_RED,PIN_GRE;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("*** HexT LED Board Test ***"); 
    
  if (PORT_LED==0)
  {
    PIN_RED = D2;
    PIN_GRE = D4;
  }
  else if (PORT_LED==1)
  {
    PIN_RED = D7;
    PIN_GRE = D8;
  }
  else{}
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GRE, OUTPUT);
}

void loop()
{
  digitalWrite(PIN_RED, HIGH);
  digitalWrite(PIN_GRE, LOW);
  Serial.println("* - RED LED ON , GREEN LED OFF");
  delay(1000);
  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_GRE, HIGH);
  Serial.println("* - RED LED OFF, GREEN LED ON");
  delay(1000);
}
