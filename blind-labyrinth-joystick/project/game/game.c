#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../joystick/joystick.h"
#include "../RIT/RIT.h"
#include "../robot/robot.h"
#include "../draw/draw.h"

signed char MOVE;
extern _robot robot;
extern char WIN;
extern int PLAY;  // definita in IRQ_timer.c

void gameOpening(void) {
	MOVE = -1;  // gioco inizializzato in "stallo"
	WIN = 0;	
		
	LCD_Initialization();
	TP_Init();
  TouchPanel_Calibrate();
	LCD_Clear(Black);	
	drawLabyrinth();
  startSencente();
	
  newTimerVal(0, 0x98968);							/* timer 0, 25ms = 25*10^-3*25*10^6 = 0x98968 */
	robot_init();
	obstaclesInit();
}
 

void gameInit(void) {
	MOVE = 1;  // modalita' move ON
  clearStartSentence();
	drawRobot(robot.dir, robot.coord.x, robot.coord.y);
	joystick_init();											/* Joystick Initialization            */
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	enable_RIT();													/* il RIT, a differenza del progetto con i pulsanti
																				 * qui viene abilitato immediatamente e inizia a 
																				 * farci finire nel suo handler ad intervalli 
																				 * regolari di 50ms */
}


void gameClear(void) {
	clearObstacles();
	proximity();
}


void gameRestart(void) {
	MOVE = -1; 
	WIN = 0;
	disable_RIT();
	reset_RIT();
	
	LCD_Clear(Black);	
	drawLabyrinth();
	clearObstacles();
  startSencente();
	PLAY = 0;
	
	robot_init();
}

