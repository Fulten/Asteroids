#pragma once


#include "Display.h"
#include "Triangle.h"
#include "Sound.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Bullet.h"

class Game : public Display
{
private:
	// struct contains pos and parentSize of asteroids the will be spawned at the end of the current frame
	struct ToBeSpawned
	{
		myVec2 pos;
		int parentSize;

		ToBeSpawned(myVec2 pos, int parentSize) :
			pos(pos),
			parentSize(parentSize)
		{}
	};

	myVec2 windowSize;
	Sound sound;

	// game objects
	Ship ship;
	Triangle engine;
	std::vector<Asteroid*> asteroids;
	std::vector<ToBeSpawned> toBeSpawned;
	std::vector<Bullet*> bullets;

	// count delays
	int count = 0;
	int dCount = 0;
	int startDelay = 0;
	int asteroidCount = 0;
	int score = 0;
	int aniCount = 0;

	// time delay from last frame, 1 deltaTime == 1 second
	float* deltaTime;

	// debug flags
	bool debug = false;
	bool debugBorder = false;
	bool asteroidsMove = true;
	bool started = false;
	bool paused = false;
	bool urf = false;

	// flags used to control sound playback
	bool thrusting = false;
	bool blasterFired = false;
	bool explosion = false;

	// flage is triped when the player contacts an asteroid
	bool gameOver = false;

	// variables for controling player position and velocity
	myVec2 vel;
	float angle = 270;

public:
	Game(HWND& hWnd, int screenW, int screenH, float* deltaTime, int drawSize) : 
		windowSize((int)(screenW/ drawSize) - 3, (int)(screenH / drawSize) - 7),
		Display(hWnd, drawSize),
		deltaTime(deltaTime),
		ship((windowSize * 0.5), 3),
		engine(ship.getPos()),
		vel(0, 0)
	{
		// initilize menu state
		asteroids.push_back(new Asteroid(myVec2(40, 30), 2));
		asteroids.push_back(new Asteroid(myVec2(0, 20), 3));
		asteroids.push_back(new Asteroid(myVec2(50, 10), 1));
		asteroids.push_back(new Asteroid(myVec2(20, 30), 1));
		asteroids.push_back(new Asteroid(myVec2(50, 10), 3));
		asteroids.push_back(new Asteroid(myVec2(30, 30), 0, D3DCOLOR_XRGB(150, 100, 100)));
		asteroids.push_back(new Asteroid(myVec2(0, 20), 0, D3DCOLOR_XRGB(150, 100, 100)));

		Shape::setWindow(&windowSize);
		Display::Draw::setWindowSize(&windowSize);
		srand(29); // seed starts the asteroids moving away from player
		
	}


	void go(); // used by the main while loop to start the next frame
	// main game functions
	void updateGame(); // preforms calculations for game mechanics
	void updateFrame(); // translates current game state into gfx
	void doMovement(); // contains movement functions
	void doCollision(); // contains collision functions 
	void doSound();
	void doCleanUp(); // handle deleting of objects flaged as remove
	void reset(); // resets the game when the player's ship crashes

	// translates a shapes vertices into vecters representing its sides
	void drawShape(Shape* shape, D3DCOLOR color);

	// spawn object functions
	void SpawnAsteroids();
	void spawnChildAsteroids();
	void spawnBullet();
	void menu();
};