#pragma once

#include "myVec2.h"
#include <vector>
#include <d3d9.h>

// abstract shape class
class Shape
{
private:
	myVec2 pos;
	float radius;
	D3DCOLOR color;

	bool toBeRemoved = false;
	
protected:
	void setPos(myVec2 in);
	static myVec2* windowSize;
	float getradius();

public:
	Shape() :
		pos(1, 1),
		radius(1),
		color(D3DCOLOR_XRGB(255, 255, 255))
	{}

	Shape(myVec2 pos, float radius, D3DCOLOR color) :
		pos(pos),
		radius(radius),
		color(color)
	{}


	myVec2& getPos();
	void setColor(D3DCOLOR c);
	D3DCOLOR getColor();

	void remove();
	bool getRemove();

	static void setWindow(myVec2* window);
	virtual void GetPoints(std::vector<myVec2>& out) = 0;
	virtual void setRotation(float degrees) = 0;
	bool checkCollision(Shape& test);
};