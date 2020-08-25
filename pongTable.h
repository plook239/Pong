/*********************************************************************
* File		pongTable.h
* Author	Peter Look
* Class		COP 2001 201901 10410
* Purpose	Header file for the pong table
**********************************************************************/

#include "pongObject.h"

#ifndef PONGTABLE_H
#define PONGTABLE_H

class PongTable
{
	public:
		/* default constructor*/
		PongTable();

		/** accessors **/
		PongObject* getBall();
		PongObject* getTopWall();
		PongObject* getBottomWall();
		PongObject* getLeftWall();
		PongObject* getRightWall();
		PongObject* getComputerPaddle();
		PongObject* getPlayerPaddle();
		/** setters **/
		// none

		/** helpers **/
		void render(HDC console, float lag);
		bool collisions();
		void moveComputerPaddle();

	private:  
		PongObject ball;
		PongObject topWall;
		PongObject bottomWall;
		PongObject leftWall;
		PongObject rightWall;
		PongObject computerPaddle;
		PongObject playerPaddle;

};// end PongTable

#endif //PONGTABLE_H