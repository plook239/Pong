/*********************************************************************
* File		pong.cpp
* Author	Peter Look
* Class		COP 2001 201901 10410
* Purpose	Main application file for the game pong.
**********************************************************************/

#include <iostream>
#include <time.h>
#include "pong.h"
#include "pongTable.h"

using namespace std;

// function prototypes
HDC initConsole();
Input playerInput();
bool processInput(PongTable &table);
bool update(PongTable &table);

int main()
{
	HDC hdc = initConsole();

	// Time components for FPS calculations
	LARGE_INTEGER frequency;
	LARGE_INTEGER lastTime;
	LARGE_INTEGER currentTime;
	LARGE_INTEGER elapsedTime;
	float deltaTime = 0;

	PongTable table = PongTable();
	
	// Get CPU clock frequency
	QueryPerformanceFrequency(&frequency);

	// Set starting current time to now
	QueryPerformanceCounter(&currentTime);

	// Set initial last time to starting time
	lastTime = currentTime;

	// end game if true
	bool nukeGame = update(table);

	// initiate gameball
	table.render(hdc, 0);

	while (!nukeGame)
	{
		// get current frame times
		QueryPerformanceCounter(&currentTime);
		// get time elapsed between last frame and start of this one
		elapsedTime.QuadPart = currentTime.QuadPart - lastTime.QuadPart;
		// set last frame time to current
		lastTime = currentTime;
		// add to delta the current elapsed time converted to MS divided by the CPU freq.
		deltaTime += (double)elapsedTime.QuadPart * 1000 / (double)frequency.QuadPart;

		nukeGame = processInput(table);
	
		// update which direction to move ball
		while (deltaTime >= FRAME_RATE)
		{
			nukeGame = update(table);
			deltaTime -= FRAME_RATE;
		}

		float lagToFrameRate = deltaTime / FRAME_RATE;

		// render and erase ball
		table.render(hdc, lagToFrameRate);

	} // end main game loop

	return 0;
}// end main


 /*********************************************************
 * Initializes the console window for rendering, and returns
 * a handle to a rendering context for the console.
 * @returns HDC	- handle to console's device context
 *********************************************************/

HDC initConsole()
{
	//Get a handle to console window
	HWND conWindow = GetConsoleWindow();

	//Get handle to console standard output stream
	HANDLE conOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//Get a handle to a device context for rendering on the console
	HDC conDc = GetDC(conWindow);

	// resize the console window
	RECT conDimensions;
	GetWindowRect(conWindow, &conDimensions);
	MoveWindow(conWindow, conDimensions.left, conDimensions.top, SCREEN_WIDTH + 42, SCREEN_HEIGHT + 46, true);

	// turn off the blinking cursor
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1;
	cursor.bVisible = false;
	SetConsoleCursorInfo(conOut, &cursor);

	return conDc;
} // end initConsole

 /*********************************************************************
 * Read the console input stream for ball movement or quit key codes.
 * @return Input -  (QUIT, NONE, UP, DOWN, LEFT, RIGHT)
 **********************************************************************/

Input playerInput()
{
	Input input = NONE;	// return direction to move

						//define key constants for WASD (hex literals)
	const WORD VK_W = 0x57;
	const WORD VK_A = 0x41;
	const WORD VK_S = 0x53;
	const WORD VK_D = 0x44;
	const WORD VK_X = 0x58;

	//Get handle to console standard input stream
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

	INPUT_RECORD eventBuff[128];				// event buffer
	unsigned long eventCount, eventsRead;

	// see if there are any events on the standard input stream
	GetNumberOfConsoleInputEvents(hin, &eventCount);

	if (eventCount > 0)
	{
		// read all events availabe into an array buffer
		ReadConsoleInput(
			hin,
			eventBuff,
			128,
			&eventsRead);

		/* see if any of the events read are keyboard events
		and specifically one of the keys we care about */
		unsigned long i = 0;
		while (input == NONE && i < eventsRead)
		{
			if (eventBuff[i].EventType == KEY_EVENT)
			{
				switch (eventBuff[i].Event.KeyEvent.wVirtualKeyCode)
				{
				case VK_W:		// player hit W key
				case VK_UP:		// or up arrow
					input = UP;	// move up
					break;

				case VK_S:		// player hit S key
				case VK_DOWN:	// or down arrow
					input = DOWN;  // move down
					break;

				case VK_A:		// player hit A key
				case VK_LEFT:	// or left arrow
					input = LEFT;  // move left
					break;

				case VK_D:		// player hit D key
				case VK_RIGHT:	// or right arrow
					input = RIGHT;  // move right
					break;

				case VK_X:		// player hit X key
					input = QUIT;	// exit game
					break;

				default:		// some other key was pressed
					input = NONE;
				} //end case statement

			} // end keyboard event type

			i++;
		} // end while events exist

	}

	// get rid of any other input in the stream
	FlushConsoleInputBuffer(hin);

	return input;
}//end playerInput

 /****************************************************************************************
 * Takes input from keyboard and adjusts velocities or exits.
 * @param PongTable table - structure for paddles positions on screen and their velocities.
 * return bool playOn - false when player presses X to exit ie:(Input == QUIT).
 ******************************************************************************************/

bool processInput(PongTable &table)
{
	PongObject *playerPaddle = table.getPlayerPaddle();

	// get user input
	Input input = playerInput();

	bool playOn = true;

	Position velocityPlayerPaddle = playerPaddle->getVelocity();

	if (input == QUIT)
	{
		playOn = false;
	}

	switch (input)
	{
	case UP:
		velocityPlayerPaddle.yValue -= 1.0; //Extra credit Part 1a (1b is in update function)
		break;

	case DOWN:
		velocityPlayerPaddle.yValue += 1.0; //Extra credit Part 1a (1b is in update function)
		break;

	case LEFT:// removed horizontal input for paddles
		break;

	case RIGHT:
		break;

	default:
		break;
	} // end switch

	playerPaddle->setVelocity(velocityPlayerPaddle);

	table.moveComputerPaddle();

	return !playOn;
} //end processInput

  /************************************************************
  * Recieves PongObject table, updates positions and velocities 
  * @param PongObject table - passed by reference.
  * void return
  *************************************************************/

bool update(PongTable &table)
{
	bool endGame = false;

	//update ball
	PongObject *ball = table.getBall();

	Position currentBall = ball->getCurrent();
	Position velocityBall = ball->getVelocity();

	currentBall.xValue += velocityBall.xValue;
	currentBall.yValue += velocityBall.yValue;

	ball->setCurrent(currentBall);
	
	//update computer paddle
	PongObject *computerPaddle = table.getComputerPaddle();

	Position currentCompPaddle = computerPaddle->getCurrent();
	Position velocityCompPaddle = computerPaddle->getVelocity();

	currentCompPaddle.xValue += velocityCompPaddle.xValue;
	currentCompPaddle.yValue += velocityCompPaddle.yValue;

	computerPaddle->setCurrent(currentCompPaddle);

	//update player paddle
	PongObject *playerPaddle = table.getPlayerPaddle();

	Position currentPlayerPaddle = playerPaddle->getCurrent();
	Position velocityPlayerPaddle = playerPaddle->getVelocity();

	currentPlayerPaddle.xValue += velocityPlayerPaddle.xValue;
	currentPlayerPaddle.yValue += velocityPlayerPaddle.yValue;

	playerPaddle->setCurrent(currentPlayerPaddle);

	//Extra credit Part 1b
	if (velocityPlayerPaddle.yValue > 0) 
	{
		velocityPlayerPaddle.yValue += 0.06;
	}
	else if (velocityPlayerPaddle.yValue < 0)
	{
		velocityPlayerPaddle.yValue -= 0.06;
	}

	// true if left or right wall is hit with ball and ends game
	endGame = table.collisions();

	return endGame;
} //end update