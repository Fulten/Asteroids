#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <string>
#include "myVec2.h"

	struct TEXTUREDVERTEX { FLOAT x, y, z, rhw, tu, tv; }; // custom vertex struct contains position and texture data for more complex renders
	#define TEXTUREDFVF (D3DFVF_XYZRHW | D3DFVF_TEX1)
	struct CUSTOMVERTEX { FLOAT x, y, z, rhw; D3DCOLOR color; }; // custom vertex struct contains position and texture data for more complex renders
	#define CUSTOMFVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// draw 
class Draw
{
private:
	std::vector<CUSTOMVERTEX>* vertices;
	int* triCount;
	myVec2* windowSize;

public:
	Draw(std::vector<CUSTOMVERTEX>* vertices, int* triCount);
	~Draw();

	void setWindowSize(myVec2* windowSize);
	void DrawSquare(myVec2 pos, int size, D3DCOLOR color);
	// draw a vector from the start and end points
	void drawVector(myVec2 start, myVec2 end, D3DCOLOR color);

	void drawCharacter(myVec2 pos, char in, D3DCOLOR color, int size);
	void drawString(myVec2 pos, std::string in, D3DCOLOR color, int size);

	//void DrawCircle(myVec2 Pos,);
};

