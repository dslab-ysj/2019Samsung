/****************************************************************************
*
* Copyright 2018 baruncorechips All Rights Reserved.
*
* Filename: A053BasicKit.h
* Author: sj.yang
* Release date: 2018/09/18
* Version: 1.2
*
****************************************************************************/

#ifndef A053BASICKIT_H_
#define A053BASICKIT_H_

// HexT GPIO Pin number
#define PIN_D2 46
#define PIN_D4 47
#define PIN_D7 48
#define PIN_D8 50

// HexT ADC Pin number
#define A0 0
#define A1 1
#define A2 2
#define A3 3

// HexT PWM Pin number
#define PWM0 0
#define PWM1 1
#define PWM2 2
#define PWM4 4

// octave_chord period[us] // frequency[hz]
#define O4_DO 3822 // 261.63[hz]
#define O4_RE 3405 // 293.66[hz]
#define O4_MI 3034 // 329.63[hz]
#define O4_FA 2863 // 349.23[hz]
#define O4_SO 2551 // 392.00[hz]
#define O4_LA 2273 // 440.00[hz]
#define O4_TI 2025 // 493.88[hz]

#define O5_DO 1911 // 523.251[hz]
#define O5_RE 1703 // 587.330[hz]
#define O5_MI 1517 // 659.254[hz]
#define O5_FA 1432 // 698.456[hz]
#define O5_SO 1276 // 783.990[hz]
#define O5_LA 1136 // 880.000[hz]
#define O5_LA_S 1073 // 932.33[hz]
#define O5_TI 1012 // 987.766[hz]

#define O6_DO 956 // 1046.502[hz]
#define O6_DO_S 902 // 1108.73[hz]
#define O6_RE 851 // 1174.659[hz]
#define O6_RE_S 804 // 1244.51[hz]
#define O6_MI 758 // 1318.510[hz]
#define O6_FA 716 // 1396.913[hz]
#define O6_FA_S 676 // 1479.98[hz]
#define O6_SO 638 // 1567.982[hz]
#define O6_SO_S 602
#define O6_LA 568 // 1760.000[hz]
#define O6_TI 506 // 1975.533[hz]

#define O7_DO 478 // 2093.00[hz]
#define O7_RE 426 // 2349.32[hz]
#define O7_MI 379 // 2637.02[hz]
#define O7_FA 358 // 2793.82[hz]
#define O7_SO 319 // 3135.96[hz]
#define O7_LA 284 // 3520.00[hz]
#define O7_TI 253 // 3951.07[hz]


#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <tinyara/gpio.h>	// for GPIO control
#include <tinyara/pwm.h>	// for PWM control
#include <tinyara/analog/adc.h>	// for ADC control
#include <tinyara/analog/ioctl.h>	// for ADC control
#include <errno.h>	// for ADC control

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

#endif /* A053BASICKIT_H_ */
