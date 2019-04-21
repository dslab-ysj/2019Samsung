#include "keys.h"
#include "A053BasicKit.h"

// NTP
#define NTP_REFRESH_PERIOD  (60 * 60) /* seconds */
#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))

#define RED_ON_BOARD_LED 45
#define ADC_MAX_SAMPLES 4
#define NET_DEVNAME "wl1"
#define DEFAULT_CLIENT_ID "123456789"
#define SERVER_ADDR "api.artik.cloud"//"52.200.124.224"
#define DEVICE_ID "57a54e8940ef4c72ac1a85855cce704c" //FIX
#define DEVICE_TOKEN "296b4c1fd57a48f9a42a15ef1895ec17" // FIX
#define ACTION_TOPIC "/v1.1/actions/57a54e8940ef4c72ac1a85855cce704c" // FIX
#define MESSAGE_TOPIC "/v1.1/messages/57a54e8940ef4c72ac1a85855cce704c" //FIX

// NTP
static struct ntpc_server_conn_s ntp_server_conn[] = {{ NULL, 123 },};

static void ntp_link_error(void)
{
	printf("NTP error: stopping client\n");
	ntpc_stop();
}

static mqtt_tls_param_t g_tls; // mqtt tls config handle

mqtt_client_t* pClientHandle = NULL; // mqtt client handle

mqtt_client_config_t clientConfig; // mqtt client parameters

// mqtt client on connect callback
void onConnect(void* client, int result) {printf("mqtt client connected to the server\n");}

// mqtt client on disconnect callback
void onDisconnect(void* client, int result){printf("mqtt client disconnected from the server\n");}

// mqtt client on message callback
void onMessage(void* client, mqtt_msg_t *msg)
{
	//printf("mqtt client on message received\n");
	mqtt_client_t *mqtt_client = (mqtt_client_t *)client;

	if (mqtt_client == NULL || mqtt_client->config == NULL)
	{
		printf("message callback: is NULL.\n");
		return;
	}

	if (msg->payload_len)
	{
		//printf("Topic - %s , payload -  %s\n", msg->topic, msg->payload);
		printf("\n-Payload Data------------------------------------------------\n");
		printf("%s\n", msg->payload);
		printf("-Payload Data------------------------------------------------\n");

	}
	else {/*printf(">>> message callback: payload_len is 0\n");*/ }

	char buf[msg->payload_len];
	strcpy(buf,msg->payload);

	if (strstr(buf,"setOn") != NULL) gpio_write(PIN_D4,HIGH);
	else if (strstr(buf,"setOff") != NULL) gpio_write(PIN_D4,LOW);
}

// Utility function to configure mqtt client
void initializeConfigUtil(void)
{
	uint8_t macId[IFHWADDRLEN];
	int result = netlib_getmacaddr("wl1", macId);
	if (result < 0)
	{
		printf("Get MAC Address failed. Assigning Client ID as 123456789");
		clientConfig.client_id = DEFAULT_CLIENT_ID; // MAC id Artik 053
	}
	else
	{
		printf("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",((uint8_t *) macId)[0],((uint8_t *) macId)[1],((uint8_t *) macId)[2],((uint8_t *) macId)[3],((uint8_t *) macId)[4],((uint8_t *) macId)[5]);
		char buf[12];
		sprintf(buf, "%02x%02x%02x%02x%02x%02x",((uint8_t *) macId)[0],((uint8_t *) macId)[1], ((uint8_t *) macId)[2],((uint8_t *) macId)[3], ((uint8_t *) macId)[4],((uint8_t *) macId)[5]);
		clientConfig.client_id = buf; // MAC id Artik 053
		clientConfig.user_name = DEVICE_ID;
		clientConfig.password = DEVICE_TOKEN;
		clientConfig.clean_session = true; // check

		clientConfig.protocol_version = MQTT_PROTOCOL_VERSION_31;
		g_tls.ca_cert = mqtt_get_ca_certificate();  //the pointer of ca_cert buffer
		g_tls.ca_cert_len = mqtt_get_ca_certificate_size(); // the length of ca_cert buffer

		g_tls.cert = NULL;
		g_tls.cert_len = 0;

		g_tls.key = NULL;
		g_tls.key_len = 0;

		printf("Registering mqtt client with id = %s\n", buf);
	}
	clientConfig.on_connect = (void*) onConnect;
	clientConfig.on_disconnect = (void*) onDisconnect;
	clientConfig.on_message = (void*) onMessage;
	clientConfig.tls = &g_tls;
}

int main(int argc, char *argv[])
{
	bool wifiConnected = false;
	gpio_write_toggle(RED_ON_BOARD_LED); // Turn on on board Red LED to indicate no WiFi connection is established

#ifdef CONFIG_CTRL_IFACE_FIFO
	int ret;

	while(!wifiConnected)
	{
		ret = mkfifo(CONFIG_WPA_CTRL_FIFO_DEV_REQ,CONFIG_WPA_CTRL_FIFO_MK_MODE);
		if (ret != 0 && ret != -EEXIST)
		{
			printf("mkfifo error for %s: %s",CONFIG_WPA_CTRL_FIFO_DEV_REQ,strerror(errno));
		}
		ret = mkfifo(CONFIG_WPA_CTRL_FIFO_DEV_CFM,CONFIG_WPA_CTRL_FIFO_MK_MODE);
		if (ret != 0 && ret != -EEXIST)
		{
			printf("mkfifo error for %s: %s",CONFIG_WPA_CTRL_FIFO_DEV_CFM,strerror(errno));
		}

		ret = mkfifo(CONFIG_WPA_MONITOR_FIFO_DEV,CONFIG_WPA_CTRL_FIFO_MK_MODE);
		if (ret != 0 && ret != -EEXIST)
		{
			printf("mkfifo error for %s: %s",CONFIG_WPA_MONITOR_FIFO_DEV,strerror(errno));
		}
#endif

		if (start_wifi_interface() == SLSI_STATUS_ERROR)
		{
			printf("Connect Wi-Fi failed. Try Again.\n");
		}
		else
		{
			wifiConnected = true;
			gpio_write_toggle(RED_ON_BOARD_LED); // Turn off Red LED to indicate WiFi connection is established
		}
	}

	printf("Connect to Wi-Fi success\n");

	bool mqttConnected = false;
	bool ipObtained = false;
	printf("Get IP address\n");

	struct dhcpc_state state;
	void *dhcp_handle;

	while(!ipObtained)
	{
		dhcp_handle = dhcpc_open(NET_DEVNAME);
		ret = dhcpc_request(dhcp_handle, &state);
		dhcpc_close(dhcp_handle);

		if (ret != OK)
		{
			printf("Failed to get IP address\n");
			printf("Try again\n");
			sleep(1);
		}
		else
		{
			ipObtained = true;
		}
	}
	netlib_set_ipv4addr(NET_DEVNAME, &state.ipaddr);
	netlib_set_ipv4netmask(NET_DEVNAME, &state.netmask);
	netlib_set_dripv4addr(NET_DEVNAME, &state.default_router);

	printf("IP address  %s\n", inet_ntoa(state.ipaddr));

 	// NTP
	sleep(1);
	ntp_server_conn[0].hostname = "0.pool.ntp.org";
	if (ntpc_start(ntp_server_conn, ARRAY_SIZE(ntp_server_conn), NTP_REFRESH_PERIOD, ntp_link_error) < 0)
	{
		printf("Failed to start NTP client.\n" "The date may be incorrect and lead to undefined behavior\n");
	}
	else
	{
		int num_retries = 10;

		/* Wait for the date to be set */
		while ((ntpc_get_link_status() != NTP_LINK_UP) && --num_retries)
		{
			sleep(2);
		}

		if (!num_retries)
		{
			printf("Failed to reach NTP server.\n" "The date may be incorrect and lead to undefined behavior\n");
		}
	}

	initializeConfigUtil();

	pClientHandle = mqtt_init_client(&clientConfig);

	if (pClientHandle == NULL)
	{
		printf("mqtt client handle initialization fail\n");
		return 0;
	}

	while (mqttConnected == false )
	{
		sleep(2);
		// Connect mqtt client to server
		int result = mqtt_connect(pClientHandle, SERVER_ADDR, 8883, 60);
		if (result < 0)
		{
			printf("mqtt client connect to server fail - %d\n ",result);
			continue;
		}
		mqttConnected = true;
	}

	bool mqttSubscribe = false;

	// Subscribe to topic of interest
	while (mqttSubscribe == false )
	{
		sleep(2);
		int result = mqtt_subscribe(pClientHandle,ACTION_TOPIC, 0);
		if (result < 0)
		{
			printf("mqtt client subscribe to topic failed\n");
			continue;
		}
		mqttSubscribe = true;
		printf("mqtt client Subscribed to the topic successfully\n");

		sleep(2);
		bool tempFlag = false;

		while (1)
		{
			sleep(10); // Publish message to ARTIK cloud every 10s
			char str[600];

			// Publish ADC port readings
			strcpy(str,"{\"Sensors\":{");

			struct adc_msg_s samples[ADC_MAX_SAMPLES];
			char tempBuf[20];
			float val = read_adc(A2);
			float temp = val*330/4096-50;
			sprintf(tempBuf,"\"temperature\":%.1f", temp ); strcat(str,tempBuf); strcat(str,"}}");

			printf("Published %s\n\n",str);
			if (mqtt_publish(pClientHandle, MESSAGE_TOPIC, str, sizeof(str), 0, 0) != 0)
			{
				printf("Error: mqtt_publish() failed.\n");
			}
			else
			{
				//printf("\n-Published Data------------------------------------------------\n");

			}
		}
	}
	return 0;
}
