#include "button.h"
#include "lpc17xx.h"
#include "../robot/robot.h"

//	#include "../led/led.h" 					/* do not needed anymore, make your project clean */
#include "../RIT/RIT.h"		  			/* you now need RIT library 			 */

char key = 0;
extern unsigned int WIN;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{	
	static char LOCK = 0;
  if (!LOCK) {  // once only untill RESET
		init_RIT(0x004C4B40);		 		/* RIT Initialization 50 msec       */
		robot_init();
		WIN = 0;
		LOCK = 1;
	}
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	if (!WIN) {
		key = 1;
		enable_RIT();
	}
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	if (!WIN) {
		key = 2;
		enable_RIT();										/* enable RIT to count 50ms				 */
	}
	NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	
	LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */   		
}


