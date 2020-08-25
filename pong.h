/*********************************************************************
* File		pong.h
* Author	Peter Look
* Class		COP 2001 201901 10410
* Purpose	Header file for the main application file for 
*           the game pong.
**********************************************************************/

#include <Windows.h>

#ifndef PONG_H
#define PONG_H

const float FRAME_RATE = 1000 / 60; // = 16.66666666

const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF PAINT = WHITE;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 250;

const int BALL_WIDTH = 6;
const int BALL_HEIGHT = 6;

//screen dimension constants
const int SCREEN_OFFSET = 2;

//wall dimension constants
const int WALL_THICKNESS = 1;

//paddle dimension constants
const int PADDLE_HEIGHT = 28;
const int PADDLE_WIDTH = 2;
const int PADDLE_OFFSET = 4;

//ball dimension constants
const int BALL_START_X = SCREEN_OFFSET + PADDLE_OFFSET + PADDLE_WIDTH + 10;
const int BALL_START_Y = SCREEN_HEIGHT / 2 - 10;
const int BALL_START_VEL_X = 2;
const int BALL_START_VEL_Y = -1;

// pattern for painting ball
const COLORREF BALL_PATTERN[BALL_HEIGHT][BALL_WIDTH] =
{
	{ BLACK, BLACK, PAINT, PAINT, BLACK, BLACK },
	{ BLACK, PAINT, PAINT, PAINT, PAINT, BLACK },
	{ PAINT, PAINT, PAINT, PAINT, PAINT, PAINT },
	{ PAINT, PAINT, PAINT, PAINT, PAINT, PAINT },
	{ BLACK, PAINT, PAINT, PAINT, PAINT, BLACK },
	{ BLACK, BLACK, PAINT, PAINT, BLACK, BLACK }
};// end BALL_PATTERN

// Key input & directions
enum Input
{
	QUIT,
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};

//Position coordinate structure
struct Position 
{
	float xValue;
	float yValue;
};

#endif //PONG_H

