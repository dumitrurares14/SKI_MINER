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

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		snow.CreateSnowEffect();
		snow.CreateGroundSnow();
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
		screen->Clear(0);
		snow.UpdateGroundSnow(mousex, mousey, screen, 0xd6d6f5);
		snow.UpdateSnowEffect(screen, 0xffffff);

	}
};