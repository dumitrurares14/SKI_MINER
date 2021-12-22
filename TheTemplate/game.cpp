#include "game.h"
#include "surface.h"
#include "template.h"
#include <SDL.h>
#include <cstdio> //printf
#include <iostream>

#define SNOWDENSITY 200
namespace Tmpl8
{
	Snow snow;
	Player player;
	OreGenerator oreGenerator;
	bool isPaused;

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		snow.CreateSnowEffect();
		snow.CreateGroundSnow();
		oreGenerator.InitOreGeneration();
		std::cout << "started14!";

	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
		
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------


	void Game::Tick(float deltaTime)
	{
		bool foo = KeyPressed(keyPressed);
		//player.playerTexture = playert;
		screen->Clear(0x7393c7);
		if (!isPaused) {
			snow.UpdateGroundSnow(player.x + 10, player.y, screen, 0xd6d6f5);
			snow.UpdateSnowEffect(screen, 0xffffff);
			player.SkiMovement(screen, 5, 3, mousex, mousey, keyPressed);
			oreGenerator.UpdateOres(screen, player, foo && (keyPressed == 20 || keyPressed == 8), keyPressed);
		}

		if (foo && keyPressed == 19 && isPaused == false) {
			isPaused = true;
		}
		else if (foo && keyPressed == 19 && isPaused == true) {
			isPaused = false;
		}
		
	}
};