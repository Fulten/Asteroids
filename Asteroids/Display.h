#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include "Draw.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "Winmm.lib")

class Display : public Draw
{
private:
	int triCount, drawSize;
	void InitTextures();
	std::vector<CUSTOMVERTEX> vertices;//utilize a vector to allow the creation of dynamic objects
	std::vector<int> textures;

protected:
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddev;
	LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL; // buffer for writing to graphics card
	std::vector<LPDIRECT3DTEXTURE9> textureList; // texture list stores currently used textures

	// draws a textured square with given parameters and assigns it to certices
	void dTexSquare(float x1, float y1, float x2, float y2, bool reverse);
	void dTexTile(float x, float y, float width, float hight);
	void cleanD3D(); // clears out memeory for the deconstructor

public:
	Display(HWND& hWnd, int dSize); // constructor initilizes direct 3d devices
	~Display(); // deconstructor

	void addTex(float x1, float y1, float width1, float hight1, int texture1); // adds a new textured square to the pipeline

	void VecDraw(); // renders a target scene using colored vectors
	void VecBuild(); // uses defined vertices to render a target scene

	void TexDraw(); // renders our target scene
	void TexBuild(); // using created vertices, loads them into graphics memory

};

