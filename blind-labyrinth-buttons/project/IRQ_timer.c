/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "robot.h"
#include "led/led.h"

#define	WIN_LEDS   0x2F

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern unsigned int WIN;

/* RUNNING led 5Hz -> timer che controlla il running led nel caso in cui il 
	 robot fosse bloccato */
void TIMER0_IRQHandler (void)
{
	static int i = 1;
	
	i++;
	if(i % 2 == 0)
		LED_On(5);
	else
		LED_Off(5);		
	
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

/* TIMER DIRECTION */
void TIMER1_IRQHandler (void)
{

	static char j = 1, w = 0;
	
	/* DIRECTION led */
	j++;
	if(j%2 == 0 && !WIN) {
		LED_On(robotGetLED());
	} else if (j%2 != 0 && !WIN) {
		LED_Off(robotGetLED());
	} else {  // WIN -> all leds
		w++;
		if(w%2 == 0)
			LED_Out(WIN_LEDS);
		else
			LED_Out(0);
	}
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
