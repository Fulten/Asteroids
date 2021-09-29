#include "Shape.h"

myVec2* Shape::windowSize;

void Shape::setPos(myVec2 in)
{
	pos = in;
}


float Shape::getradius()
{
	return radius;
}

myVec2& Shape::getPos()
{
	return pos;
}

void Shape::setColor(D3DCOLOR c)
{
	color = c;
}

D3DCOLOR Shape::getColor()
{
	return color;
}

void Shape::remove()
{
	toBeRemoved = true;
}

bool Shape::getRemove()
{
	return toBeRemoved;
}

void Shape::setWindow(myVec2* window)
{
	windowSize = window;
}

// check for collisions between two shapes using circle on circle collision, while acounting for screen wraping
bool Shape::checkCollision(Shape& test)
{
	// copy the two positions 
	myVec2 pos1 = pos;
	myVec2 pos2 = test.getPos();

	// retrive the distance between the two points using the normal grid
	float distance = (pos1 - pos2).getLengthSq();

	// create a second grid space offset from the original to check since i decided to use non euiclidian space
	// ofset the 2 points by half the window size to check for collisions across the boarder
	pos1 -= *windowSize * 0.5;
	pos2 -= *windowSize * 0.5;

	// correct the position of points that wrap around
	if (pos1.x < 0) pos1.x += windowSize->x;
	if (pos1.x > windowSize->x) pos1.x -= windowSize->x;
	if (pos1.y < 0) pos1.y += windowSize->y;
	if (pos1.y > windowSize->y) pos1.y -= windowSize->y;

	if (pos2.x < 0) pos2.x += windowSize->x;
	if (pos2.x > windowSize->x) pos2.x -= windowSize->x;
	if (pos2.y < 0) pos2.y += windowSize->y;
	if (pos2.y > windowSize->y) pos2.y -= windowSize->y;

	// caculate the distance between points using our transformed grid
	float distanceOffset = (pos1 - pos2).getLengthSq();

	float check;

	// use the shortest distance to calculate collision
	if (distance < distanceOffset)
		check = distance;
	else
		check = distanceOffset;

	// find collision
	if (check < (radius + test.getradius()) * (radius + test.getradius()))
		return true;

	return false;
}