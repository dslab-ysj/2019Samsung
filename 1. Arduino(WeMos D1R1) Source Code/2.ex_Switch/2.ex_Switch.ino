/****************************************************************************
*
* Copyright 2019 DS LAB All Rights Reserved.
*
* Filename: 2.ex_Switch.ino
* Author: sj.yang
* Release date: 2019/04/03
* Version: 1.0
*
****************************************************************************/
#define GPIO_PORT_SWI 0 // or 1

int PORT_SWI = GPIO_PORT_SWI;
int PORT_LED = !GPIO_PORT_SWI;

int PIN_SWL,PIN_SWR;
int PIN_RED,PIN_GRE;

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
    
    PIN_RED = D7;
    PIN_GRE = D8;    
  }
  else if (PORT_SWI==1)
  {
    PIN_SWL = D7;
    PIN_SWR = D8;
    
    PIN_RED = D2;
    PIN_GRE = D4; 
  }
  else{}
  pinMode(PIN_SWL, INPUT);
  pinMode(PIN_SWR, INPUT);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GRE, OUTPUT);
}

void loop()
{
  SWL_state = digitalRead(PIN_SWL);
  SWR_state = digitalRead(PIN_SWR);

  if(SWL_state != last_SWL_state)
  {
    digitalWrite(PIN_RED, SWL_state);
    if(SWL_state){Serial.println("* - RED LED ON");}
    else {Serial.println("* - RED LED OFF");}
  } else{}

  if(SWR_state != last_SWR_state)
  {
    digitalWrite(PIN_GRE, SWR_state); 
    if(SWR_state){Serial.println("* - GREEN LED ON ");}
    else {Serial.println("* - GREEN LED OFF ");}
  } else{}
  
  last_SWL_state = SWL_state;
  last_SWR_state = SWR_state;
}
