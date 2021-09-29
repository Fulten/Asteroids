#include "Display.h"

// constructor
Display::Display(HWND& hWnd, int dSize) :
	Draw(&vertices, &triCount),
	drawSize(dSize),
	triCount(0)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION); // create the Direct 3d Interface

	D3DPRESENT_PARAMETERS d3dpp; // create a stuct to hold various device information

	ZeroMemory(&d3dpp, sizeof(d3dpp)); // clear out the struct for use

	d3dpp.Windowed = TRUE; // Sets the program to windowed, rather than fullscreen
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // discard old frames
	d3dpp.hDeviceWindow = hWnd;

	// create a device class using this information and from the d3dpp struct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	//initilize textures
	//InitTextures();
}

Display::~Display()
{
	cleanD3D();
}

void Display::cleanD3D() // clears memory locations
{
	v_buffer->Release(); // close and release the vertex buffer
	d3ddev->Release(); // close and release the 3d device
	d3d->Release(); // close and release Direct 3d
}

void Display::InitTextures()
{
	LPDIRECT3DTEXTURE9* temp = new LPDIRECT3DTEXTURE9;
	D3DXCreateTextureFromFile(d3ddev, L"sprites\\Test.png", temp); // 0
	textureList.push_back(*temp);
}

// renders our target scene using colored vertices
void Display::VecDraw()
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->BeginScene(); // begins the 3d scene

	d3ddev->SetFVF(CUSTOMFVF); // set vertex format
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));// select the vertex buffer
	for (int i = 0; i < triCount / 2; i++)
	{
		d3ddev->DrawPrimitive(D3DPT_TRIANGLEFAN, i * 4, 2); // copy the vertex buffer to back buffer
	}
	
	d3ddev->EndScene(); // ends scene
	d3ddev->Present(NULL, NULL, NULL, NULL); // displays the created frame 
	v_buffer->Release(); // releases the video buffer after its been used

	//textures.clear();
	triCount = 0; // tracks the number of triangles stored as vertices to the buffer
}

// using created colored vertices, loads them into graphics memory
void Display::VecBuild()
{

	DrawSquare(myVec2(-1, -1), 1, D3DCOLOR_XRGB(0, 0, 0));
	//dynamicly creates the vertex data and stores the pointer into the v_buffer, which is created globally
	d3ddev->CreateVertexBuffer(vertices.size() * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid; // the void pointer

	// adjust's vertex alignment to better line up with pixels
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices.at(i).x *= drawSize;
		vertices.at(i).x -= 0.5;

		vertices.at(i).y *= drawSize;
		vertices.at(i).y -= 0.5;
	}


	v_buffer->Lock(0, 0, (void**)&pVoid, 0); // lock the vertex buffer
	memcpy(pVoid, vertices.data(), vertices.size() * sizeof(CUSTOMVERTEX)); //copys the vertex data from vertices into video memory
	v_buffer->Unlock(); // unlock the vertex buffer

	vertices.clear(); // empty the vector after we processed its data
}

// renders our target scene using textured vertices
void Display::TexDraw()
{
	// clear the window to  black
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	d3ddev->BeginScene(); // begins the 3d scene
	// do 3D rendering on the back buffer

	d3ddev->SetFVF(TEXTUREDFVF); // set vertex format
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(TEXTUREDVERTEX));// select the vertex buffer

	d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//setting render state to allow alpha
	d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//controls actual alpha blending
	d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (int i = 0; i < textures.size(); i++)
	{
		d3ddev->SetTexture(0, textureList.at(textures.at(i)));
		d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, i * 6, 2); // copy the vertex buffer to back buffer
	}

	//d3ddev->SetTexture(0, textureList.at(1));
	//d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triCount); // copy the vertex buffer to back buffer

	d3ddev->EndScene(); // ends scene
	d3ddev->Present(NULL, NULL, NULL, NULL); // displays the created frame 
	v_buffer->Release(); // releases the video buffer after its been used

	textures.clear();
	triCount = 0; // tracks the number of triangles stored as vertices to the buffer
}

// using created Textured vertices, loads them into graphics memory
void Display::TexBuild()
{
	//dynamicly creates the vertex data and stores the pointer into the v_buffer, which is created globally
	d3ddev->CreateVertexBuffer(vertices.size() * sizeof(TEXTUREDVERTEX),
		0,
		TEXTUREDFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid; // the void pointer

	// adjust's vertex alignment to better line up with pixels
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices.at(i).x -= 0.5;
		vertices.at(i).y -= 0.5;
	}

	v_buffer->Lock(0, 0, (void**)&pVoid, 0); // lock the vertex buffer
	memcpy(pVoid, vertices.data(), vertices.size() * sizeof(TEXTUREDVERTEX)); //copys the vertex data from vertices into video memory
	v_buffer->Unlock(); // unlock the vertex buffer

	vertices.clear(); // empty the vector after we processed its data
}

/*
void Display::dTexSquare(float x1, float y1, float x2, float y2, bool reverse)
{

	if (reverse) // reverses the texels across the y axis allowing mirrored textures
	{
		//first triangle
		vertices.push_back({ x1, y1, 0.5f, 1.0f,  1.0f, 0.0f });
		vertices.push_back({ x2, y1, 0.5f, 1.0f,  0.0f, 0.0f });
		vertices.push_back({ x1, y2, 0.5f, 1.0f,  1.0f, 1.0f });
		triCount++;

		//second triangle						
		vertices.push_back({ x2, y1, 0.5f, 1.0f,  0.0f, 0.0f });
		vertices.push_back({ x2, y2, 0.5f, 1.0f,  0.0f, 1.0f });
		vertices.push_back({ x1, y2, 0.5f, 1.0f,  1.0f, 1.0f });
		triCount++;
	}
	else
	{
		//first triangle
		vertices.push_back({ x1, y1, 0.5f, 1.0f,  0.0f, 0.0f });
		vertices.push_back({ x2, y1, 0.5f, 1.0f,  1.0f, 0.0f });
		vertices.push_back({ x1, y2, 0.5f, 1.0f,  0.0f, 1.0f });
		triCount++;

		//second triangle						 
		vertices.push_back({ x2, y1, 0.5f, 1.0f,  1.0f, 0.0f });
		vertices.push_back({ x2, y2, 0.5f, 1.0f,  1.0f, 1.0f });
		vertices.push_back({ x1, y2, 0.5f, 1.0f,  0.0f, 1.0f });
		triCount++;
	}
}

// draws a tile scaled by defeined hight and width in pixels
void Display::dTexTile(float x, float y, float width, float hight) 
{
	dTexSquare(x * drawSize, y * drawSize, (x + width) * drawSize, (y + hight) * drawSize, false);
}

// adds a new node to the end of the list
void Display::addTex(float x, float y, float width, float hight, int texture)
{
	textures.push_back(texture);
	dTexTile(x, y, width, hight);
}

*/