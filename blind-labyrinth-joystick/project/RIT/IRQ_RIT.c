/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../draw/draw.h"
#include "../timer/timer.h"


/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern char key;
extern char fermo;
extern _robot robot;

extern signed char MOVE;  /* serve per capire se siamo in modalita' EXPLORE o MOVE */
extern char WIN;				 /* serve per capire se la partita e' giunta a conclusione */

void RIT_IRQHandler (void)
{	
	/* variabili joystick */
	static unsigned int up = 0;
	static unsigned int right = 0;
	static unsigned int down = 0;
	static unsigned int left = 0;
	static unsigned int select = 0;
	unsigned int PINS = 0;
	unsigned int N;
	int pin;

	/* JOYSTICK: e' gia' debounced ma non ha meccanismi di interrupt quindi 
	 * occorre fare polling con il RIT per verificare le pressioni.
   * Inoltre il select del joystick non e' debounced e, infine, piu' tasti
	 * direzionali possono essere premuti contemporaneamente */
	
	
	/* SELECT */ 
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0 && !WIN) {	 
		/* Joytick Select pressed */
		select++;
		
		switch(select) {
			case 1:
				if (MOVE == 1) {
					MOVE = 0;  // passiamo alla modalita' ROTATE
					drawRobot(robot.dir, robot.coord.x, robot.coord.y);
				} else if (MOVE == 0) {
					MOVE = 1;  // passiamo alla modalita' MOVE
					drawRobot(robot.dir, robot.coord.x, robot.coord.y);
				}
				break;
			default: 
				
				break;
		}
	} else {  /* SELECT rilasciato */
			select = 0;
	}
	
	
	/* VERIFICO QUANTI TASTI DIREZIONALI SONO PREMUTI CONTEMPORANEAMENTE */
	for (N = 26; N < 30; N++) {
		pin = (LPC_GPIO1->FIOPIN & (1<<N));
		pin >>= N;
		PINS += pin;
	}
	
	
	if( !WIN && PINS == 3 ) 
	{  /* se non abbiamo vinto e solo un tasto e' premuto */
		
		/* JOYSTICK DOWN */
		if((LPC_GPIO1->FIOPIN & (1<<26)) == 0) {
			down++;

			if (MOVE == 1 && down % 20 == 0) {  // MOVE 1 step al secondo 50ms*20
				if (robot.dir != 'S') {
					robot_set_dir('S');
					step();
					fermo = 0;
				} else if (robot.dir == 'S' && !fermo) {
					step();
				} else { }
			} else if (MOVE == 0) {  // EXPLORE
				if (robot.dir != 'S') {
					robot_set_dir('S');
					fermo = 0;
				}
			}
			
		} else {
			down = 0;
		}
		
		
		/* JOYSTICK LEFT */
		if((LPC_GPIO1->FIOPIN & (1<<27)) == 0) {
			left++;
		
			if (MOVE == 1 && left % 20 == 0) {
				if (robot.dir != 'O') {
					robot_set_dir('O');
					step();
					fermo = 0;
				} else if (robot.dir == 'O' && !fermo) {
					step();
				} else { }
			} else if (MOVE == 0) {
				if (robot.dir != 'O') {
					robot_set_dir('O');
					fermo = 0;
				}
			}	
		} else {
			left = 0;
		}
		
		
		/* JOYSTICK RIGHT */
		if((LPC_GPIO1->FIOPIN & (1<<28)) == 0) {
			right++;
			
			if (MOVE == 1 && right % 20 == 0) {
				if (robot.dir != 'E') {
					robot_set_dir('E');
					step();
					fermo = 0;
				} else if (robot.dir == 'E' && !fermo) {
					step();
				} else { }
			} else if (MOVE == 0) {  // EXPLORE
				if (robot.dir != 'E') {
					robot_set_dir('E');
					fermo = 0;
				}
			} 
			
		}	else {
			right = 0;
		}
		
		
		/* JOYSTICK UP */
		if((LPC_GPIO1->FIOPIN & (1<<29)) == 0) {
			up++;
			
			if (MOVE == 1 && up % 20 == 0) {
				if (robot.dir != 'N') {  // robot vuol muoversi lungo N partendo da un'altra direzione
					robot_set_dir('N');
					step();
					fermo = 0;
				} else if (robot.dir == 'N' && !fermo) {
					step();
				} else { }
			} else if (MOVE == 0) {
				if (robot.dir != 'N') {
					robot_set_dir('N');
					fermo = 0;
				}
			}
			
		} else {
			up = 0;
		}
		
	} else {
		// piu' tasti premuti
		up = 0;
		down = 0;
		right = 0;
		left = 0;
	}
	
	
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
