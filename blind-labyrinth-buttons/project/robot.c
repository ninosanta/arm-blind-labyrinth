#include "led/led.h"
#include "robot.h"
#include "timer.h"
#include "RIT/RIT.h"


#define		R			13
#define 	C			15
#define MAX_DIST 5
#define LEDN	3  // 08 - N 
#define LEDE	2  // 09 - E
#define LEDS	1  // 10 - S
#define	LEDO	0  // 11 - O


/* Labirinto */
char const labyrinth[R][C] = { {2,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
															 {0,0,1,1,1,1,0,0,0,0,0,0,0,0,0},
															 {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
															 {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
															 {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
															 {1,1,1,1,1,0,0,1,0,1,0,0,0,1,0},
															 {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
															 {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
															 {1,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
															 {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
															 {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
															 {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
															 {2,0,1,0,0,0,0,0,0,0,0,0,0,0,2} };

															 
typedef struct {
	signed char x, y;
} _coord;


typedef struct {
	_coord coord;
	char dir;	 		// direzione verso cui punta il robot
	char led;   	// led rappresentante la direzione puntata
} _robot;

_robot robot;


typedef struct {
	_coord N, S, O, E;
} _cardinal;

/* varibile che utilizzero' per muovermi all'interno della matrice 
 * lungo uno dei 4 punti cardinali.
 * e.g. per muoverci verso OVEST sommiamo NULLA all'indice di riga
 *      i.e. 0. restando sempre sulla stessa riga appunto, e -1 
 *      all'indice di colonna muovendoci verso "sx" i.e. ovest 
 * Vedere funzioni proximity() e step() */
_cardinal nsoe = { {-1, 0},    // N
									 { 1, 0},    // S
									 { 0,-1},    // O 
									 { 0, 1} };  // E															 


char fermo = 0;		/* serve a non far risovrascrivere il timer0 
									 * quando il robot si blocca e RUNNING deve 
									 * lampeggiare a 5 Hz
									 * -> si usa in step() e nel RIT */															 
															 

unsigned int WIN = 1;  // per EINT0
									 
									 
void robot_set_coord(char a, char b) {
	robot.coord.x = a;
	robot.coord.y = b;
}


char checkVal(char a, char b) {
	return labyrinth[a][b];
}


void robot_init(void) {
	robot_set_coord(7, 7);  // posizione inziale del robot
	robot_set_dir('E');			// all'inizio punta a est
}


void robot_set_dir(char dir) {
	LED_Off(robotGetLED());  // ci pensera' proximity()
	
	switch(dir) {
		case('N'):
			robot.dir = 'N';
			robot.led = LEDN;
			break;
		case('S'):
			robot.dir = 'S';
			robot.led = LEDS;
			break;
		case('E'):
			robot.dir = 'E';
			robot.led = LEDE;
			break;
		case('O'):
			robot.dir = 'O';
			robot.led = LEDO;
			break;
		default:
			break;
	}
	
	proximity();
}


void proximity(void) {
	signed char i,j, dist;
	signed char dx, dy;
	
	switch(robot.dir) {
		case('N'):
			dx = nsoe.N.x;
			dy = nsoe.N.y;
			break;
		case('S'):
			dx = nsoe.S.x;
			dy = nsoe.S.y;
			break;
		case('E'):
			dx = nsoe.E.x;
			dy = nsoe.E.y;
			break;
		case('O'):
			dx = nsoe.O.x;
			dy = nsoe.O.y;
			break;
		default:
			break;
	}
	
	i = robot.coord.x;
	j = robot.coord.y;
	dist = 0;
	do { 
		(dx != 0) ? (i += dx) : (j += dy);  // una cella avanti nella giusta direzione
		/* siamo nella matrice ? */
		if (i < R && j < C && i >= 0 && j >= 0) {
			if (labyrinth[i][j] == 2) {
				/* i.e. punta verso l'uscita i.e. VITTORIA */
				break;
			} else if (labyrinth[i][j] == 1) { 
				/* OSTACOLO beccato */
				break;
			} else {  // 0
				dist++;
			}
		} else {  // abbiamo sforato
				(dx != 0) ? i-- : j--;  // dietrofront per l'indice corretto (per l'if su lab)
				break;
		}
	} while (dist <= MAX_DIST);
	
	/* LEDs DI DIREZIONE */
	if (labyrinth[i][j] != 2) {  
		if(dist > MAX_DIST) {
			/* non deve lampeggiare ma stare fisso */
			LED_On(robotGetLED());
			disable_timer(1);
			reset_timer(1);
		} else if(dist <= MAX_DIST && dist >= 3) {
			newTimerVal(1, 0x5F5E10);  // 2Hz: 0.25s, timer1
		} else if ( dist == 2  || dist == 1 ) {
			newTimerVal(1, 0x2FAF08);  // 4Hz: 0.125s, timer1
		}	else {  // dist 0
			newTimerVal(1, 0x17D784);  // 8Hz: 0,0625s, timer1
		}
	} else {
		/* se punta verso l'uscita non deve lampeggiare ma restare fisso */
		LED_On(robotGetLED());
	}
}


void rotate(void) {
	switch(robot.dir) {
		case('N'):
			robot.dir = 'E';
			break;
		case('S'):
			robot.dir = 'O';
			break;
		case('E'):
			robot.dir = 'S';
			break;
		case('O'):
			robot.dir = 'N';
			break;
		default:
			break;
	}
	
	robot_set_dir(robot.dir);
}


void newTimerVal(char timerN, unsigned int timerV) {
	disable_timer(timerN);
	reset_timer(timerN);
	init_timer(timerN, timerV);
	enable_timer(timerN);
}


void step(void) {
	signed char x, y, dx, dy;
	char val;
	
	switch(robot.dir) {
		case('N'):
			dx = nsoe.N.x;
			dy = nsoe.N.y;
			break;
		case('S'):
			dx = nsoe.S.x;
			dy = nsoe.S.y;
			break;
		case('E'):
			dx = nsoe.E.x;
			dy = nsoe.E.y;
			break;
		case('O'):
			dx = nsoe.O.x;
			dy = nsoe.O.y;
			break;
		default:
			break;
	}
	
	/* VEDIAMO COSA SUCCEDE SE CI MUOVIAMO DI UN PASSO */
	x = robot.coord.x;
	x += dx;
	y = robot.coord.y;
	y += dy;
	fermo = 0;
	
	/*  se finiamo oltre il bordo -> occorre stia fermo: RUNNING 5Hz */
	if(x == R || y == C || x == -1 || y == -1) {
    newTimerVal(0,0x2625A0);  // 0.1s, timer0
		/* e' fermo */
		fermo = 1;
	}
	/* dentro la matrice -> becca ostacolo: RUNNING 5Hz
											 -> movimeno concesso: RUNNING 1Hz 
											 -> vittoria */
	if (x < R && y < C && x >= 0 && y >= 0) {
		val = checkVal(x,y);
		
		if (val == 1) {
			newTimerVal(0,0x002625A0);  // 0.1s, timer0
			/* e' fermo */
			fermo = 1;
		}
		
		if (val == 0) {
			robot_set_coord(x, y);
			// LED a 1Hz: 0.5s, lo faccio dentro al RIT
      proximity();
		}
		
		if (val == 2) {  /* VITTORIA */
			/* 1 Hz all */
			robot_set_coord(x, y);
			WIN = 1;
			disable_timer(0);
			reset_timer(0);
			disable_RIT();
			reset_RIT();
			newTimerVal(1,0xBEBC20);
			fermo = 1;
		}
	}

}


unsigned int robotGetLED(void) {
	return robot.led;
}

