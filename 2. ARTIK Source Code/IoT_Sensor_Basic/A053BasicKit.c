/****************************************************************************
*
* Copyright 2018 baruncorechips All Rights Reserved.
*
* Filename: A053BasicKit.c
* Author: sj.yang
* Release date: 2018/09/18
* Version: 1.2
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
	printf("Current value of the gpio port%d is %d\n",port,curValue);
	if (curValue <= 0) value = 1;
	else if (curValue == 1) value = 0;

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
