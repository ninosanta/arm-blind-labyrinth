#include "../GLCD/GLCD.h"
#include "draw.h"

extern signed char MOVE;
extern char WIN;

void drawRobot(char dir, char x, char y) {
	signed int i, j;
	unsigned int r, c;
	unsigned short int color;
	
	if (MOVE == 1) {
		color = Orange;
	} else if (MOVE == 0) {
		color = Magenta;
	}
	if (WIN == 1) {
		color = Red;
	}
	
	switch (dir) { 
		case ('E'):
			r = x*SQUARE+TRI_DISP_R_EST + START_LAB_ROW ;
			c = y*SQUARE+TRI_DISP_C_EST + START_LAB_COL ;
		
			for (i = 0; i < TRI_HH; i++)  {
				LCD_DrawLine(c, r+i, c+i, r+i, color);
			}
			for (j = 1; i <= TRI_L; i++, j+=2)  {
				LCD_DrawLine(c, r+i, c+i-j, r+i, color);
			}
			break;
		case ('O'):
			r = x*SQUARE+TRI_DISP_R_EST + START_LAB_ROW ;
			c = y*SQUARE+SQUARE - TRI_DISP_C_EST + START_LAB_COL ;
		
			for (i = 0; i < TRI_HH; i++)  {
				LCD_DrawLine(c, r+i, c-i, r+i, color);
			}
			for (j = 1; i <= TRI_L; i++, j+=2)  {
				LCD_DrawLine(c, r+i, c-i+j, r+i, color);
			}
			break;
		case ('N'):
			r = x*SQUARE+SQUARE-TRI_DISP_C_EST + START_LAB_ROW;
			c = y*SQUARE+TRI_DISP_R_EST + START_LAB_COL ;
		
			for (i = 0; i > -TRI_HH; i--)  {
				LCD_DrawLine(c-i, r, c-i, r+i, color);
			}
			for (j = 1; i >= -TRI_L; i--, j+=2)  {
				LCD_DrawLine(c-i, r, c-i, r+i+j, color);
			}
			break;
		case ('S'):
			r = x*SQUARE+TRI_DISP_C_EST + START_LAB_ROW ;
			c = y*SQUARE+TRI_DISP_R_EST + START_LAB_COL ;
		
			for (i = 0; i > -TRI_HH; i--)  {
				LCD_DrawLine(c-i, r, c-i, r-i, color);
			}
			for (j = 1; i >= -TRI_L; i--, j+=2)  {
				LCD_DrawLine(c-i, r, c-i, r-i-j, color);
			}
			break;
		default:
			break;
	}
	
}


void drawLabyrinthMatrixOnly(void) {
	unsigned short int i, j, r, c;
	
	/* RIGHE labirinto */ 
	for ( i = START_LAB_ROW, r = 0; r <= R; r++, i += SQUARE ) {
		for( j = START_LAB_COL; j <= LAB_C + START_LAB_COL; j++ ) {
			LCD_SetPoint(j, i, Cyan);
		}
	}
	/* COLONNE labirinto */
	for ( j = START_LAB_COL, c = 0; c <= C; c++, j += SQUARE ) {
		for ( i = START_LAB_ROW; i <= LAB_R + START_LAB_ROW; i++ ) {
			LCD_SetPoint(j, i, Cyan);
		}
	}
}


void drawLabyrinth(void) {
	unsigned short int i, j, r, c;
	
	/* TEXT BOX */
	// righe
	for ( i = SQUARE/2, r = 0; r < 2; r++, i += (SQUARE*3) ) {
		for( j = START_LAB_COL, c = 0; c <= LAB_C; c++) {
			LCD_SetPoint(j+c, i, Cyan);
		}
	}
	// colonne
	for ( j = START_LAB_COL, c = 0; c < 2; j += LAB_C, c++ ) {
		for (  i = SQUARE/2, r = 0; r <= (SQUARE*3); r++ ) {
			LCD_SetPoint(j, i+r, Cyan);
		}
	}
	// riempimento
	for (i = SQUARE/2+1, r = 0; r < SQUARE*3-1; r++) {
		for (j = START_LAB_COL+1, c = 0; c < LAB_C-1; c++)
			LCD_SetPoint(j+c, i+r, Yellow);
	}
	GUI_Text(SQUARE*5+SQUARE/2, SQUARE/2+SQUARE-10, (uint8_t *) "Mr. Robot", Black, Yellow);
	GUI_Text(SQUARE*2, SQUARE/2+SQUARE+16-10, (uint8_t *) "in the blind labyrinth", Black, Yellow);
	
	
	drawLabyrinthMatrixOnly();


	/* TASTI */
	// RESET: righe
	for ( i = START_RES_BUTT_ROW, r = 0; r < 2; r++, i += RES_BUT_R ) {
		for( j = START_RES_BUTT_COL, c = 0; c <= RES_BUT_C; c++) {
			LCD_SetPoint(j+c, i, Cyan);
		}
	}
	// RESET: colonne
	for ( j = START_RES_BUTT_COL, c = 0; c < 2; j += RES_BUT_C, c++ ) {
		for(  i = START_RES_BUTT_ROW, r = 0; r <= RES_BUT_R; r++ ) {
			LCD_SetPoint(j, i+r, Cyan);
		}
	}
	// riempimento
	for ( i = START_RES_BUTT_ROW+1, r = 0; r < RES_BUT_R-1; r++) {
		for ( j = START_RES_BUTT_COL+1, c = 0; c < RES_BUT_C-1; c++ ) {
			LCD_SetPoint(j+c, i+r, Yellow);
		}
	}
	// TESTO:
	GUI_Text(START_RES_BUTT_COL+SQUARE/5, START_RES_BUTT_ROW+SQUARE/2, (uint8_t *) "Restart", Black, Yellow);
	
	
	// CLEAR: righe
	for ( i = START_RES_BUTT_ROW, r = 0; r < 2; r++, i += RES_BUT_R ) {
		for( j = START_RES_BUTT_COL + RES_BUT_C + SQUARE*3, c = 0; c <= RES_BUT_C; c++) {
			LCD_SetPoint(j+c, i, Cyan);
		}
	}
	// CLEAR: colonne
	for ( j = START_RES_BUTT_COL + RES_BUT_C + SQUARE*3, c = 0; c < 2; j+= RES_BUT_C, c++ ) {
		for(  i = START_RES_BUTT_ROW, r = 0; r <= RES_BUT_R; r++ ) {
			LCD_SetPoint(j, i+r, Cyan);
		}
	}
	// riempimento
	for ( i = START_RES_BUTT_ROW+1, r = 0; r < RES_BUT_R-1; r++) {
		for ( j = START_RES_BUTT_COL + RES_BUT_C + SQUARE*3 + 1, c = 0; c < RES_BUT_C-1; c++ ) {
			LCD_SetPoint(j+c, i+r, Yellow);
		}
	}
	// TESTO:
	GUI_Text(START_RES_BUTT_COL+RES_BUT_C+SQUARE*3 +SQUARE-4, START_RES_BUTT_ROW+SQUARE/2, (uint8_t *) "Clear", Black, Yellow);
	
	
}


void drawSquare(char x, char y, unsigned short int color) {
	int i, j, k, n;

	for (i = x*SQUARE + START_LAB_ROW+1, k = 0; k < SQUARE-1; i++, k++) {
		for (j = y*SQUARE + START_LAB_COL+1, n = 0; n < SQUARE-1; j++, n++) {
			LCD_SetPoint(j, i, color);
		}
	}
}


void drawObstacle(char x, char y) {
	drawSquare(x, y, Red); 
}


void drawPath(char x, char y) {
	drawSquare(x, y, Grey2); 
}


void drawExit(char x, char y) {
	drawSquare(x, y, Green2); 
}


void clearSquare(char x, char y) {
	drawSquare(x, y, Black); 
}


void startSencente(void) {
	GUI_Text(START_LAB_COL+SQUARE*5-2, START_LAB_ROW+SQUARE*5+SQUARE/2+1, (uint8_t *) "Touch here", Black, Yellow);
	GUI_Text(START_LAB_COL+SQUARE*2-SQUARE/2+2, START_LAB_ROW+SQUARE*5+SQUARE/2+16, (uint8_t *) "to start a new game...", Black, Yellow);
}


void clearStartSentence(void) {
	GUI_Text(START_LAB_COL+SQUARE*5-2, START_LAB_ROW+SQUARE*5+SQUARE/2+1, (uint8_t *) "Touch here", Black, Black);
	GUI_Text(START_LAB_COL+SQUARE*2-SQUARE/2+2, START_LAB_ROW+SQUARE*5+SQUARE/2+16, (uint8_t *) "to start a new game...", Black, Black);
	drawLabyrinthMatrixOnly();
}


void winSentence(void) { 
	GUI_Text(START_LAB_COL+SQUARE*2-1, START_LAB_ROW+SQUARE*5, (uint8_t *) "Congrats, you did it!", Black, Yellow);
	GUI_Text(START_LAB_COL+SQUARE*2+SQUARE/2, START_LAB_ROW+SQUARE*5+15, (uint8_t *) "Touch \"Restart\" for", Black, Yellow);
	GUI_Text(START_LAB_COL+SQUARE*5-2, START_LAB_ROW+SQUARE*5+31, (uint8_t *) "some more!", Black, Yellow);
}

