/******************************************************************************
 *
 * Copyright 2019 Barun Corechips All Rights Reserved
 *
 * Project : 22_Open_Weather_App
 * Filename : main.c
 * Author : sj.yang, wc.park
 * Version : 1.0
 *
 *******************************************************************************/

#include <stdio.h>
#include <tinyara/config.h>
#include <apps/shell/tash.h>
#include "weather_api.h"

// For Parsing Defines
#define TEMP_LEN 7
#define HUMI_LEN 3
#define TMIN_LEN 7
#define TMAX_LEN 7
#define DATA_OFFSET 2
#define ABS_TEMP 273.15

int weather_api_main(void)
{
	if (wifi_connect(SSID, PSK) != 0)
	{
		return -1;
	}
	start_dhcp_client();

	int sockfd, len;
	int ret;
	struct sockaddr_in server_addr;
	char str[MAX_DATA_SIZE]={0};
	//char *message = "POST /data/2.5/weather?q=<city name>&appid=<my api key> HTTP/1.0\r\n\r\n";
	//q=<원하는 도시명>,appid=<my api key>
	char *message = "POST /data/2.5/weather?q=suigen&appid=a667a1402db485e82f9eb341a088445d HTTP/1.0\r\n\r\n";
	char *host = "api.openweathermap.org";	//weather api
	int PORT = 80;
	struct hostent *hostip;

	//for parsing variables
	char *temp_offset, *humi_offset, *tmin_offset, *tmax_offset;
	char temp_arr[TEMP_LEN]={0};
	char humi_arr[HUMI_LEN]={0};
	char tmin_arr[TEMP_LEN]={0};
	char tmax_arr[TEMP_LEN]={0};

	int i;

	// set up address to connect to
	//printf("Set up address...\n");
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_len = sizeof(server_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	hostip = gethostbyname(host);
	if(hostip == NULL) printf("[Error] no such host]n");
	memcpy(&server_addr.sin_addr.s_addr,hostip->h_addr,hostip->h_length);

	// create the socket
	//printf("Create the socket...\n");
	sockfd = lwip_socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		printf("[Error] lwip_socket()\n");
		return -1;
	}

	// connect
	//printf("Connect to server...\n");
	ret = lwip_connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (ret == -1) {
		printf("[Error] lwip_connect()\n");
		return -1;
	}

	len = strlen(message);
	ret = lwip_write(sockfd,message,len);
	if (ret != (int)len) {
		printf("[Error] lwip_write()\n");
		return -1;
	}
	lwip_read(sockfd,str, MAX_DATA_SIZE);
	printf("\nFull data : %s\n\n",str);	//Use api by parsing this string

	// Parsing
	temp_offset = strstr(str,"temp") + strlen("temp") + DATA_OFFSET;
	for(i = 0 ; i < TEMP_LEN-1 ; i++)
	{
		if (*(temp_offset+(TEMP_LEN-1))==",") break;
		temp_arr[i] = *(temp_offset+i);
	}

	humi_offset = strstr(str,"humidity") + strlen("humidity") + DATA_OFFSET;
	for(i = 0 ; i < HUMI_LEN-1 ; i++)
	{
		if (*(humi_offset+(HUMI_LEN-1))==",") break;
		humi_arr[i] = *(humi_offset+i);
	}

	tmin_offset = strstr(str,"temp_min") + strlen("temp_min") + DATA_OFFSET;
	for(i = 0 ; i < TEMP_LEN-1 ; i++)
	{
		if (*(tmin_offset+(TEMP_LEN-1))==",") break;
		tmin_arr[i] = *(tmin_offset+i);
	}

	tmax_offset = strstr(str,"temp_max") + strlen("temp_max") + DATA_OFFSET;
	for(i = 0 ; i < TEMP_LEN-1 ; i++)
	{
		if (*(tmax_offset+(TEMP_LEN-1))==",") break;
		tmax_arr[i] = *(tmax_offset+i);
	}
	printf("\n*** The Weather of Suwon(suigen) ***\n");
	printf("| Current Temp. : %.2f degree\n",atof(temp_arr)-ABS_TEMP);
	printf("| Maxinum Temp. : %.2f degree\n",atof(tmax_arr)-ABS_TEMP);
	printf("| Minimum Temp. : %.2f degree\n",atof(tmin_arr)-ABS_TEMP);
	printf("|   Humidity    : %.2f%% \n",atof(humi_arr));
	printf("*************************************\n");

	return 0;
}

int main(int argc, FAR char *argv[])
{
	tash_cmd_install("weather_api",weather_api_main, TASH_EXECMD_SYNC);
	return 0;
}
