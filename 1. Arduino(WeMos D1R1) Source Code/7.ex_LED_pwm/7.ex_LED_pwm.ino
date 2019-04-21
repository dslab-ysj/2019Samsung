/****************************************************************************
*
* Copyright 2019 DS LAB All Rights Reserved.
*
* Filename: 7.ex_LED_pwm.ino
* Author: sj.yang
* Release date: 2019/04/08
* Version: 1.0
*
****************************************************************************/

/******************************
* PWM0 = D3;
* PWM1 = D5;
* PWM2 = D6;
* PWM3 = D9;
*******************************/

#define PWM_PORT_LED 0 // or 1
#define MAX_PWM 255

int PORT_LED = PWM_PORT_LED;
int PIN_RED,PIN_GRE;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("*** HexT LED Board Test(PWM) ***"); 

  if (PORT_LED==0)
  {
    PIN_RED = D3;
    PIN_GRE = D5;
  }
  else if (PORT_LED==1)
  {
    PIN_RED = D6;
    PIN_GRE = D9;
  }
  else
  {}
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GRE, OUTPUT);
}

void loop() 
{
  int i=0;
  
  for(i = 0; i < MAX_PWM; i++)
  { // 255번 * 20ms -> 약 5s
    analogWrite(PIN_RED,i);
    analogWrite(PIN_GRE,i);
    delay(20);   
  }
  for(i = MAX_PWM; i > 0; i--)
  {
    analogWrite(PIN_RED,i);
    analogWrite(PIN_GRE,i);
    delay(20);   
  }
}
