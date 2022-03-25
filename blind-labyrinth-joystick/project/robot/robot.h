#ifndef		ROBOT_H
#define		ROBOT_H

#define 	OBSTACLES		28


typedef struct {
	signed char x, y;
} _coord;

typedef struct {
	_coord coord;
	char detected;
} _obstacle;



typedef struct {
	_coord coord;
	char dir;	 		// direzione verso cui punta il robot
} _robot;


char checkVal(char a, char b);
void robot_init(void);
void robot_set_coord(char a, char b);
void robot_set_dir(char dir);
void proximity(void);
void step(void);
void newTimerVal(char timerN, unsigned int timerV);
void obstaclesInit(void);
void obstacleDetected(char x, char y);
void clearObstacles(void);

#endif 	// ROBOT_H

