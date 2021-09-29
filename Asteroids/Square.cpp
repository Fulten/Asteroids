#include "Square.h"

Square::Square() :
	Shape()
{
	points[0] = myVec2( 0, 0);
	points[1] = myVec2( 0, 0);
	points[2] = myVec2( 0, 0);
	points[3] = myVec2( 0, 0);
}

Square::Square(myVec2 pos, float size) :
	Shape(pos, size, D3DCOLOR_XRGB(0, 255, 255))
{
	points[0] = myVec2( 0, -size);
	points[1] = myVec2( size, 0);
	points[2] = myVec2( 0, size);
	points[3] = myVec2( -size, 0);
}

void Square::GetPoints(std::vector<myVec2>& out)
{
	for (int i = 0; i < 4; i++)
	{
		myVec2 temp;
		float length = points[i].getLength();
	
		//starburst
		//temp.x = cos(angle) * points[i].x - sin(angle) * points[i].x;
		//temp.y = cos(angle) * points[i].y - sin(angle) * points[i].y;

		temp.x = cos(angle) * points[i].x - sin(angle) * points[i].y;
		temp.y = sin(angle) * points[i].x + cos(angle) * points[i].y;

		out.push_back(temp + getPos());
	}

	float length = points[0].getLength();
	myVec2 temp;

	temp.x = cos(angle) * points[0].x - sin(angle) * points[0].y;
	temp.y = sin(angle) * points[0].x + cos(angle) * points[0].y;

	out.push_back(temp + getPos());
}

void Square::setPos(myVec2 pos)
{
	Shape::setPos(pos);
}

void Square::setPos(float x, float y)
{
	Shape::setPos(myVec2(x, y));
}

void Square::setRotation(float degrees)
{
	angle = degrees * 3.1415 / 180;
}

void Square::move(float vel)
{
	myVec2 temp;

	temp.x = cos(angle) * vel;
	temp.y = sin(angle) * vel;

	temp += getPos();

	if (temp.x < 0) temp.x += windowSize->x;
	if (temp.x > windowSize->x) temp.x -= windowSize->x;

	if (temp.y < 0) temp.y += windowSize->y;
	if (temp.y > windowSize->y) temp.y -= windowSize->y;

	Shape::setPos(temp);
}