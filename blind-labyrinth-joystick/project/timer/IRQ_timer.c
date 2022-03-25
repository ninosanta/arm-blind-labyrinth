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
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../game/game.h"
#include "../robot/robot.h"
#include "../draw/draw.h"


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

#define		T_PRESSION 5


extern signed char MOVE;
extern char WIN;
int PLAY = 0;



void TIMER0_IRQHandler (void)
{
	static int clear = 0;
	static int restart = 0;
	
	
	/* TOUCH */
  getDisplayPoint(&display, Read_Ads7846(), &matrix);
	
	if(display.x <= 240 && display.x > 0 && display.y <= 320 && display.y > 0) {
		/* abbiamo cliccato dentro al display */
		
		
		/* PLAY */
		if (PLAY < T_PRESSION && !WIN && display.y >= START_LAB_ROW && display.x >= START_LAB_COL 
				&& display.y <= (START_LAB_ROW + LAB_R) && display.x <= (START_LAB_COL + LAB_C)) {
				/* abbiamo cliccato dentro al labirinto */
			PLAY++;
					
			if(PLAY == T_PRESSION) {	/* 1/8 seconds = 5 times * 25 ms*/
				gameInit();
			}
		}

		
		/* CLEAR */
		if(!WIN && PLAY >= T_PRESSION &&
			 display.y >= START_RES_BUTT_ROW && display.x >= START_RES_BUTT_COL + RES_BUT_C + SQUARE*3 &&
			 display.y <= START_RES_BUTT_ROW+RES_BUT_R && display.x <= START_RES_BUTT_COL + RES_BUT_C + SQUARE*3 + RES_BUT_C)  
		{  // se abbiamo toccato il tasto clear
			clear++; 
			
			if(clear == T_PRESSION) {	/* 1/8 seconds = 5 times * 25 ms*/
				// puliamo e riscriviamo
				gameClear();
				clear = 0;
			}
		}
		
		
		/* RESET */
		if(PLAY >= T_PRESSION &&
			 display.y >= START_RES_BUTT_ROW && display.x >= START_RES_BUTT_COL &&
			 display.y <= START_RES_BUTT_ROW+RES_BUT_R && display.x <= START_RES_BUTT_COL + RES_BUT_C)  
		{  // se abbiamo toccato il tasto clear
			restart++;
			
			if(restart == T_PRESSION) {	/* 1/8 seconds = 5 times * 25 ms*/
				// puliamo e riscriviamo
				gameRestart();
				restart = 0;
			}
		}
	
	} else {
		//do nothing if touch returns values out of bounds
	}
			
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

	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
