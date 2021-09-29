#pragma once

#include <math.h>

class myVec2
{
public:
	myVec2() = default; // default constructor
	myVec2(float xIn, float yIn);
	myVec2 operator+(const myVec2& rhs) const; // left + right
	myVec2& operator+=(const myVec2& rhs); // left = left + right

	myVec2 operator-(const myVec2& rhs) const; // left - right
	myVec2& operator-=(const myVec2& rhs); // left = left - right

	myVec2 operator*(float rhs) const; // left * right
	myVec2& operator*=(float rhs); // left = left * right

	//greater than
	bool operator>(const myVec2& rhs) const;
	// less than
	bool operator<(const myVec2& rhs) const;

	myVec2 Normalize();
	myVec2 getNormalized() const;

	float getLengthSq() const;

	// returns the length of the vector's hypotenuse
	float getLength() const;

	myVec2 round();

public:
	float x;
	float y;

};