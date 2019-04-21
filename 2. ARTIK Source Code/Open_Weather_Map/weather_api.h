/******************************************************************************
 *
 * Copyright 2019 Barun Corechips All Rights Reserved
 *
 * Project : 22_Open_Weather_App
 * Filename : weather_api.h
 * Author : sj.yang, wc.park
 * Version : 1.0
 *
 *******************************************************************************/

#ifndef WEATHER_API_H_
#define WEATHER_API_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <artik_module.h>
#include <artik_wifi.h>
#include <artik_network.h>
#include <readline.h>
#include <netdb.h>
#include <net/lwip/sockets.h>

#define WIFI_SCAN_TIMEOUT       15
#define WIFI_CONNECT_TIMEOUT    30
#define WIFI_DISCONNECT_TIMEOUT 10

#define BACKLOG 10
#define MAX_DATA_SIZE 1500

#define SSID "B_CORECHIPS" //FIX
#define PSK  "bcore00151" //FIX

struct callback_result {
	sem_t sem;
	artik_wifi_connection_info info;
	artik_error error;
};

void wifi_connect_callback(void *result, void *user_data);
int wifi_connect(const char* ssid, const char* passphrase);
int start_dhcp_client(void);
int weather_api_main(void);

#endif /* WEATHER_API_H_ */
