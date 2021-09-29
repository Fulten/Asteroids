#include "Bullet.h"


Bullet::Bullet(myVec2 pos, float angle) :
	Shape(pos, 1, D3DCOLOR_XRGB(0, 255, 0)),
	angle(angle)
{
	points[0] = myVec2(-2, 0);
	points[1] = myVec2(0, 0);
}

Bullet::~Bullet()
{

}

void Bullet::GetPoints(std::vector<myVec2>& out)
{
	//float angle = this->angle + 3.1415 / 2;
	for (int i = 0; i < size; i++)
	{
		myVec2 temp;
		float length = points[i].getLength();

		//starburst pattern
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

void Bullet::setPos(myVec2 pos)
{
	Shape::setPos(pos);
}

void Bullet::setPos(float x, float y)
{
	Shape::setPos(myVec2(x, y));
}

void Bullet::setRotation(float degrees)
{
	angle = degrees * 3.1415 / 180;
}

void Bullet::move(float vel)
{
	myVec2 temp;

	temp.x = cos(angle) * vel;
	temp.y = sin(angle) * vel;

	temp += getPos();

	if (temp.x < 2) remove();
	if (temp.x > windowSize->x - 2) remove();

	if (temp.y < 2) remove();
	if (temp.y > windowSize->y - 2) remove();

	Shape::setPos(temp);
}