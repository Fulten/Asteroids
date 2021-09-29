#include "Game.h"

// used by the main while loop to start the next frame
void Game::go()
{
	// increment the animation cycle  note: currently bound to the frame rate should be based off of time passed
	aniCount++;
	if (aniCount > 40) aniCount = 0;

	// run and render the menu until the game has started
	if (started)
		updateGame();
	else
		menu();

	doSound();
	updateFrame();
	Display::VecBuild();
	Display::VecDraw();
}

// translates current game state into gfx
void Game::updateFrame()
{ 
	// renders debug information
	if (debugBorder == true)
	{
		D3DCOLOR shipcolor = D3DCOLOR_XRGB(255, 255, 0);
		D3DCOLOR asteroidcolor = D3DCOLOR_XRGB(180, 50, 0);
		ship.setColor(shipcolor);

		// draws a boarder of the play area 
		for (int x = 0; x < windowSize.x; x++)
		{
			for (int y = 0; y < windowSize.y; y++)
			{
				if (y == (int)windowSize.y - 1 || x == (int)windowSize.x - 1)
					Display::Draw::DrawSquare(myVec2(x, y), 1, D3DCOLOR_XRGB(255, 255, 0));

				if (y == 0 || x == 0)
					Display::Draw::DrawSquare(myVec2(x, y), 1, D3DCOLOR_XRGB(255, 255, 0));
			}
		}

		// player collision with asteriods is highlighted in green
		/*note: obsolete now that the player simply dies when contacting an asteroid
		for (int i = 0; i < asteroids.size(); i++)
		{
			asteroids.at(i)->setColor(asteroidcolor);
			if (ship.checkCollision(*asteroids.at(i)))
				ship.setColor(D3DCOLOR_XRGB(0, 255, 0));
		}*/

		// bullet collision with asteriods is highlighted in green 
		/*note: This is now obsolete as collision detection removes asteriods now
		for (int i = 0; i < bullets.size(); i++)
		{
			for (int f = 0; f < asteroids.size(); f++)
			{
				if (bullets.at(i)->checkCollision(*asteroids.at(f)))
					asteroids.at(f)->setColor(D3DCOLOR_XRGB(0, 255, 0));
			}
		}*/
	}
	
	
	if (started) // main rendering code
	{
		// draw bullets in list
		for (int i = 0; i < bullets.size(); i++)
		{	
			drawShape(bullets.at(i), bullets.at(i)->getColor());
		}

		// draw a fire jet at the back of the player's ship when holding forward
		if (thrusting)
		{
			// sets the engins color and size to its default value
			engine.setColor(D3DCOLOR_XRGB(200, 50, 0));
			engine.setLength(5);

			// alternates the engine's color and size every 5 or so ticks
			if (thrusting && aniCount % 10 > 5)
			{
				engine.setColor(D3DCOLOR_XRGB(255, 150, 100));
				engine.setLength(3);
			}

			// draw the players engine
			drawShape(&engine, engine.getColor());
		}
			
		// draw player's ship
		drawShape(&ship, ship.getColor());

		// draw asteroids in list
		for (int i = 0; i < asteroids.size(); i++)
		{
			drawShape(asteroids.at(i), asteroids.at(i)->getColor());
		}

		// draw hud elements i.e the current score
		std::string hud = "SCORE:" + std::to_string(score);
		Display::drawString(myVec2(2, 3), hud, D3DCOLOR_XRGB(100, 50, 50), 1);
		Display::drawString(myVec2(1, 2), hud, D3DCOLOR_XRGB(255, 50, 50), 1);
		
		// in debug mode display the current count of asteroids
		if (debug)
			Display::drawString(myVec2(0, 10), "ASTEROIDS:" + std::to_string(asteroidCount), D3DCOLOR_XRGB(255, 255, 255), 1);

		if (paused) // render a big paused if the game is paused
		{
			hud = "PAUSED";
			Display::drawString(myVec2(39, windowSize.y / 2 - 7), hud, D3DCOLOR_XRGB(50, 10, 10), 2);
			Display::drawString(myVec2(38, windowSize.y / 2 - 8), hud, D3DCOLOR_XRGB(255, 100, 100), 2);
		}
	}
	else // handels drawing the start screen when first started or when the player dies
	{
		// draw asteroids in list
		for (int i = 0; i < asteroids.size(); i++)
		{
			drawShape(asteroids.at(i), asteroids.at(i)->getColor());
		}

		drawShape(&ship, ship.getColor());

		Display::drawString(myVec2(16, windowSize.y / 4 - 4), "ASTEROIDS", D3DCOLOR_XRGB(50, 0, 0), 2);
		Display::drawString(myVec2(15, windowSize.y / 4 - 5), "ASTEROIDS",  D3DCOLOR_XRGB(255, 0, 0) , 2);
		//Display::drawString(myVec2(12, windowSize.y / 3 - 5), "BY ROBERT REINHOLDT", D3DCOLOR_XRGB(255, 255, 255), 1);
		Display::drawString(myVec2(1, windowSize.y / 2 - 5), "PRESS SPACE", aniCount % 20 > 10 ? D3DCOLOR_XRGB(255, 150, 100) : D3DCOLOR_XRGB(255, 100, 100), 2);

		std::string hud = "LAST SCORE:" + std::to_string(score);

		if(score > 0)
			Display::drawString(myVec2(33, windowSize.y / 3 - 5), hud, aniCount % 20 > 10 ? D3DCOLOR_XRGB(50, 200, 100) : D3DCOLOR_XRGB(50, 255, 100), 1);
	}
}

// preforms calculations for game mechanics
void Game::updateGame()
{
	thrusting = false;

	// counters
	if (count > 0) count--;
	if (dCount > 0) dCount--;

	if (!paused)
	{
		// players ship controls
		// turning in degrees adjusted for time
		if (GetAsyncKeyState('D')) angle += 20 * *deltaTime;
		if (GetAsyncKeyState('A')) angle -= 20 * *deltaTime;
		ship.setRotationDeg(angle);
		engine.setRotation(angle);
		
		// adds space drag, drag in space!
		vel -= vel * 0.02 * *deltaTime;

		myVec2 temp(0,0);
		// forward velocity is aligned with the direction the ship is facing
		if (GetAsyncKeyState('W'))
		{
			temp.x = cos(ship.getRotation());
			temp.y = sin(ship.getRotation());
			thrusting = true;
		}

		// adjust change in acceleration
		temp *= 0.5;

		vel += temp * *deltaTime;

		// clamp our velocity vector
		if (vel.getLengthSq() > 1)
			vel.Normalize();

		// fire bullets
		if (GetAsyncKeyState(' ') && count == 0)
		{
			spawnBullet();
			count = 30;
		}
	}
	
	// pause the game when escape is presed
	if (GetAsyncKeyState(VK_ESCAPE) && dCount == 0)
	{
		paused = !paused;
		dCount = 20;
	}

	// debug keys
	if (debug)
	{
		// highlight the debug boarder of the playable area
		// note: not as anymore usful as the area is properly aligned with the screen now
		if (GetAsyncKeyState('Y') && dCount == 0)
		{
			debugBorder = !debugBorder;
			dCount = 20;
		}

		// toggle movement of asteroids 
		if (GetAsyncKeyState('T') && dCount == 0)
		{
			asteroidsMove = !asteroidsMove;
			dCount = 20;
		}

		// toggle ultra rapid fire mode
		if (GetAsyncKeyState('U') && dCount == 0)
		{
			urf = !urf;
			dCount = 20;
		}

		if (urf)
			count = 0;
	}
	
	// spawns asteroids when no more asteroids are in play
	if (asteroidCount <= 0)
	{ SpawnAsteroids(); }

	// game functions
	if (!paused)
		doMovement();

	doCollision();
	spawnChildAsteroids();

	// reset the game when the player dies
	if (gameOver)
	{
		reset();
		// initilize asteroids in the menue
		asteroids.push_back(new Asteroid(myVec2(40, 30), 2));
		asteroids.push_back(new Asteroid(myVec2(-50, 20), 3));
		asteroids.push_back(new Asteroid(myVec2(50, -10), 1));
		asteroids.push_back(new Asteroid(myVec2(20, 30), 1));
		asteroids.push_back(new Asteroid(myVec2(-50, 10), 3));
		asteroids.push_back(new Asteroid(myVec2(30, 30), 0, D3DCOLOR_XRGB(150, 100, 100)));
		asteroids.push_back(new Asteroid(myVec2(0, 20), 0, D3DCOLOR_XRGB(150, 100, 100)));
		started = false;
		startDelay = 50;
	}

	// cleanup objects that are at the end of their life
	doCleanUp();
}

// contains movement functions
void Game::doMovement()
{
	ship.move(vel);
	engine.setPos(ship.getPos());

	// do movement for asteroids
	if(asteroidsMove)
		for (int i = 0; i < asteroids.size(); i++)
		{
			asteroids.at(i)->move(0.5f);
		}

	// do movement for bullets
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets.at(i)->move(2);
	}
}

// contains collision functions 
void Game::doCollision()
{
	// player collision with asteriods
	for (int i = 0; i < asteroids.size(); i++)
	{
		if (ship.checkCollision(*asteroids.at(i)))
		{
			gameOver = true;
			explosion = true;
		}
	}

	// bullet collision with asteriods
	for (int i = 0; i < bullets.size(); i++)
	{
		for (int f = 0; f < asteroids.size(); f++)
		{
			if (bullets.at(i)->checkCollision(*asteroids.at(f)))
			{
				asteroids.at(f)->remove();
				bullets.at(i)->remove();
				explosion = true;
			}
		}
	}
}

// contains processing of sound flags
void Game::doSound()
{
	sound.thruster(thrusting);

	if (blasterFired)
	{
		blasterFired = false;
		sound.blaster();
	}

	if (explosion)
	{
		explosion = false;
		sound.explosion();
	}

}

// handle deleting of objects flaged as remove
void Game::doCleanUp()
{
	// delete bullets that are flagged for removal
	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets.at(i)->getRemove() == true)
		{
			bullets.at(i)->~Bullet();
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}

	// delete asteroids that are flagged for removal
	for (int i = 0; i < asteroids.size(); i++)
	{
		if (asteroids.at(i)->getRemove() == true)
		{
			toBeSpawned.push_back(ToBeSpawned(asteroids.at(i)->getPos(), asteroids.at(i)->getSize()));
			asteroids.at(i)->~Asteroid();
			asteroids.erase(asteroids.begin() + i);
			i--;
			score++;
			asteroidCount--;
		}
	}
}

// clears all current game objects and resets player position and rotation
void Game::reset()
{
	// reset player and values
	ship.setPos(windowSize * 0.5);
	vel = myVec2(0, 0);
	angle = 270;
	thrusting = false;

	// delete all bullets 
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets.at(i)->~Bullet();
		bullets.erase(bullets.begin() + i);
		i--;
	}

	// delete all asteroids
	for (int i = 0; i < asteroids.size(); i++)
	{
		asteroids.at(i)->~Asteroid();
		asteroids.erase(asteroids.begin() + i);
		i--;
	}

	asteroidCount = 0;
	gameOver = false;
}

// handels the start command and movment calculations needed for the start menu
void Game::menu()
{
	if (startDelay > 0) startDelay--;
	ship.setRotationDeg(angle);

	for (int i = 0; i < asteroids.size(); i++)
	{
		asteroids.at(i)->move(0.5f);
	}
	// start when the player presses space
	if (GetAsyncKeyState(' ') && startDelay == 0)
	{
		started = true;
		// reset the game to its defualt state
		reset();
		score = 0;
	}
}

// translates a shapes vertices into vecters representing its sides and passes them to Display::Draw vector
void Game::drawShape(Shape* shape, D3DCOLOR color)
{
	std::vector<myVec2> temp;

	// the pair 0, 1 and , last -1, last are repeated
	shape->GetPoints(temp);

	if (temp.size() < 2) // throw exception if drawShape is attempted with fewer than 2 points
		throw "Draw Shape requires at least 2 defined vertices";

	for (int i = 1; i < temp.size(); i++)
	{
		Display::Draw::drawVector(temp.at(i - 1), temp.at(i), color);
	}
}

// spawns 2 large asteriods some distance from the player to either side
void Game::SpawnAsteroids()
{
	asteroids.push_back(new Asteroid(ship.getPos() + myVec2(50, 0), 3));
	asteroids.push_back(new Asteroid(ship.getPos() + myVec2(-50, 0), 3));
	asteroidCount += 2;
}

// spawns asteroids based on the information contained in the toBeSpawned list
void Game::spawnChildAsteroids()
{
	for (int i = 0; i < toBeSpawned.size(); i++)
	{
		if (toBeSpawned.at(i).parentSize > 0)
		{
			// alter the color of the smallest sized asteroid
			D3DCOLOR color = D3DCOLOR_XRGB(180, 50, 0);
			if(toBeSpawned.at(i).parentSize == 1)
				color = D3DCOLOR_XRGB(150, 100, 100);

			// spawns 2 asteroids at the position of the original parent
			asteroids.push_back(new Asteroid(toBeSpawned.at(i).pos + myVec2(5, 0), toBeSpawned.at(i).parentSize - 1, color));
			asteroids.push_back(new Asteroid(toBeSpawned.at(i).pos + myVec2(-5, 0), toBeSpawned.at(i).parentSize - 1, color));
			
			asteroidCount += 2;
		}
	}
	// clear the toBeSpawned list as we have finished using it
	toBeSpawned.clear();
}

// spawns a player bullet at the players position and rotation
void Game::spawnBullet()
{
	bullets.push_back(new Bullet(ship.getPos(), ship.getRotation()));
	blasterFired = true;
}