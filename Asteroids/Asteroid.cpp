#include "Asteroid.h"


Asteroid::~Asteroid()
{}

Asteroid::Asteroid(myVec2 pos) : 
	Shape(pos, 8, D3DCOLOR_XRGB(180, 50, 0)),
	rockSize(1)
{
	direction = (rand() % 360) * 3.1415 / 180 ;

	for (int i = 0; i < size; i++)
	{
		float temp = (3.1415 * 2 / size) * (i + (int)1);
		float length = rand() % 7 + rockSize * 5;

		points[i].x = cos(temp) * length;
		points[i].y = sin(temp) * length;
	}
}

Asteroid::Asteroid(myVec2 pos, float s) : 
	Shape(pos, s * 5 + 2, D3DCOLOR_XRGB(180, 50, 0)),
	rockSize(s)
{


	direction = (rand() % 360) * 3.1415 / 180;

	rotationSpeed = rand() % 3 + 1;

	for (int i = 0; i < size; i++)
	{
		float temp = (3.1415 * 2 / size) * (i + (int)1);
		float length = rand() % 7 + rockSize * 5;

		points[i].x = cos(temp) * length;
		points[i].y = sin(temp) * length;
	}
}

Asteroid::Asteroid(myVec2 pos, float s, D3DCOLOR color) : 
	Asteroid(pos, s)
{
	setColor(color);
}

void Asteroid::GetPoints(std::vector<myVec2>& out)
{
	// adjust angle the ship is rendered at by about 90`
	float angle = this->angle + 3.1415 / 2;
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

void Asteroid::setPos(myVec2 pos)
{
	Shape::setPos(pos);
}

void Asteroid::setPos(float x, float y)
{
	Shape::setPos(myVec2(x, y));
}

void Asteroid::setRotation(float degrees)
{
	angle = degrees * 3.1415 / 180;
}

void Asteroid::move(float vel)
{
	angle += 0.01 * rotationSpeed;

	myVec2 temp;

	temp.x = cos(direction) * vel;
	temp.y = sin(direction) * vel;

	temp += getPos();

	if (temp.x < 0) temp.x += windowSize->x;
	if (temp.x > windowSize->x) temp.x -= windowSize->x;

	if (temp.y < 0) temp.y += windowSize->y;
	if (temp.y > windowSize->y) temp.y -= windowSize->y;

	Shape::setPos(temp);
}


int Asteroid::getSize()
{
	return rockSize;
}