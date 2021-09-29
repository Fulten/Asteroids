#pragma once

#include "Shape.h"

class Square : public Shape
{
private:
	myVec2 points[4];
	float angle = 270.0f;

public:

	Square();
	Square(myVec2 pos, float size);

	void setPos(myVec2 pos);
	void setPos(float x, float y);

	void GetPoints(std::vector<myVec2>& out) override;
	void setRotation(float degrees) override;
	void move(float vel);

};

