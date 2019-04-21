/****************************************************************************
*
* Copyright 2018 baruncorechips All Rights Reserved.
*
* Filename: hext.h
* Author: sj.yang
* Release date: 2018/09/18
* Version: 1.2
*
****************************************************************************/
#ifndef HEXT_H_
#define HEXT_H_

int hext_main(int argc, FAR char *argv[]);

static void show_help_usage_hext(FAR char *argv[]);
static void show_help_basic_hext(FAR char *argv[]);
static void show_help_app_hext(FAR char *argv[]);

int hext_led_main(int argc, FAR char *argv[]);
int hext_switch_main(int argc, FAR char *argv[]);
int hext_light_main(int argc, FAR char *argv[]);
int hext_ir_main(int argc, FAR char *argv[]);
int hext_temp_main(int argc, FAR char *argv[]);
int hext_buzzer_main(int argc, FAR char *argv[]);
int hext_pwmled_main(int argc, FAR char *argv[]);
int hext_servo_main(int argc, FAR char *argv[]);

int hext_ledsw_main(int argc, FAR char *argv[]);
int hext_toggle_main(int argc, FAR char *argv[]);
int hext_dimming_main(int argc, FAR char *argv[]);
int hext_autotemp_main(int argc, FAR char *argv[]);

#endif /* HEXT_H_ */
