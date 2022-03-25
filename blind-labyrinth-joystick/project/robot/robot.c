#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "../draw/draw.h"
#include "robot.h"

#define		R			13
#define 	C			15
#define MAX_DIST 5

extern signed char MOVE;
char WIN = 0;

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


_obstacle obs[OBSTACLES];
															 
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

									 
void robot_set_coord(char a, char b) {
	robot.coord.x = a;
	robot.coord.y = b;
}


char checkVal(char a, char b) {
	return labyrinth[a][b];
}


void robot_init(void) {
	robot_set_coord(7, 7);  // posizione inziale del robot
	robot.dir = 'E';			// all'inizio punta a est
}


void robot_set_dir(char dir) {
	
	switch(dir) {
		case('N'):
			robot.dir = 'N';
			break;
		case('S'):
			robot.dir = 'S';
			break;
		case('E'):
			robot.dir = 'E';
			break;
		case('O'):
			robot.dir = 'O';
			break;
		default:
			break;
	}
	
	clearSquare(robot.coord.x, robot.coord.y);
	drawRobot(robot.dir, robot.coord.x, robot.coord.y);
	
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
				obstacleDetected(i, j);
				break;
			} else {  // 0
				dist++;
			}
		} else {  // abbiamo sforato
				break;
		}
	} while (dist < MAX_DIST);
	
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
    
		
		/* e' fermo */
		fermo = 1;
	}
	/* dentro la matrice -> becca ostacolo
											 -> movimeno concesso
											 -> vittoria */
	if (x < R && y < C && x >= 0 && y >= 0) {
		val = checkVal(x,y);
		
		if (val == 1) {

	
			/* e' fermo */
			fermo = 1;
		}
		
		if (val == 0) {
			drawPath(robot.coord.x, robot.coord.y);
			robot_set_coord(x, y);
			drawRobot(robot.dir, x, y);

      proximity();
		}
		
		if (val == 2) {  /* VITTORIA */
			/* 1 Hz all */
			disable_RIT();
			reset_RIT();
			
			clearSquare(robot.coord.x, robot.coord.y);
			drawPath(robot.coord.x, robot.coord.y);
			robot_set_coord(x, y);
			drawExit(x,y);
			WIN = 1;
			drawRobot(robot.dir, x, y);
			winSentence();
			MOVE = -1;
		}
	}
}


void obstaclesInit(void) {
	char i, j, k = 0;
	
	for (i = 0; i < R; i++) {
		for (j = 0; j < C; j++) {
			if (labyrinth[i][j] == 1) {
				obs[k].coord.x = i;
				obs[k].coord.y = j;
				obs[k++].detected = 0;
			}
		}
	}
}


void clearObstacles(void) {
	char i;
	
	for (i = 0; i < OBSTACLES; i++) {
		if (obs[i].detected == 1) {
			obs[i].detected = 0;
			clearSquare(obs[i].coord.x, obs[i].coord.y);
		}
	}
}


void obstacleDetected(char x, char y) {
	char i;
	
	for (i = 0; i < OBSTACLES; i++) {
		if (obs[i].coord.x == x && obs[i].coord.y == y) {
			if (obs[i].detected == 0) {
				obs[i].detected = 1;
				drawObstacle(x,y);
				break;
			}	else if (obs[i].detected == 1) {
				break;
			}
		}
	}
	
}

