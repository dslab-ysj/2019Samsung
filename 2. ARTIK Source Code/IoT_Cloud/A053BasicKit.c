/****************************************************************************
*
* Copyright 2018 baruncorechips All Rights Reserved.
*
* Filename: A053BasicKit.c
* Author: sj.yang
* Release date: 2018/10/04
* Version: 1.3
*
****************************************************************************/

#include "A053BasicKit.h"

/***************************************************************************
 *
 *	GPIO function
 *	- void gpio_write(int port, int value);
 *	- int gpio_read(int port);
 *
 ***************************************************************************/

void gpio_write(int port, int value)
{
	char str[4];
	static char devpath[16];
	snprintf(devpath, 16, "/dev/gpio%d", port);
	int fd = open(devpath, O_RDWR);

	ioctl(fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);
	write(fd, str, snprintf(str, 4, "%d", value != 0) + 1);

	close(fd);
}
// Write (toggle) the value of given gpio port.
void gpio_write_toggle(int port)
{
  int value = 0;
  char str[4];
  static char devpath[16];
  int curValue = gpio_read(port);
  if (curValue <= 0)
    value = 1;
  else if (curValue == 1)
    value = 0;

  snprintf(devpath, 16, "/dev/gpio%d", port);
  int fd = open(devpath, O_RDWR);

  ioctl(fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_OUT);

  write(fd, str, snprintf(str, 4, "%d", value != 0) + 1);

  close(fd);
}

int gpio_read(int port)
{
	char buf[4];
	char devpath[16];
	snprintf(devpath, 16, "/dev/gpio%d", port);
	int fd = open(devpath, O_RDWR);

	ioctl(fd, GPIOIOC_SET_DIRECTION, GPIO_DIRECTION_IN);
	read(fd, buf, sizeof(buf));
	close(fd);

	return buf[0] == '1';
}

/***************************************************************************
 *
 * PWM function
 * - int pwm_open (int port);
 * - void pwm_write(int fd, int period, int duty_cycle);
 * - void pwm_close (int fd);
 *
 ***************************************************************************/

int pwm_open (int port)
{
	int fd;

	switch (port){
		case 0 : fd = open("/dev/pwm0", O_RDWR); break;
		case 1 : fd = open("/dev/pwm1", O_RDWR); break;
		case 2 : fd = open("/dev/pwm2", O_RDWR); break;
		case 3 : fd = open("/dev/pwm3", O_RDWR); break;
		case 4 : fd = open("/dev/pwm4", O_RDWR); break;
		case 5 : fd = open("/dev/pwm5", O_RDWR); break;
		default : break;
	}
	return fd;
}

void pwm_write(int fd, int period, int duty_cycle)
{
	int frequency;
	ub16_t duty;
	struct pwm_info_s pwm_info;

	// set pwo_info parameter
	frequency = 1000000 / period;
	duty = duty_cycle * 65536 / period;
	pwm_info.frequency = frequency;
	pwm_info.duty = duty;

	ioctl(fd, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)&pwm_info));
	ioctl(fd, PWMIOC_START);
}

void pwm_close (int fd)
{
	ioctl(fd, PWMIOC_STOP);
	close(fd);
}

void ServoAngle(int fd, int PERIOD, int angle)
{
	int duty_cycle;
	duty_cycle = 1500 + angle * 8.9;
	if(duty_cycle >2300) {duty_cycle = 2300;}
	else if(duty_cycle<700) {duty_cycle = 700;}
	else {}
	pwm_write(fd, PERIOD, duty_cycle);
}

/***************************************************************************
 *
 * ADC function
 * - int read_adc(int channel);
 *
 ***************************************************************************/

int read_adc(int channel)
{
	int fd, ret;
	struct adc_msg_s sample[S5J_ADC_MAX_CHANNELS];
	int32_t data;
	size_t readsize;
	ssize_t nbytes;

	fd = open("/dev/adc0", O_RDONLY);
	if (fd < 0)
	{
		printf("%s: open failed: %d\n", __func__, errno);
		return -1;
	}

	for (;;)
	{
		ret = ioctl(fd, ANIOC_TRIGGER, 0);
		if (ret < 0)
		{
			printf("%s: ioctl failed: %d\n", __func__, errno);
			close(fd);
			return -1;
		}

		readsize = S5J_ADC_MAX_CHANNELS * sizeof(struct adc_msg_s);
		nbytes = read(fd, sample, readsize);
		if (nbytes < 0)
		{
			if (errno != EINTR) {
				printf("%s: read failed: %d\n", __func__, errno);
				close(fd);
				return -1;
			}
		}
		else if (nbytes == 0) {	printf("%s: No data read, Ignoring\n", __func__); }
		else
		{
			int nsamples = nbytes / sizeof(struct adc_msg_s);
			if (nsamples * sizeof(struct adc_msg_s) != nbytes)
			{
				printf("%s: read size=%ld is not a multiple of sample size=%d, Ignoring\n", __func__, (long)nbytes, sizeof(struct adc_msg_s));
			}
			else
			{
				int i;
				for (i = 0; i < nsamples; i++)
				{
					if (sample[i].am_channel == channel)
					{
						data = sample[i].am_data;
						//printf("Sample %d: channel: %d, value: %d\n", i + 1, sample[i].am_channel, sample[i].am_data);
						close(fd);
						return data;
					}
				}
			}
		}
	}

	return 0;
}

/***************************************************************************
 *
 * Wifi function
 * - void networkLinkUpHandler(slsi_reason_t* reason);
 *    + Handler for network link up connection event
 *    + Sets the global connection state variable and the result of the network join request.
 * - void networkLinkDownHandler(slsi_reason_t* reason);
 *    + Handler for network link down connection event
 *    + Sets the global connection variable when the access point is disconnected from the network.
 * - int8_t start_wifi_interface(void);
 *    + Starts the Wi-Fi interface and request connection to the specified network
 *    + Return: Completed successfully or failed
 *    + Starts the Wi-Fi interface in Station mode and requests to join the specified network.
 *
 ***************************************************************************/

slsi_security_config_t *getSecurityConfig(char *sec_type, char *psk, WiFi_InterFace_ID_t mode);

static int g_connection_state = STATE_DISCONNECTED;
static uint8_t g_join_result  = 0;

static sem_t g_sem_join;

void networkLinkUpHandler(slsi_reason_t* reason)
{
	g_connection_state = STATE_CONNECTED;

	g_join_result = reason->reason_code;
	sem_post(&g_sem_join);
}

void networkLinkDownHandler(slsi_reason_t* reason)
{
	g_connection_state = STATE_DISCONNECTED;

	if (reason)
	{
		printf("Disconnected from network %s "  "reason_code: %d %s\n", reason->bssid, reason->reason_code,reason->locally_generated ? "(locally_generated)": "");
	}
	else
	{
		printf("Disconnected from network\n");
	}
}

int8_t start_wifi_interface(void)
{
	if ( WiFiRegisterLinkCallback(&networkLinkUpHandler, &networkLinkDownHandler) )
	{
		return SLSI_STATUS_ERROR;
	}

	if ( WiFiStart(SLSI_WIFI_STATION_IF, NULL) == SLSI_STATUS_ERROR )
	{
		return SLSI_STATUS_ERROR;
	}

	sem_init(&g_sem_join, 0, 0);

	slsi_security_config_t *security_config = getSecurityConfig(SLSI_WIFI_SECURITY_WPA2_AES, PSK, SLSI_WIFI_STATION_IF);

	if ( WiFiNetworkJoin((uint8_t*)SSID, strlen(SSID),NULL, security_config) == SLSI_STATUS_ERROR )
	{
		return SLSI_STATUS_ERROR;
	}

	sem_wait(&g_sem_join);

	if( g_join_result )
	{
		return SLSI_STATUS_ERROR;
	}

	free(security_config);
	sem_destroy(&g_sem_join);

	return SLSI_STATUS_SUCCESS;
}
