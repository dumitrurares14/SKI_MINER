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
	Sprite playerSprite(new Surface("assets/miner.png"),1);
	Player player(&playerSprite);
	OreGenerator oreGenerator;

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
		snow.UpdateGroundSnow(player.x+10, player.y, screen, 0xd6d6f5);
		snow.UpdateSnowEffect(screen, 0xffffff);
		player.SkiMovement(screen, 5,3, mousex, mousey);
		oreGenerator.UpdateOres(screen,player, foo,keyPressed);
		

		
	}
};