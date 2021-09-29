// Name: Robert Reinholdt Date: 5/21/2021
// purpose: create a simple 2d shooter using directx and object oriented programing

#include <iostream>
#include <Windows.h>
#include <timeapi.h>
#include <string>
#include "Game.h"

int DrawSize = 5; // controls the size tiles are scaled to
// hight and width of window
int windowWidth = 795;
int windowHight = 799;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// handle for the window
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// Clear out the window class of use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(101));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";
	wc.hIconSm = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	// register windows class
	RegisterClassEx(&wc);

	// create the window and use the restult as the handle
	hWnd = CreateWindowEx(NULL,
		L"WindowClass1", // name of the window class
		L"Asteroids fps: |", // title of the window
		WS_OVERLAPPEDWINDOW, // window style
		300, // x position of the window
		100, // y position of the window
		windowWidth, // width of the window
		windowHight, // hight of the window 
		NULL, // we have no parent window
		NULL, // we arn't useing menus
		hInstance, // application handle
		NULL); // used with multiple windows

	// display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	MSG msg;

	//initilize the game
	double lastTimeFrame = (double)timeGetTime();
	double lastAnimation = (double)timeGetTime();
	float deltaTime;

	float fps = 0;

	Game game(hWnd, windowWidth, windowHight, &deltaTime, DrawSize);

	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); // translate keystroke messages into a usable format
			DispatchMessageW(&msg); // send the message to the WindowProc function

			if (msg.message == WM_QUIT)
				goto exit;
		}

		deltaTime = (float)((double)timeGetTime() - lastTimeFrame) / 50;
		lastTimeFrame = (double)timeGetTime();


		fps = 20 / deltaTime;

		std::string temp = "Asteroids | fps: " + std::to_string(fps);
		SetWindowTextA(hWnd, temp.c_str());

		game.go();
	}

exit:

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for each message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		//close application completely
		PostQuitMessage(0);
		return 0;
	} break;

	default:
	{

	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}