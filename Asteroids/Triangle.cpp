#include "Triangle.h"

Triangle::Triangle() : 
	Shape()
{
	points[0] = myVec2(4, 0);
	points[1] = myVec2(0, 16);
	points[2] = myVec2(-4, 0);
}

Triangle::Triangle(myVec2 pos) :
	Shape(pos, 4, D3DCOLOR_XRGB(255, 50, 0))
{
	points[0] = myVec2(0, 5);
	points[1] = myVec2(1, 1);
	points[2] = myVec2(-1, 1);
}

void Triangle::GetPoints(std::vector<myVec2>& out)
{
	float angle = this->angle + 3.1415 / 2;
	for (int i = 0; i < 3; i++)
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

void Triangle::setPos(myVec2 pos)
{
	Shape::setPos(pos);
}

void Triangle::setPos(float x, float y)
{
	Shape::setPos(myVec2(x, y));
}

void Triangle::setLength(int leng)
{
	points[0].y = leng;
}

void Triangle::setRotation(float degrees)
{
	angle = degrees * 3.1415 / 180;
}

void Triangle::move(float vel)
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
