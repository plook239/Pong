/*********************************************************************
* File		pongObject.h
* Author	Peter Look
* Class		COP 2001 201901 10410
* Purpose	Header file for a pong game object class that represents
			the ball, paddles, and walls.
**********************************************************************/

#include "pong.h"

#ifndef PONGOBJECT_H
#define PONGOBJECT_H

class PongObject 
{
	public:
		/* default constructor*/
		PongObject();
		/*over-load default constructor*/
		PongObject(int heightIn, int widthIn, Position currentIn, Position previousIn, Position velocityIn, bool hasMovedIn);

		/** accessors **/
		Position getCurrent();
		Position getPrevious();
		Position getVelocity();
		Position getCurrent(float lag);
		bool hasMoved(float lag);
		int getHeight();
		int getWidth();
		bool isBall();
		bool isDirty();
		
		/** setters **/
		void setCurrent(Position value);
		void setPrevious(Position value);
		void setVelocity(Position value);
		void setHeight(int value);
		void setWidth(int value);
		void setIsBall(bool value);
		void setIsDirty(bool value);

		/** helpers **/
		void erase(HDC console);
		void draw(HDC console, float lag);
		void render(HDC console, float lag);
		bool intersects(PongObject* other);

	private:
		Position current;
		Position previous;
		Position velocity;
		int height;
		int width;
		bool ballInd;
		bool dirty;

}; // end PongObject

#endif // PONGOBJECT_H
