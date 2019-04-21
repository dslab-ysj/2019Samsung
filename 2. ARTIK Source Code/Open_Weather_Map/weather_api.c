/******************************************************************************
 *
 * Copyright 2019 Barun Corechips All Rights Reserved
 *
 * Project : 22_Open_Weather_App
 * Filename : weather_api.c
 * Author : sj.yang, wc.park
 * Version : 1.0
 *
 *******************************************************************************/

#include "weather_api.h"

static artik_network_dhcp_client_handle g_dhcp_handle = NULL;

void wifi_connect_callback(void *result, void *user_data)
{
	struct callback_result *res = (struct callback_result *)user_data;
	memcpy(&(res->info), (artik_wifi_connection_info *)result,
			sizeof(artik_wifi_connection_info));
	sem_post(&(res->sem));
}

int wifi_connect(const char* ssid, const char* passphrase)
{
	int ret = 0;
	artik_error err = S_OK;
	struct callback_result conn_res;
	struct timespec timeout;
	artik_wifi_module *wifi = NULL;

	if (!ssid) {
		fprintf(stderr, "SSID is not defined.\n");
		return -1;
	}
	wifi = (artik_wifi_module *)
			artik_request_api_module("wifi");
	if (!wifi) {
		fprintf(stderr, "Failed to request wifi module\n");
		return -1;
	}

	err = wifi->init(ARTIK_WIFI_MODE_STATION);
	if (err != S_OK) {
		fprintf(stderr, "Failed to initialize wifi (%s)\n", error_msg(err));
		ret = -1;
		goto exit;
	}

	sem_init(&(conn_res.sem), 0, 0);

	err = wifi->set_connect_callback(wifi_connect_callback, &(conn_res.sem));
	if (err != S_OK) {
		sem_destroy(&(conn_res.sem));
		fprintf(stderr, "Failed to set connect callback (%)\n", error_msg(err));
		ret = -1;
		goto exit;
	}
	err = wifi->connect(ssid, passphrase, false);
	if (err != S_OK) {
		sem_destroy(&(conn_res.sem));
		wifi->set_connect_callback(NULL, NULL);
		fprintf(stderr, "Failed to start connection to AP (%s)\n",
				error_msg(err));
		ret = -1;
		goto exit;
	}

	clock_gettime(CLOCK_REALTIME, &timeout);
	timeout.tv_sec += WIFI_CONNECT_TIMEOUT;
	if ((sem_timedwait(&(conn_res.sem), &timeout) < 0)
			&& (errno == ETIMEDOUT)) {
		sem_destroy(&(conn_res.sem));
		wifi->set_connect_callback(NULL, NULL);
		fprintf(stderr, "Timed out while waiting for connection\n");
		ret = -1;
		goto exit;
	}

	sem_destroy(&(conn_res.sem));
	wifi->set_connect_callback(NULL, NULL);
	if (!conn_res.info.connected) {
		fprintf(stderr, "Failed to join the network (err=%d)\n",
				conn_res.info.error);
		ret = -1;
		goto exit;
	}

	fprintf(stderr, "Connected to %s\n", ssid);
exit:
	artik_release_api_module(wifi);
	return ret;
}

int start_dhcp_client(void) {
	int ret = 0;
	artik_network_module *network = (artik_network_module *)
			artik_request_api_module("network");
	artik_error err = S_OK;

	if (!network) {
		fprintf(stderr, "Failed to request network module\n");
		return -1;
	}

	err = network->dhcp_client_start(&g_dhcp_handle, ARTIK_WIFI);
	if (err != S_OK) {
		fprintf(stderr, "Failed to request DHCP lease (err=%d)\n", ret);
		ret = -1;
		goto exit;
	}

exit:
	artik_release_api_module(network);
	return ret;
}
