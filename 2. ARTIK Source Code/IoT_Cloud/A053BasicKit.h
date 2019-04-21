/****************************************************************************
*
* Copyright 2018 baruncorechips All Rights Reserved.
*
* Filename: A053BasicKit.h
* Author: sj.yang
* Release date: 2018/10/04
* Version: 1.3
*
****************************************************************************/

#ifndef A053BASICKIT_H_
#define A053BASICKIT_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <fcntl.h>
#include <tinyara/config.h>
#include <tinyara/gpio.h>	// for GPIO control
#include <tinyara/pwm.h>	// for PWM control
#include <tinyara/analog/adc.h>	// for ADC control
#include <tinyara/analog/ioctl.h>	// for ADC control
#include <errno.h>	// for ADC control
#include <net/if.h>
#include <apps/netutils/wifi/slsi_wifi_api.h>

#include <apps/netutils/dhcpc.h>
#include <apps/netutils/mqtt_api.h>
#include <apps/netutils/ntpclient.h>

// WIFi
#define STATE_DISCONNECTED      0
#define STATE_CONNECTED       1
#define SLSI_WIFI_SECURITY_OPEN   "open"
#define SLSI_WIFI_SECURITY_WPA2_AES "wpa2_aes"
#define SSID "B_CORECHIPS" //FIX
#define PSK  "bcore00151" //FIX

// Hexagon GPIO Pin number
#define PIN_D2 46
#define PIN_D4 47
#define PIN_D7 48
#define PIN_D8 50

// Hexagon ADC Pin number
#define A0 0
#define A1 1
#define A2 2
#define A3 3

// Hexagon PWM Pin number
#define PWM0 0
#define PWM1 1
#define PWM2 2
#define PWM4 4

// octave_chord period[us] // frequency[hz]
#define O5_DO 1911 // 523.251[hz]
#define O5_RE 1703 // 587.330[hz]
#define O5_MI 1517 // 659.254[hz]
#define O5_FA 1432 // 698.456[hz]
#define O5_SO 1276 // 783.990[hz]
#define O5_LA 1136 // 880.000[hz]
#define O5_TI 1012 // 987.766[hz]

#define O6_DO 956 // 1046.502[hz]
#define O6_RE 851 // 1174.659[hz]
#define O6_MI 758 // 1318.510[hz]
#define O6_FA 716 // 1396.913[hz]
#define O6_SO 638 // 1567.982[hz]
#define O6_LA 568 // 1760.000[hz]
#define O6_TI 506 // 1975.533[hz]

#define	HIGH	1
#define	LOW		0
#define ENABLE	1
#define DISABLE	0
#define S5J_ADC_MAX_CHANNELS	4


//GPIO function
void gpio_write(int port, int value);
void gpio_write_toggle(int port);
int gpio_read(int port);

//PWM function
int pwm_open (int port);
void pwm_write(int fd, int period, int duty_cycle);
void pwm_close (int fd);
void ServoAngle(int fd, int PERIOD, int angle);

//ADC function
int read_adc(int channel);

//Wifi function
void networkLinkUpHandler(slsi_reason_t* reason);
void networkLinkDownHandler(slsi_reason_t* reason);
int8_t start_wifi_interface(void);

//mqtt function
#endif /* A053BASICKIT_H_ */
