/****************************************************************************
*
* Copyright 2019 DS LAB All Rights Reserved.
*
* Filename: 2.ex_Switch_ori.ino
* Author: sj.yang
* Release date: 2019/04/05
* Version: 1.0
*
****************************************************************************/
#define GPIO_PORT_SWI 0 

int PORT_SWI = GPIO_PORT_SWI;

int PIN_SWL,PIN_SWR;
int SWL_state=0;
int SWR_state=0;
int last_SWL_state=0;
int last_SWR_state=0;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("*** HexT Switch Board Test ***"); 
    
  if (PORT_SWI==0)
  {
    PIN_SWL = D2;
    PIN_SWR = D4;
  }
  else if (PORT_SWI==1)
  {
    PIN_SWL = D7;
    PIN_SWR = D8;
  }
  else{}
  pinMode(PIN_SWL, INPUT);
  pinMode(PIN_SWR, INPUT);
}

void loop()
{
  SWL_state = digitalRead(PIN_SWL);
  SWR_state = digitalRead(PIN_SWR);
  
  if(SWL_state != last_SWL_state)
  {
    if(SWL_state){Serial.println("* - LEFT  SWITCH ON");}
    else {Serial.println("* - LEFT  SWITCH OFF");}
  } else{}

  if(SWR_state != last_SWR_state)
  {
    if(SWR_state){Serial.println("* - RIGHT SWITCH ON");}
    else {Serial.println("* - RIGHT SWITCH OFF");}
  } else{}
  
  last_SWL_state = SWL_state;
  last_SWR_state = SWR_state;
}
