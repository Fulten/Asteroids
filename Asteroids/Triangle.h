#pragma once

#include "Shape.h";

class Triangle : public Shape
{
private:
	myVec2 points[3];
	float angle = 0.0f;

public: 

	Triangle();
	Triangle(myVec2 pos);

	void setPos(myVec2 pos);
	void setPos(float x, float y);
	void setLength(int leng);

	void GetPoints(std::vector<myVec2>& out) override;
	void setRotation(float degrees) override;
	void move(float vel);
};

