#include <apps/shell/tash.h>
#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "game.h"
#include "A053BasicKit.h"
#include "mario.h"

/*************************************************
 * functions for games
 ************************************************/

void pwm_func(int gpio_pin, int period, int duty_cycle, char* name)
{
	int PWM_PIN = PWM0;
	int fd = pwm_open(PWM_PIN);
	printf("%s\n", name);
	pwm_write(fd,period,duty_cycle);
	while(gpio_read(gpio_pin)){}   
	pwm_close(fd);
	return;
}



int binary_main(int argc, FAR char *argv[])
{
   int sw_left = PIN_D2;
   int sw_right = PIN_D4;
   int OOO = PIN_D7;
   int XXX = PIN_D8;

   while(1)
   {
      gpio_write(OOO, gpio_read(sw_left));
      gpio_write(XXX, gpio_read(sw_right));

      // Exit condition
      if((gpio_read(sw_right)==1) && (gpio_read(sw_left)==1))
      {
		gpio_write(OOO, LOW);
		gpio_write(XXX, LOW);
		break;
      }

   }
   return 0;
}

int mp3_main(int argc, FAR char *argv[])
{
	int play_sw = PIN_D2;
	int stop_sw = PIN_D4;

	int play_val = 0;

	int buzzer_pin = PWM0;

	int i;

	int size = sizeof(mario_song) / sizeof(int);

	printf("==============================================\n");
	printf("                                              \n");
	printf("                 MP3 PLAYER                   \n");
	printf("               (SuperMario BGM)               \n");
	printf("==============================================\n");
	printf("                                              \n");
	printf("  Left switch : play | Right switch : stop    \n");



	while(1)
	{
		play_val = gpio_read(play_sw);
		if(play_val == HIGH)
		{
			printf("                                              \n");
			printf("                    PLAY!                     \n");
			printf("                                              \n");

			for(i=0; i<size;i++)
			{
				muse(mario_song[i],mario_beat[i],buzzer_pin);
				if (gpio_read(stop_sw)==1) break;
			}
			break;
		}

	}




	printf("==============================================\n");
	printf("                                              \n");
	printf("                End of player.                \n");
	printf("                                              \n");
	printf("==============================================\n");

	return 0;
}


int piano_main(int argc, FAR char *argv[])
{
   int duty_cycle = 100;

   printf("============= Play song =============\n");

   int sw_DO = PIN_D2;
   int sw_RE = PIN_D4;
   int sw_MI = PIN_D7;
   int sw_SO = PIN_D8;

   int sw_DO_val = 0;
   int sw_RE_val = 0;
   int sw_MI_val = 0;
   int sw_SO_val = 0;

   //switch 처음 눌림 감지
   while(1)
   {
	  sw_DO_val = gpio_read(sw_DO);
	  sw_RE_val = gpio_read(sw_RE);
	  sw_MI_val = gpio_read(sw_MI);
	  sw_SO_val = gpio_read(sw_SO);

	  if((sw_DO_val==1) && (sw_MI_val==1)) break; // Exit condition
	  else if(sw_DO_val == 1) pwm_func(sw_DO, O6_DO, duty_cycle, "DO");
      else if(sw_RE_val == 1) pwm_func(sw_RE, O6_RE, duty_cycle, "RE");
      else if(sw_MI_val == 1) pwm_func(sw_MI, O6_MI, duty_cycle, "MI");
      else if(sw_SO_val == 1) pwm_func(sw_SO, O6_FA, duty_cycle, "SO");
      else {}
   }

   return 0;
}


int ruler_main(int argc, FAR char *argv[])
{
	int32_t ir_val;
	int sw_stop = PIN_D2;
	int pin_ir = A0;

	while(1)
	{
		ir_val = read_adc(pin_ir);

		if(ir_val>850) printf("Distance : 0cm (%d)\n",ir_val);
		else if(ir_val>500) printf("Distance : 1cm (%d)\n",ir_val);
		else if(ir_val>300) printf("Distance : 2cm (%d)\n",ir_val);
		else if(ir_val>200) printf("Distance : 3cm (%d)\n",ir_val);
		else if(ir_val>170) printf("Distance : 4cm (%d)\n",ir_val);
		else if(ir_val>150) printf("Distance : 5cm (%d)\n",ir_val);
		else if(ir_val>130) printf("Distance : 6cm (%d)\n",ir_val);
		else printf("out of range \n");

		if(gpio_read(sw_stop)==1) break;  // Exit condition

		up_mdelay(1000);
	}
	return 0;
}

int inst2_main(int argc, FAR char *argv[])
{
	int low_tone_sw  = 0;
	int high_tone_sw = 0;
	int pin_ir = 0;
	int pin_buzzer = 0;
	int val_ir = 0;
	int fd_buzzer = 0;
	int TONE = 0;
	int Duty_Cycle = 100;
	int TONE_var = 0;
	int count;

	low_tone_sw  = PIN_D2; // GPIO 0 PORT
	high_tone_sw = PIN_D4;

	pin_ir = A0; // ADC 0 PORT
	pin_buzzer = PWM0; // PWM 0 PORT


	while(1)
	{
		count = 0;

		val_ir = read_adc(pin_ir);

		if(gpio_read(high_tone_sw)) {
		      if(val_ir>550)
		    	  TONE = E7;
		      else if(val_ir>250)
		    	  TONE = D7;
		      else if(val_ir>140)
		    	  TONE = C7;
		      else if(val_ir>105)
		    	  TONE = B6;
		      else
		    	  TONE = A6;
		} else {
		      if(val_ir>550)
		    	  TONE = C6;
		      else if(val_ir>250)
		    	  TONE = D6;
		      else if(val_ir>140)
		    	  TONE = E6;
		      else if(val_ir>105)
		    	  TONE = F6;
		      else
		    	  TONE = G6;
		}

		while(gpio_read(low_tone_sw) && gpio_read(high_tone_sw)){
			count++;
			up_mdelay(1);
			if(count == 3000)
				goto B;
		}

		if(gpio_read(low_tone_sw)){

			fd_buzzer = pwm_open(pin_buzzer);

			pwm_write(fd_buzzer,TONE,100);

			TONE_var = TONE;

			while(TONE==TONE_var && gpio_read(low_tone_sw) && gpio_read(high_tone_sw) == 0){

				val_ir = read_adc(pin_ir);


					  if(val_ir>550)
						  TONE_var = C6;
					  else if(val_ir>250)
						  TONE_var = D6;
					  else if(val_ir>140)
						  TONE_var = E6;
					  else if(val_ir>105)
						  TONE_var = F6;
					  else
						  TONE_var = G6;
			}
				pwm_close(fd_buzzer);
		} else if(gpio_read(high_tone_sw)){

			fd_buzzer = pwm_open(pin_buzzer);

			pwm_write(fd_buzzer,TONE,100);

			TONE_var = TONE;

			while(TONE==TONE_var && gpio_read(high_tone_sw) && gpio_read(low_tone_sw) == 0){

				val_ir = read_adc(pin_ir);

					  if(val_ir>550)
						  TONE_var = E7;
					  else if(val_ir>250)
						  TONE_var = D7;
					  else if(val_ir>140)
						  TONE_var = C7;
					  else if(val_ir>105)
						  TONE_var = B6;
					  else
						  TONE_var = A6;

			}
				pwm_close(fd_buzzer);
		} else
			while(gpio_read(low_tone_sw) == 0 && gpio_read(high_tone_sw) == 0);
	}

	B:

	printf("Exit Program.\n");

	return 0;
}


int inst3_main(int argc, FAR char *argv[]){

	int pin_buzzer = 0;
	int SW_low_tone = 0;
	int SW_high_tone = 0;
	int SW_exit1 = 0;
	int SW_exit2 = 0;
	int sound_low = 0;
	int sound_high = 0;
	int fd_buzzer = 0;
	int count;

	pin_buzzer = PWM0;
	SW_low_tone = PIN_D2;
	SW_high_tone = PIN_D4;

	if(atoi(argv[2]) == 1){
		sound_low = C6;
		sound_high = G6;
		printf("C,Gn");
	} else if(atoi(argv[2]) == 2){
		sound_low = D6;
		sound_high = A6;
		printf("D,A\n");
	} else if(atoi(argv[2]) == 3){
		sound_low = E6;
		sound_high = B6;
		printf("E,B\n");
	} else {
		sound_low = F6;
		sound_high = C7;
		printf("F,C\n");
	}

	while(1){

		count = 0;

		if(gpio_read(SW_low_tone)){

			fd_buzzer = pwm_open(pin_buzzer);

			pwm_write(fd_buzzer,sound_low,100);
			printf("%d\n",sound_low);

			while(gpio_read(SW_low_tone) && gpio_read(SW_high_tone) == 0);
		} else if(gpio_read(SW_high_tone)){

			fd_buzzer = pwm_open(pin_buzzer);

			pwm_write(fd_buzzer,sound_high,100);
			printf("%d\n",sound_high);

			while(gpio_read(SW_high_tone) && gpio_read(SW_low_tone) == 0);
		}

		pwm_close(fd_buzzer);

		while(gpio_read(46) && gpio_read(47)){
			count++;
			up_mdelay(1);
			if(count == 3000){
				goto A;
			}
		}

	}

	A:

	printf("Exit Program.\n");

	return 0;

}

static void show_game_hexagon(FAR const char *program)
{
   printf("|-------------------------------------------|\n");
   printf("| ******* ARTIK053 BASIC KIT GAMES ******** |\n");
   printf("|               %s binary                 |\n", program);
   printf("|              %s musicbox                |\n", program);
   printf("|             %s instrument1              |\n", program);
   printf("|               %s ruler                  |\n", program);
   printf("|             %s instrument2              |\n", program);
   printf("|             %s instrument3 (1,2,3,4)    |\n", program);
   printf("|-------------------------------------------|\n");

}


int game_main(int argc, FAR char *argv[])
{

   if ( (argc==2) && (strcmp(argv[1],"binary")==0) ) {binary_main(argc,argv);}
   else if ( (argc==2) && (strcmp(argv[1],"musicbox")==0) ) {mp3_main(argc,argv);}
   else if ( (argc==2) && (strcmp(argv[1],"instrument1")==0) ) {piano_main(argc,argv);}
   else if ( (argc==2) && (strcmp(argv[1],"ruler")==0) ) {ruler_main(argc,argv);}
   else if ( (argc==2) && (strcmp(argv[1],"instrument2")==0) ) {inst2_main(argc,argv);}
   else if ( (argc==3) && (strcmp(argv[1],"instrument3")==0) ) {inst3_main(argc,argv);}
   else show_game_hexagon(argv[0]);

   return 0;
}







