#ifndef CONSTANTS_H
#define CONSTANTS_H

// Window resolution
const int WINDOW_WIDTH	 = 800;
const int WINDOW_HEIGHT	 = 600;

// Grid dimensions
const int BOX_SIZE		 = 150;
const int LINE_THICKNESS = 15;
const int GRID_SIZE		 = (3 * BOX_SIZE + 2 * LINE_THICKNESS);
const int MARGIN		 = (WINDOW_HEIGHT - GRID_SIZE) / 2;

const int BOX_NUMBER	 = 9;
const int SPRITE_NUMBER  = 2;

const int FPS_DECIMAL_DIGIT = 2;

enum ePlayerTurn {
	PLAYER_CPU = 0,
	PLAYER_ONE = 1,
	PLAYER_TWO = 2,
};

enum eWinner {
	NO_WINNER = 0,
	P1_WINNER = 1,
	P2_WINNER = 2,
};

#endif