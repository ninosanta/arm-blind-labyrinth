#include "../robot/robot.h"

#ifndef 	DRAW_H
#define		DRAW_H

#define 		R 	13
#define 		C		15
#define 	START_LAB_ROW		65		// riga pixel da cui inizia il labirinto
#define		START_LAB_COL		6			// colonna pixel da cui inizia il labirinto
#define		LAB_R 195
#define 	LAB_C 225
#define		SQUARE					15
#define 	TRI_DISP_R_EST	2			// displacement da riga del triangolo dentro cella
#define		TRI_DISP_C_EST	5			// displacement da colonna
#define 	TRI_HH					6			// semi-altezza triangolo
#define 	TRI_L						11		// distanza base vertice triangolo
#define		TRI_DISP_C_NOR	8

#define START_RES_BUTT_ROW 	275
#define START_RES_BUTT_COL 	36
#define	RES_BUT_C						60
#define	RES_BUT_R						30	


void drawLabyrinthMatrixOnly(void);
void drawLabyrinth(void);
void drawRobot(char dir, char x, char y);
void clearSquare(char x, char y);
void winSentence(void);
void startSencente(void);
void drawObstacle(char x, char y);
void drawPath(char x, char y);
void clearStartSentence(void);
void drawExit(char x, char y);

#endif  //	DRAW_H



