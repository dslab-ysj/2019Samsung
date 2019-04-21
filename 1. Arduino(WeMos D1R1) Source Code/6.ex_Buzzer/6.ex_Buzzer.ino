/****************************************************************************
*
* Copyright 2019 DS LAB All Rights Reserved.
*
* Filename: 6.ex_Buzzer.ino
* Author: sj.yang
* Release date: 2019/04/03
* Version: 1.0
*
****************************************************************************/

/******************************
* PWM0 = D3;
* PWM1 = D5;
* PWM2 = D6;
* PWM3 = D9;
*******************************/

#include "pitches.h"

#define PWM_PORT_BUZ 0

int PORT_BUZ = PWM_PORT_BUZ;
int PIN_BUZ;
int ArraySize = sizeof(Korean_National_Anthem_M)/sizeof(int);
int TaegeukgiSize = sizeof(Taegeukgi)/sizeof(char);
                 
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("*** HexT Buzzer Board Test ***"); 

  if(PORT_BUZ==0)
  {
    PIN_BUZ=D3;
  }
  else if(PORT_BUZ==1)
  {
    PIN_BUZ=D6;
  }
  else
  {}
  pinMode(PIN_BUZ, OUTPUT);
}

void BuzzerPlay(int PIN, int FREQ, int LENGTH)
{ // BuzzerPlay( , [Hz], [ms])
  int duty_cycle = 50;
  if(FREQ == 0)
  { 
    noTone(PIN);
  }
  else
  {
    analogWriteFreq(FREQ);
    analogWrite(PIN, duty_cycle);
  }
  delay(LENGTH*250);
}
void loop() 
{
  for(int i=0; i<TaegeukgiSize; i++)
  {
    if (i%30==0) Serial.println();
    Serial.print(Taegeukgi[i]); 
    Serial.print(" ");     
  }
  for(int i=0; i<ArraySize; i++)
  {
    BuzzerPlay(PIN_BUZ, Korean_National_Anthem_M[i],Korean_National_Anthem_R[i]);
  }
  delay(10000);
}
