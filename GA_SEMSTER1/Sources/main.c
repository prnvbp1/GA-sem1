/* ###################################################################
**     Filename    : main.c
**     Project     : GA_SEMSTER1
**     Processor   : MKL05Z32VLF4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2023-11-27, 20:43, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "TSS1.h"
#include "RED_LED.h"
#include "TU1.h"
#include "GREEN_LED.h"
#include "BLUE_LED.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include <stdio.h>
#include <stdlib.h>
volatile uint32_t msTicks; /* counts 1ms timeTicks */
void SysTick_Handler(void) {
 msTicks++; /* increment counter necessary in Delay() */
}
void waitms (int wait_ms) //function that causes a delay taking an integer parameter wait_ms which representing the desired delay time in milliseconds.
{
	unsigned int i, j;
	for (i = 0; i< wait_ms;i++)
	{
		for	(j=0;j<785;j++)
		{
			__asm("nop");
		}
	}

}
int ispressed = 0;
void RLED() //pwm to produce a red led
{
	RED_LED_SetRatio16(RED_LED_DeviceData,0xFFFF);
	BLUE_LED_SetRatio16(BLUE_LED_DeviceData,0x0);
	GREEN_LED_SetRatio16(GREEN_LED_DeviceData,0x0);
}
void BLED() //pwm to produce a blue led
{
	RED_LED_SetRatio16(RED_LED_DeviceData,0x0);
	BLUE_LED_SetRatio16(BLUE_LED_DeviceData,0xFFFF);
	GREEN_LED_SetRatio16(GREEN_LED_DeviceData,0x0);
}
void GLED() //pwm to produce a green led
{
	RED_LED_SetRatio16(RED_LED_DeviceData,0x0);
	BLUE_LED_SetRatio16(BLUE_LED_DeviceData,0x0);
	GREEN_LED_SetRatio16(GREEN_LED_DeviceData,0xFFFF);

}
void WLED() //pwm to produce a white led
{
		RED_LED_SetRatio16(RED_LED_DeviceData,0xFFF);
		BLUE_LED_SetRatio16(BLUE_LED_DeviceData,0xFFFF);
		GREEN_LED_SetRatio16(GREEN_LED_DeviceData,0xFFFF);
}

void OFFLED() //pwm to produce the led off
{
	RED_LED_SetRatio16(RED_LED_DeviceData,0x0);
	BLUE_LED_SetRatio16(BLUE_LED_DeviceData,0x0);
	GREEN_LED_SetRatio16(GREEN_LED_DeviceData,0x0);
}

void orange() //pwm to produce an orange led
{
	RED_LED_SetRatio16(RED_LED_DeviceData,0xFFFF);
	BLUE_LED_SetRatio16(BLUE_LED_DeviceData,0x0);
	GREEN_LED_SetRatio16(GREEN_LED_DeviceData,0xFFFF);
}
void L_BLUE() //pwm to produce a light blue led
{
	RED_LED_SetRatio16(RED_LED_DeviceData,0x000);
	BLUE_LED_SetRatio16(BLUE_LED_DeviceData,0x4000);
	GREEN_LED_SetRatio16(GREEN_LED_DeviceData,0x8000);

}

void pink() //pwm to produce a pink led
{
	RED_LED_SetRatio16(RED_LED_DeviceData,0x4000);
	BLUE_LED_SetRatio16(BLUE_LED_DeviceData,0x4000);
	GREEN_LED_SetRatio16(GREEN_LED_DeviceData,0x0);
}
int Rando(int min, int max,int seed) // to help generate a random sequence. The function Rando takes three parameters which represents the minimum and maximum value
 // and seed value used to initialise the random number generator.
{
	srand(seed);
    return (rand() % (max +1 )) + min;
}



void GenerateSeq(int seq[], int max_round,int firsttouch) // generate sequence of the LED
{
	int i;
	for (i = 1; i <= max_round; ++i)
	{
		seq[i] = Rando(0, 2,(firsttouch * i));
	}
}



void LED_OUTPUT(int num) // selects the led from the sequence based on the colours
{
	switch(num)
	{
	case 0:
		    RLED();
		    break;
	case 1:
			BLED();
			break;
	case 2:
			GLED();
			break;
	case 3:
			WLED();
			break;

	case 4:
			orange();
			break;
	case 5:
			L_BLUE();
			break;
	case 6:
		pink();
		break;
	default:
			RLED();
			break;
	}

}
flash(int num) //flashes 3 times the desired led colour which has been number coded.
//So based on the number that was input, the number is linked to a specific colour as set up in the LED_OUTPUT function
{
	int i;
	for(i =0 ; i< 4; i++)
	{
		LED_OUTPUT(num);
		waitms(500);
		OFFLED();
		waitms(500);
	}
}
flashx(int num, int times) // flashes a number of times according to the user
{
	int j;
	for (j = 0 ; j < times; j++ )
	{
		LED_OUTPUT(num);
		waitms(1000);
		OFFLED();
		waitms(1000);
	}
}
void LEDseq(int seq[], int lastnum) //display sequence on LED
{
	int i;
	for(i = 1; i <= lastnum; i++)
	{
		int num = (seq[i]);
		LED_OUTPUT(seq[i]);
		waitms(1000);
		OFFLED();
		waitms(1000);
	}
}

int promptcolour(int position) //determines a colour based on a position on the touchpad
{
 if (position > 40) // position for blue
 {
   return 2;
 }
 if (position > 22 && position < 36) // position for red
 {
   return 1;
 }
 if (position < 20) // position for green
 {
   return 3;
 }
}

int WINDEX(int pround,int level,int firsttouch) // generates a sequence for the white LEd in a sequence
{
	int index;
	srand(firsttouch);
	index = Rando(0,(pround-level),firsttouch * level);
	return index;
}

void outwhite(int seq[],int index,int pround,int level) //display the LED sequence with a flash of white LED at a specified index
{
	int x;
	int white_ver[10];
	for(x = 0 ; x < pround ; x++)
	{
		white_ver[x] = seq[x];
	}

	int j;
	for (j = 0; j < level;)
	{
		white_ver[index + j] = 3;
		j++;
	}
	LEDseq(white_ver,pround);
}

void playerguess(int index, int level, int passed,int seq[]) // function that manages the guess made by the user and takes in the
{
	passed = 2;
	int guessing = 1;
	int guess = 0;

	while (guessing == 1 ) // This loop waits for the player to make a guess.
	{
		nextguess:;
		int answer = 0;
		ispressed = 0;
		while(ispressed < 2 && answer == 0)
		{
			TSS_Task(); // TSS_Task() is called to get the input of the user from the touchpad
		}

		waitms(1000);
		answer = TSS1_cKey0.Position; // the answer will depend on the position that was triggered on the touchpad
		if (answer <= 20)
		{
			RLED;
			waitms(1000);
			OFFLED();
			if(seq[index + guess]) //If the player's guess matches the sequence (seq[index + guess] is true), the board flashes light blue
			{
				flash(5);
				if (guess != level) // If the user has not completed, the loop goes back to next guess at the beginning of the while loop
				{
					guess++;
					ispressed = 0;
					answer = 0;
					goto nextguess;

				}
				else
				{

					passed = 1;
					guessing = 0;
				}
			}
			else
			{
				flash(6); // If the player's guess doesn't match the sequence, the board flashes pink
				passed = 0;
				guessing = 0;
			}
		}
		if (answer >= 45)
		{
			BLED();
			waitms(1000);
			OFFLED();
			if(seq[index + guess])
			{
				flash(5);
				if (guess != level)
				{
					guess++;
					ispressed = 0;
					answer = 0;
					goto nextguess;
				}
				else
				{

					passed = 1;
					guessing = 0;
				}
			}
			else
			{
				flash(6);
				passed = 0;
				guessing = 0;
			}
		}
		if (answer >= 21 && answer <= 44)
		{
			GLED();
			waitms(1000);
			OFFLED();
			if(seq[index + guess])
			{
				flash(5);
				if (guess != level)
				{
					guess++;
					ispressed = 0;
					answer = 0;
					goto nextguess;
				}
				else
				{
					passed = 1;
					guessing = 0;
				}
			}
			else
			{
				flash(6);
				passed = 0;
				guessing = 0;
			}
		}
	}

}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */

int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
            PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */

  	 restart:;
  	  flash (6);

  	int max_round = 10 ;

  	int seqcont[10];
    int white;
    int p_rounds = 5;
    int game_on = 1;
    int seedgen = 0;
    int level = 1;
    int firsttouch;
    OFFLED();
Configure();

while(seedgen == 0 )
    {

    	while (ispressed<2)
    	{

    		TSS_Task();

    	}
    	firsttouch = (TSS1_cKey0.Position +25) * 1234; //when the user first touch the microcontroller the position on the touch pad will help generate a see under the variable first touch.
    	seedgen = 1;
    }


    flash(4); //flashes orange to signal the LED sequence that will be displayed


    while (game_on == 1 )
    {


    	GenerateSeq(seqcont,max_round,firsttouch);
    	OFFLED();
    	waitms(1000);

    	levelpoint:;
    	LEDseq(seqcont,p_rounds); //display the LED sequence
    	OFFLED();
    	flash(4); // flashes orange to show the end of the LED sequence display and the start of the sequence with the white led
    	int white;
    	white = WINDEX(p_rounds,level,firsttouch);
    	OFFLED();
    	waitms(1000);
    	outwhite(seqcont,white,p_rounds,level);
    	int passed = 2;
    	playerguess(white,level,passed,seqcont);
    	Configure();

    	if (passed == 1)
    	{
    		level++;
    		p_rounds++;
    		goto levelpoint;
    	}
    	else
    	{
    		goto restart;
    		OFFLED();
    		waitms(2500);
    	}


    }




  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
