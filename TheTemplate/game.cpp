#include "game.h"
#include "surface.h"
#include "template.h"
#include <SDL.h>
#include <cstdio> //printf
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
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		GAMESPEED = 80.0f;
		printf("STARTED 1.0.0\n");
		printf("Gamespeed:%f\n", GAMESPEED);
		snow.CreateSnowEffect();
		snow.CreateGroundSnow();
		oreGenerator.InitOreGeneration();
		env.InitTreeGeneration();
		env.InitPickUps();
		env.InitNpc();
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
		float dt = deltaTime / 1000;
		screen->Clear(0x7393c7);
		if (!isPaused && !player.gameOver) {
			//Game loop when not paused or game over
			//Updating ground snow, player, ores, env, generators
			//showing UI
			snow.UpdateGroundSnow((int)player.x + 10, (int)player.y, screen, 0xc7cfd6,dt);
			player.SkiMovement(screen, 5, 200 , mousex, mousey, keyPressed, dt);
			oreGenerator.UpdateOres(screen, player, pressFlag && (keyPressed == 20 || keyPressed == 8), keyPressed, dt);
			env.UpdateTrees(screen, player, dt);
			env.UpdateHealthUp(screen, player, dt);
			env.UpdateNpc(screen, player, dt);
			//UI
			player.ShowScore(screen);
			player.ShowHealth(screen);
			//PAUSE INPUT
			if (pressFlag && keyPressed == 19) {
				isPaused = true;
			}
		}
		else if(isPaused) {
			//PAUSED
			//Drawing pause menu with pause sprite and showing score
			pauseSprite.Draw(screen, (ScreenWidth / 2) - (pauseSprite.GetWidth() / 2), (ScreenHeight / 2) - (pauseSprite.GetHeight() / 2));

			screen->Print("SCORE:", (ScreenWidth/2)-30, (ScreenHeight/2)+100, 0x326194);
			player.ShowScore(screen, (ScreenWidth / 2)+10, (ScreenHeight / 2)+100 , 0x326194);

			//UNPAUSE INPUT
			if (pressFlag && keyPressed == 19 && !player.gameOver) {
				isPaused = false;
			}
		}
		else if (player.gameOver) {
			//GAME OVER
			//Drawing game over screen with game over sprites, showing current score and high score
			gameOverSprite.Draw(screen, (ScreenWidth / 2) - (gameOverSprite.GetWidth() / 2), (ScreenHeight / 2) - (gameOverSprite.GetHeight() / 2)-40);
			restartSprite.Draw(screen, (ScreenWidth / 2) - (gameOverSprite.GetWidth() / 2), (ScreenHeight / 2) - (gameOverSprite.GetHeight() / 2)+160);

			screen->Print("SCORE:", (ScreenWidth / 2) - 100, (ScreenHeight / 2) + 150, 0x326194);
			player.ShowScore(screen, (ScreenWidth / 2) - 60, (ScreenHeight / 2) + 150, 0x326194);

			screen->Print("HIGH SCORE:", (ScreenWidth / 2) + 40, (ScreenHeight / 2) + 150, 0x326194);
			player.ShowHighScore(screen, (ScreenWidth / 2) + 110, (ScreenHeight / 2) + 150, 0x326194);

			//RESTART INPUT
			//If enter is pressed restart everything
			if (pressFlag && keyPressed == 40) {
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
		snow.UpdateSnowEffect(screen, 0xffffff, deltaTime);//Snow weather effect at all time
	}
};