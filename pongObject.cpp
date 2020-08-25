/*********************************************************************
* File		pongObject.cpp
* Author	Peter Look
* Class		COP 2001 201901 10410
* Purpose	Implementation file for a pong game object class that represents
the ball, paddles, and walls.
**********************************************************************/

#include "pongObject.h"

/* default constructor*/
PongObject::PongObject() 
{
	current = {0.0f, 0.0f};
	previous = { 0.0f, 0.0f };
	velocity = { 0.0f, 0.0f };
	height = 0;
	width = 0;
	ballInd = false;
	dirty = true;
}// end default constructor

/*over-load default constructor*/
PongObject::PongObject(int heightIn, int widthIn, Position currentIn, Position previousIn, Position velocityIn, bool hasMovedIn)
{
	height = heightIn;
	width = widthIn;
	current = currentIn;
	previous = previousIn;
	velocity = velocityIn;
	ballInd = hasMovedIn;
	dirty = true;
}// end PongObject

/** accessors **/
Position PongObject::getCurrent() { return current; }
Position PongObject::getPrevious() { return previous; }
Position PongObject::getVelocity() { return velocity; }
int PongObject::getHeight() { return height; }
int PongObject::getWidth() { return width; }
bool PongObject::isBall() { return ballInd; }
bool PongObject::isDirty() { return dirty; }

/** setters **/
void PongObject::setCurrent(Position value) 
{
	current = value;
}// end setCurrent
void PongObject::setPrevious(Position value) 
{
	previous = value;
}// end setPrevious
void PongObject::setVelocity(Position value) 
{
	velocity = value;
}// end setVelocity
void PongObject::setHeight(int value) 
{	
	height = value;
}// end setHeight
void PongObject::setWidth(int value)
{
	width = value;
}// end setWidth
void PongObject::setIsBall(bool value) 
{
	ballInd = value;
} // end setIsBall
void PongObject::setIsDirty(bool value)
{
	dirty = value;
} // end setIsBall

/** helpers **/
Position PongObject::getCurrent(float lag)
{
	Position local = current;
	local.xValue = current.xValue + (lag * velocity.xValue);
	local.yValue = current.yValue + (lag * velocity.yValue);
	
	return local;
}// end getCurrent

bool PongObject::hasMoved(float lag)
{
	Position adjusted = getCurrent(lag);
	return (int(adjusted.xValue) != int(previous.xValue) || int(adjusted.yValue) != int(previous.yValue));
}// end hasMoved

void PongObject::erase(HDC console) 
{
	int xpos = previous.xValue;
	int ypos = previous.yValue;

	for (int row = 0; row <= height - 1; row++)
	{
		for (int column = 0; column <= width - 1; column++)
		{
			SetPixel(console, xpos + column, ypos + row, BLACK);
		}
	}

	return;
}// end erase

void PongObject::draw(HDC console, float lag)
{
	Position adjusted = getCurrent(lag);
	int xpos = adjusted.xValue;
	int ypos = adjusted.yValue;

	for (int row = 0; row <= height - 1; row++)
	{
		for (int column = 0; column <= width - 1; column++)
		{
			COLORREF paint = PAINT;
			
			if (isBall()) 
			{
				paint = BALL_PATTERN[row][column];
			}

			SetPixel(console, xpos + column, ypos + row, paint);
		}
	}

	setPrevious(adjusted);

	return;
}// end draw

void PongObject::render(HDC console, float lag) 
{
	if ( hasMoved(lag) || isDirty() ) 
	{
		erase(console);
		draw(console, lag);
		dirty = false;
	}

	return;
}

bool PongObject::intersects(PongObject* other)
{
	float thisTop = current.yValue;
	float thisBottom = current.yValue + height;
	float thisLeft = current.xValue;
	float thisRight = current.xValue + width;

	float otherTop = other->getCurrent().yValue;
	float otherBottom = other->getCurrent().yValue + other->getHeight();
	float otherLeft = other->getCurrent().xValue;
	float otherRight = other->getCurrent().xValue + other->getWidth();
	
	return !(otherLeft > thisRight || otherRight < thisLeft || otherTop > thisBottom || otherBottom < thisTop);
}