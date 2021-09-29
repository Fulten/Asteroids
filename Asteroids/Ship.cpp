
#include "Ship.h"

Ship::Ship(myVec2 pos) :
	Shape(pos, 2, D3DCOLOR_XRGB(255, 255, 0))
{
	points[0] = myVec2( 0, -3);
	points[1] = myVec2( 2,  1);
	points[2] = myVec2( 0,  0);
	points[3] = myVec2(-2,  1);
}

Ship::Ship(myVec2 pos, float size) : 
	Shape(pos,  size, D3DCOLOR_XRGB(255, 255, 0))
{
	points[0] = myVec2( 0, -2 * size);
	points[1] = myVec2( 1 * size,  1 * size);
	points[2] = myVec2( 0,  0);
	points[3] = myVec2(-1 * size,  1 * size);
}

void Ship::GetPoints(std::vector<myVec2>& out)
{
	// adjust angle the ship is rendered at by about 90` because i am an idiot who cannot properly algnine his models
	float angle = this->angle + 3.1415 / 2;
	for (int i = 0; i < size; i++)
	{
		myVec2 temp;
		float length = points[i].getLength();

		//starburst pattern that i thought look neat as angle increases the shape cycles through distortions like an expanding and shrinking ring
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

void Ship::setPos(myVec2 pos)
{
	Shape::setPos(pos);
}

void Ship::setPos(float x, float y)
{
	Shape::setPos(myVec2(x, y));
}

void Ship::setRotationDeg(float degrees)
{
	angle = degrees * 3.1415 / 180;
}

void Ship::setRotation(float radians)
{
	angle = radians;
}

float Ship::getRotation() const
{
	return angle;
}

// old movement, no conservation of momentum
void Ship::moveOld(float vel)
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

// new movement
void Ship::move(myVec2 vel)
{
	myVec2 temp;

	temp = vel + getPos();

	if (temp.x < 0) temp.x += windowSize->x;
	if (temp.x > windowSize->x) temp.x -= windowSize->x;

	if (temp.y < 0) temp.y += windowSize->y;
	if (temp.y > windowSize->y) temp.y -= windowSize->y;

	Shape::setPos(temp);
}