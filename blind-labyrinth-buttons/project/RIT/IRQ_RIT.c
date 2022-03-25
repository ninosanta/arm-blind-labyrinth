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
#include "../led/led.h"
#include "../robot.h"
#include "../timer.h"


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

void RIT_IRQHandler (void)
{	
	static unsigned int pressed = 0;
	static char i = 0;
	static unsigned int down = 0;
	static char rel = 0;
	
	/* KEY1 PREMUTO/RILASCIATO */
	if(key == 1) { 
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0) {  // controllo se il pin 11 (KEY1) e' attivo
			down++;
			reset_RIT();  // reset del contatore quando abbiamo la conferma che
										// il pulsante e' stato premuto
			switch(down) {
				case(1):
					rel = 1;
					break;
				default:
					break;
			}
		} else {	/* button released */
			disable_RIT();  
			reset_RIT();
			
			if (rel == 1) {
				rotate();  // ROTAZIONE AL RILASCIO
				rel = 0;
				pressed = 0;
			}
			down = 0;
			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection 
																								* -> ripristiniamo il pin alla funzionalita'
																								* 	 di interrupt */
		}
	}
	
	/* KEY2 PREMUTO/RILASCIATO */
	if(key == 2) {
		// variabile che tiene conto di quante volte premiamo il pulsante KEY2
		pressed++;  
						 
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0) {  // controllo se il pin 12 (KEY2) e' attivo
			reset_RIT();  // reset del contatore quando abbiamo la conferma che  il pulsante e' stato premuto
			
			/* 20*50ms -> 1 sec e' passato */
			if (pressed%20 == 0  && fermo == 0) { 
				step();  // tentativo di movimento di un passo
			}
			
			/* per il blinking a 5Hz ci pensa step() con timer0 */
			
			/* 10*50ms -> 0.5 sec -> RUNNING led 1 Hz */
			if (pressed%10 == 0 && fermo == 0) {  
				if(i%2 == 0)   // inizio con led OFF cosi' se deve illuminarsi lo fa nei multipli di 20
					LED_Off(5);
				else
					LED_On(5);
				i++;
			}
			
		}
		else {	/* button released */
			// come nell'if ma vice versa:
			fermo = 0;
			disable_timer(0);  	/* se fosse il robot fosse stato bloccato, sarebbe partito il timer0 in step() */
			reset_timer(0);
			LED_Off(5);  				/* KEY2 potrebbe infatti essere rilasciato
													 * mentre questo led e' acceso */
			i = 0;
//		pressed = 0;  			/* azzerandolo, ogni volta che si preme KEY2 e dopo lo si rilascia
//												 * il conteggio del secondo alla successiva pressione il conteggio  
//												 * ripartirebbe da 0 senza contare il tempo di eventuali "micro" pressioni 
//												 * precedenti e minori di un secondo. 
//												 * Non azzerandolo, se ad esempio il KEY2 viene premuto per mezzo secondo 
//												 * e poi rilasciato, alla pressione successiva di un altro mezzo secondo 
//												 * ci sara' il tentativo di muovere il robot

			disable_RIT();  
			reset_RIT();

			NVIC_EnableIRQ(EINT2_IRQn);							 /* disable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection 
																								* -> ripristiniamo il pin alla funzionalita'
																								* 	 di interrupt */
		}
			
	}
  
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
