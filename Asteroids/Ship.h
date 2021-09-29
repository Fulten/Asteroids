#pragma once

#include "Shape.h"

class Ship : public Shape
{
private:
	myVec2 points[4];
	float angle = 0.0f;
	const int size = 4;

public:

	Ship(myVec2 pos);
	Ship(myVec2 pos, float size);

	void setPos(myVec2 pos);
	void setPos(float x, float y);

	void operator=(Ship& rhs) = delete;

	void GetPoints(std::vector<myVec2>& out) override;
	void setRotation(float degrees) override;
	void setRotationDeg(float radians);
	float getRotation() const;
	void moveOld(float vel);
	void move(myVec2 vel);
};
