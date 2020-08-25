/*********************************************************************
* File		pongTable.cpp
* Author	Peter Look
* Class		COP 2001 201901 10410
* Purpose	Header file for the pong table
**********************************************************************/

#include "pongTable.h"

const float MAX_AI_VELOCITY = 1.8;

/* default constructor*/
PongTable::PongTable()
{
	// ball coords and velocities
	Position ballCurrent = { 10.0, 10.0 };
	Position ballPrevious = { 0.0, 0.0 };
	Position ballVelocity = { 2.0, -1.25 };
	ball = PongObject(BALL_HEIGHT, BALL_WIDTH, ballCurrent, ballPrevious, ballVelocity, true);

	// AI paddle
	Position aiPaddlePos;
	aiPaddlePos.xValue = SCREEN_WIDTH - SCREEN_OFFSET - WALL_THICKNESS - PADDLE_OFFSET - PADDLE_WIDTH;
	aiPaddlePos.yValue = ( SCREEN_HEIGHT / 2) - ( PADDLE_HEIGHT / 2 );
	computerPaddle = PongObject(PADDLE_HEIGHT, PADDLE_WIDTH, aiPaddlePos, { 0, 0 }, { 0, 0 }, false);

	// PLAYER paddle
	Position playerPaddlePos;
	playerPaddlePos.xValue = SCREEN_OFFSET + WALL_THICKNESS + PADDLE_OFFSET - 1;
	playerPaddlePos.yValue = (SCREEN_HEIGHT / 2) - (PADDLE_HEIGHT / 2);
	playerPaddle = PongObject(PADDLE_HEIGHT, PADDLE_WIDTH, playerPaddlePos, { 0, 0 }, { 0, 0 }, false);

	// top wall
	int topHeight = WALL_THICKNESS;
	int topWidth = SCREEN_WIDTH - (2 * SCREEN_OFFSET);
	Position topCurrent = { SCREEN_OFFSET, SCREEN_OFFSET};
	Position topPrevious = { 0, 0};
	Position topVelocity = { 0, 0 };
	topWall = PongObject(topHeight, topWidth, topCurrent, topPrevious, topVelocity, false);

	// bottom wall
	int bottomHeight = WALL_THICKNESS;
	int bottomWidth = SCREEN_WIDTH - (2 * SCREEN_OFFSET);
	Position bottomCurrent = { SCREEN_OFFSET, SCREEN_HEIGHT - SCREEN_OFFSET - WALL_THICKNESS };
	Position bottomPrevious = { 0, 0 };
	Position bottomVelocity = { 0, 0 };
	bottomWall = PongObject(bottomHeight, bottomWidth, bottomCurrent, bottomPrevious, bottomVelocity, false);

	// left wall
	int leftHeight = SCREEN_HEIGHT - (2 * SCREEN_OFFSET);
	int leftWidth = WALL_THICKNESS;
	Position leftCurrent = { SCREEN_OFFSET, SCREEN_OFFSET };
	Position leftPrevious = { 0, 0 };
	Position leftVelocity = { 0, 0 };
	leftWall = PongObject(leftHeight, leftWidth, leftCurrent, leftPrevious, leftVelocity, false);

	// right wall
	int rightHeight = SCREEN_HEIGHT - (2 * SCREEN_OFFSET);
	int rightWidth = WALL_THICKNESS;
	Position rightCurrent = { SCREEN_WIDTH - SCREEN_OFFSET - WALL_THICKNESS, SCREEN_OFFSET };
	Position rightPrevious = { 0, 0 };
	Position rightVelocity = { 0, 0 };
	rightWall = PongObject(rightHeight, rightWidth, rightCurrent, rightPrevious, rightVelocity, false);

}// end default constructor

/** accessors **/
PongObject* PongTable::getBall() { return &ball; }
PongObject* PongTable::getTopWall() { return &topWall;}
PongObject* PongTable::getBottomWall() { return &bottomWall; }
PongObject* PongTable::getLeftWall() { return &leftWall; }
PongObject* PongTable::getRightWall() { return &rightWall; }
PongObject* PongTable::getComputerPaddle() { return &computerPaddle; }
PongObject* PongTable::getPlayerPaddle() { return &playerPaddle; }

/** setters **/
// none

/** helpers **/
void PongTable::render(HDC console, float lag)
{
	// draw the ball
	ball.render(console, lag);

	//draw ai paddle
	computerPaddle.render(console, lag);

	//draw player paddle
	playerPaddle.render(console, lag);

	//draw the walls
	topWall.render(console, 0);
	bottomWall.render(console, 0);
	leftWall.render(console, 0);
	rightWall.render(console, 0);

	return;
}

bool PongTable::collisions()
{
	bool wallContact = false;

	Position ballCurrent = ball.getCurrent();
	Position ballVelocity = ball.getVelocity();

	// check for collision between ball and vertical walls or paddles, if collision is made, velocity is inverted and ball continues on.
	if (ball.intersects(&computerPaddle)) 
	{
		ballCurrent.xValue = computerPaddle.getCurrent().xValue - ball.getWidth() - 1;
		ballVelocity.xValue *= -1;
		computerPaddle.setIsDirty(true);
		wallContact = false;
	}
	else if (ball.intersects(&leftWall))
	{
		ballCurrent.xValue = leftWall.getCurrent().xValue + leftWall.getWidth() + 1;
		ballVelocity.xValue *= -1;
		leftWall.setIsDirty(true);
		wallContact = true;
	}
	else if (ball.intersects(&rightWall))
	{
		ballCurrent.xValue = rightWall.getCurrent().xValue - ball.getWidth() - 1;
		ballVelocity.xValue *= -1;
		rightWall.setIsDirty(true);
		wallContact = true;
	}
	else if (ball.intersects(&playerPaddle))
	{
		ballCurrent.xValue = playerPaddle.getCurrent().xValue + playerPaddle.getWidth() + 1;
		ballVelocity.xValue *= -1;
		playerPaddle.setIsDirty(true);
		wallContact = false;
	}

	// check for collision between ball and horizontal walls, if collision is made, velocity is inverted and ball continues on.
	if (ball.intersects(&topWall))
	{
		ballCurrent.yValue = topWall.getCurrent().yValue + topWall.getHeight() + 1;
		ballVelocity.yValue *= -1;
		topWall.setIsDirty(true);
	}
	else if (ball.intersects(&bottomWall))
	{
		ballCurrent.yValue = bottomWall.getCurrent().yValue - ball.getHeight() - 1;
		ballVelocity.yValue *= -1;
		bottomWall.setIsDirty(true);
	}

	ball.setCurrent(ballCurrent);
	ball.setVelocity(ballVelocity);

	// check for collision between paddles (ai and player) and  horizontal walls
	/** ai **/
	Position aiCurrentPos = computerPaddle.getCurrent();
	Position aiCurrentVel = computerPaddle.getVelocity();

	if (computerPaddle.intersects(&topWall)) 
	{
		aiCurrentPos.yValue = topWall.getCurrent().yValue + topWall.getHeight() + 1;
		aiCurrentVel.yValue = 0;
		topWall.setIsDirty(true);
	}
	else if (computerPaddle.intersects(&bottomWall))
	{
		aiCurrentPos.yValue = bottomWall.getCurrent().yValue - PADDLE_HEIGHT - 1;
		aiCurrentVel.yValue = 0;
		bottomWall.setIsDirty(true);
	}

	computerPaddle.setCurrent(aiCurrentPos);
	computerPaddle.setVelocity(aiCurrentVel);

	/** player **/
	Position playerCurrentPos = playerPaddle.getCurrent();
	Position playerCurrentVel = playerPaddle.getVelocity();

	if (playerPaddle.intersects(&topWall))
	{
		playerCurrentPos.yValue = topWall.getCurrent().yValue + topWall.getHeight() + 1;
		playerCurrentVel.yValue = 0;
		topWall.setIsDirty(true);
	}
	else if (playerPaddle.intersects(&bottomWall))
	{
		playerCurrentPos.yValue = bottomWall.getCurrent().yValue - PADDLE_HEIGHT - 1;
		playerCurrentVel.yValue = 0;
		bottomWall.setIsDirty(true);
	}

	playerPaddle.setCurrent(playerCurrentPos);
	playerPaddle.setVelocity(playerCurrentVel);

	return wallContact;
}

void PongTable::moveComputerPaddle()
{

	Position ballCurrPos = ball.getCurrent();
	Position compPaddlePos = computerPaddle.getCurrent();
	Position compPaddleVel = computerPaddle.getVelocity();

	float paddleDiv = PADDLE_HEIGHT / 3;
	float paddleTop = computerPaddle.getCurrent().yValue + paddleDiv;
	float paddleBottom = paddleTop + PADDLE_HEIGHT - paddleDiv;
	float ballTop = ball.getCurrent().yValue;
	float ballbottom = ballTop + BALL_HEIGHT;

	if (ballbottom < paddleTop)
	{
		compPaddleVel.yValue -= 0.02;
	}
	else if (ballTop > paddleBottom)
	{
		compPaddleVel.yValue += 0.02;
	}
	else
	{
		compPaddleVel.yValue = 0.0;
	}

	//Extra credit 2 line 225 - 232
	if (compPaddleVel.yValue > MAX_AI_VELOCITY) 
	{
		compPaddleVel.yValue = MAX_AI_VELOCITY;
	}
	else if ( -(compPaddleVel.yValue) < -(MAX_AI_VELOCITY) )
	{
		compPaddleVel.yValue = -(MAX_AI_VELOCITY);
	}


	computerPaddle.setVelocity(compPaddleVel);

	return;
}