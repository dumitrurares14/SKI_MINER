#include "game.h"
#include "surface.h"
#include "template.h"
#include <SDL.h>
#include <cstdio> //printf
#include <iostream>
#include <string>
#include <Windows.h>

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
	Surface wow(200, 200);
	
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	void Game::Init()
	{
		snow.CreateSnowEffect();
		snow.CreateGroundSnow();
		oreGenerator.InitOreGeneration();
		env.InitTreeGeneration();
		env.InitPickUps();
		env.InitNpc();
		std::cout << "started19!";
	}
	
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
		player.SaveScore();
	}
	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		bool pressFlag = KeyPressed(keyPressed);
		screen->Clear(0x7393c7);
		snow.UpdateSnowEffect(screen, 0xffffff, deltaTime);

		if (!isPaused && !player.gameOver) {
			snow.UpdateGroundSnow((int)player.x + 10, (int)player.y, screen, 0xd6d6f5,deltaTime);
			player.SkiMovement(screen, 5, 200 , mousex, mousey, keyPressed,deltaTime);
			oreGenerator.UpdateOres(screen, player, pressFlag && (keyPressed == 20 || keyPressed == 8), keyPressed,deltaTime);
			env.UpdateTrees(screen, player,deltaTime);
			env.UpdateHealthUp(screen, player, deltaTime);
			env.UpdateNpc(screen, player, deltaTime);
			player.ShowScore(screen);
			player.ShowHealth(screen);
		}
		else if(isPaused) {
			pauseSprite.Draw(screen, (ScreenWidth / 2) - (pauseSprite.GetWidth() / 2), (ScreenHeight / 2) - (pauseSprite.GetHeight() / 2));

			screen->Print("SCORE:", (ScreenWidth/2)-30, (ScreenHeight/2)+100, 0x4287f5);
			player.ShowScore(screen, (ScreenWidth / 2)+10, (ScreenHeight / 2)+100 , 0x4287f5);
		}
		else if (player.gameOver) {
			gameOverSprite.Draw(screen, (ScreenWidth / 2) - (gameOverSprite.GetWidth() / 2), (ScreenHeight / 2) - (gameOverSprite.GetHeight() / 2)-40);
			restartSprite.Draw(screen, (ScreenWidth / 2) - (gameOverSprite.GetWidth() / 2), (ScreenHeight / 2) - (gameOverSprite.GetHeight() / 2)+160);

			screen->Print("SCORE:", (ScreenWidth / 2) - 100, (ScreenHeight / 2) + 150, 0x4287f5);
			player.ShowScore(screen, (ScreenWidth / 2) - 60, (ScreenHeight / 2) + 150, 0x4287f5);

			screen->Print("HIGH SCORE:", (ScreenWidth / 2) + 40, (ScreenHeight / 2) + 150, 0x4287f5);
			player.ShowHighScore(screen, (ScreenWidth / 2) + 110, (ScreenHeight / 2) + 150, 0x4287f5);
		}

		if (pressFlag && keyPressed == 19 && isPaused == false && !player.gameOver) {
			isPaused = true;
			PlaySound(NULL, NULL, SND_ASYNC);
		}
		else if (pressFlag && keyPressed == 19 && isPaused == true && !player.gameOver) {
			isPaused = false;
		}

		if (pressFlag && keyPressed == 40 && player.gameOver) {
			player.gameOver = false;
			player.PlayerReset();
			snow.CreateSnowEffect();
			snow.CreateGroundSnow();
			oreGenerator.InitOreGeneration();
			env.InitTreeGeneration();
			env.InitPickUps();
			env.InitNpc();
		}
	}

};