#pragma once

#include "Shape.h"

class Bullet : public Shape
{
private:
	myVec2 points[2];
	float angle;
	const int size = 2;

public:

	Bullet(myVec2 pos, float angle);
	~Bullet();

	void setPos(myVec2 pos);
	void setPos(float x, float y);

	void GetPoints(std::vector<myVec2>& out) override;
	void setRotation(float degrees) override;
	void move(float vel);

};

