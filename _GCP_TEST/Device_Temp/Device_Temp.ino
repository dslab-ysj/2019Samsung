#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define device_id "team3-temp" //Device ID

char *ssid = "D.S.Lab"; 
char *pass = "dslab123";
int state = WL_IDLE_STATUS;

unsigned int port = 10000;
char host[] = "192.168.0.25"; //Gateway IP Address

char packetBuffer[255]; //buffer to hold incoming packet
char ReplyBuffer[] = "acknowledged";       // a string to send back

char temp[100];
char str[100];

WiFiUDP UDP;

void RecvMessage (int packetSize){
  if(!packetSize)
    packetSize = UDP.parsePacket();
    
  if(packetSize){
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = UDP.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(UDP.remotePort());

    // read the packet into packetBufffer
    int len = UDP.read(packetBuffer, 255);
    
    if (len > 0)
    {
      packetBuffer[len] = 0;
    }
    
    String team2 = packetBuffer;

    if(team2.compareTo("TempHigh") == 0 )
    {
      Serial.println("   - Temp High (From team3)");
    }
    if(team2.compareTo("TempLow") == 0 )
    {
      Serial.println("   - Temp Low (From team3)");
    }
        
    Serial.print("**Contents:");
    Serial.println(packetBuffer);
  }
}

void SendMessage(char *action, char *data){
  int i;
  char message[100];

  int len_id = strlen(device_id);
  int len_ac = strlen(action);
  int len_da = strlen(data);

  char *msg_device = "{ \"device\" : \"";
  char *msg_action = "\", \"action\":\"";
  char *msg_data = "\", \"data\":\"";
  char *msg_finish = "\" }";
  
  strcpy(message,msg_device);
  strcat(message,device_id);
  strcat(message,msg_action);
  strcat(message,action);

  if(len_da != 1){
    strcat(message,msg_data);
    strcat(message,data);
  }
  
  strcat(message,msg_finish);
  int packetSize = UDP.parsePacket();

    UDP.beginPacket(host,port);//send ip to server
    char ipBuffer[255];
    WiFi.localIP().toString().toCharArray(ipBuffer, 255);
    UDP.write(message);
    Serial.print("Send message : ");
    Serial.println(message);
    UDP.endPacket();

    while(packetSize == 0){
      packetSize = UDP.parsePacket();
    }

    RecvMessage(packetSize);

}

void setup() 
{ 
  pinMode(D4,OUTPUT);
  Serial.begin(115200);
//  if(!WiFi.getAutoConnect()){
//    Serial.print("Connecting to ");
//    Serial.println(ssid);
    WiFi.begin(ssid, pass);
//  }else{
//    Serial.print("AutoConnect");
//  }

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("...");
  }

  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
  
  UDP.begin(port);
  
  SendMessage("detach","\0");
  SendMessage("attach","\0");
  SendMessage("subscribe","\0");
 
}

void loop() 
{
  int i;
  
  for(i = 0 ; i < 100 ; i++){
      temp[i] = 0;
      str[i] = 0;
      RecvMessage(0);
      delay(30);
  }

  itoa(analogRead(A0)*330/1024-55,temp,10);
  
  strcpy(str,"temp=");
  strcat(str,temp);
  
  SendMessage("event",str);

}
