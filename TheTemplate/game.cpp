#include "game.h"
#include "surface.h"
#include "template.h"
#include <SDL.h>
#include <cstdio> //printf
#include <iostream>
#include <string>
#include <Windows.h>


#define SNOWDENSITY 200
namespace Tmpl8
{
	Snow snow;
	Player player;
	OreGenerator oreGenerator;
	Environment env;
	bool isPaused;
	Sprite pauseSprite(new Surface("assets/pause.png"), 1);
	Sprite gameOverSprite(new Surface("assets/game_over.png"), 1);
	Sprite restartSprite(new Surface("assets/restart_info.png"), 1);


	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	void Game::Init()
	{
		snow.CreateSnowEffect();
		snow.CreateGroundSnow();
		oreGenerator.InitOreGeneration();
		env.InitTreeGeneration();
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
		if (!isPaused && !player.gameOver) {
			screen->Clear(0x7393c7);
			snow.UpdateGroundSnow((int)player.x + 10, (int)player.y, screen, 0xd6d6f5,deltaTime);
			snow.UpdateSnowEffect(screen, 0xffffff,deltaTime);
			player.SkiMovement(screen, 5, 200 , mousex, mousey, keyPressed,deltaTime);
			oreGenerator.UpdateOres(screen, player, foo && (keyPressed == 20 || keyPressed == 8), keyPressed,deltaTime);
			env.UpdateTrees(screen, player,deltaTime);
			player.ShowScore(screen);
			player.ShowHealth(screen);
		}
		else if(isPaused) {
			screen->Clear(0x7393c7);
			snow.UpdateSnowEffect(screen, 0xffffff,deltaTime);
			pauseSprite.Draw(screen, (ScreenWidth / 2) - (pauseSprite.GetWidth() / 2), (ScreenHeight / 2) - (pauseSprite.GetHeight() / 2));
		}
		else if (player.gameOver) {
			screen->Clear(0x7393c7);
			snow.UpdateSnowEffect(screen, 0xffffff,deltaTime);
			gameOverSprite.Draw(screen, (ScreenWidth / 2) - (gameOverSprite.GetWidth() / 2), (ScreenHeight / 2) - (gameOverSprite.GetHeight() / 2)-40);
			restartSprite.Draw(screen, (ScreenWidth / 2) - (gameOverSprite.GetWidth() / 2), (ScreenHeight / 2) - (gameOverSprite.GetHeight() / 2)+160);
		}

		if (foo && keyPressed == 19 && isPaused == false && !player.gameOver) {
			isPaused = true;
			PlaySound(NULL, NULL, SND_ASYNC);
		}
		else if (foo && keyPressed == 19 && isPaused == true && !player.gameOver) {
			isPaused = false;
		}

		if (foo && keyPressed == 40 && player.gameOver) {
			player.gameOver = false;
			player.PlayerReset();
			snow.CreateSnowEffect();
			snow.CreateGroundSnow();
			oreGenerator.InitOreGeneration();
			env.InitTreeGeneration();
		}
	}

};