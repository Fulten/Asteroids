#pragma once
#include "Shape.h"
#include <random>

class Asteroid : public Shape
{
private:

	myVec2 points[16];
	float angle = 0.0f;
	float direction = 0.0f;
	int rotationSpeed = 1;
	const int size = 16;
	const int rockSize;

public:

	Asteroid(myVec2 pos);
	Asteroid(myVec2 pos, float s);
	Asteroid(myVec2 pos, float s, D3DCOLOR color);
	~Asteroid();


	void setPos(myVec2 pos);
	void setPos(float x, float y);

	void GetPoints(std::vector<myVec2>& out) override;
	void setRotation(float degrees) override;
	void move(float vel);
	int getSize();


};

