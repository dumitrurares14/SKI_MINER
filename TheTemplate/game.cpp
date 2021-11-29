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
	Sprite playerSprite(new Surface("assets/ctankbase.tga"), 16);
	Player player(&playerSprite);
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
		//player.playerTexture = playert;
		screen->Clear(0x7393c7);
		snow.UpdateGroundSnow(player.x, player.y, screen, 0xd6d6f5);
		snow.UpdateSnowEffect(screen, 0xffffff);

		player.SkiMovement(screen, 60, 1.5f, mousex, mousey);
	}
};