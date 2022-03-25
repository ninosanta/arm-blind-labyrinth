/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {
	/* i.e. solo una porta del joystick abilitata */
  LPC_PINCON->PINSEL3 &= ~(3<<18);	/* PIN mode GPIO (00b value per P1.25)
																		 * -> ci focalizziamo sul pulsante di pressione del 
																		 * 		joystick i.e. il 25esimo
																		 * 		-> tale pin #25 all'inizializzazione deve essere 
																		 *			 settato in modalita' GPIO altrimenti non posso
																		 * 			 leggere il valore del Joystick */
	LPC_GPIO1->FIODIR   &= ~(1<<25);	// P1.25 Input (joysticks on PORT1 defined as Input)
}
