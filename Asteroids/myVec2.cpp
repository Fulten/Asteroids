#include "myVec2.h"

// constructor
myVec2::myVec2(float x, float y) :
	x(x),
	y(y)
{}

// left + right
myVec2 myVec2::operator+(const myVec2& rhs) const
{
	return myVec2(x + rhs.x, y + rhs.y);
}

// left = left + right
myVec2& myVec2::operator+=(const myVec2& rhs)
{
	*this = *this + rhs;
	return *this;
}

myVec2 myVec2::operator-(const myVec2& rhs) const
{
	return myVec2(x - rhs.x, y - rhs.y);
}

myVec2& myVec2::operator-=(const myVec2& rhs)
{
	*this = *this - rhs;
	return *this;
}

// left * right
myVec2 myVec2::operator*(float rhs) const
{
	return myVec2(x * rhs, y * rhs);
}

myVec2& myVec2::operator*=(float rhs)
{
	*this = *this * rhs;
	return *this;
}

bool myVec2::operator>(const myVec2& rhs) const
{
	if (x > rhs.x && y > rhs.y)
		return true;
	return false;
}

bool myVec2::operator<(const myVec2& rhs) const
{
	if (x < rhs.x && y < rhs.y)
		return true;
	return false;
}

float myVec2::getLengthSq() const
{
	return x * x + y * y;
}

float myVec2::getLength() const
{
	return sqrt(getLengthSq());
}

myVec2 myVec2::Normalize()
{
	return *this = getNormalized();
}

myVec2 myVec2::getNormalized() const
{
	const float len = getLength();
	if (len != 0.0f)
		return *this * (1.0f / len);
	return *this;
}

myVec2 myVec2::round()
{
	return myVec2((int)x, (int)y);
}