#pragma once
#include "surface.h"
#include "template.h"
#include <SDL.h>
#include <cstdio> //printf


namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp(int button) { mouseButton = 0; }
	void MouseDown(int button) { mouseButton = button; }
	void MouseMove(int x, int y) { mousex = x, mousey = y; }
	void KeyUp( int key ) { /* implement if you want to handle keys */ }
	void KeyDown( int key ) { /* implement if you want to handle keys */ }
private:
	Surface* screen;
	int mousex, mousey;
	int mouseButton;
};




class Entity {
public:

	int x, y;


	Entity() {
		x = 0;
		y = 0;
	}

	void RenderThis(Surface* screen,Sprite sprite,int x, int y) {
		sprite.Draw(screen, x - 20, y - 20);
	}
};

class Snow : public Entity{
#define maxP 200000 //amount of snow ground particles
#define SNOWDENSITY 200 // amount of snow particles on screen
#define SNOWFORCE 1 // force multiplier for moving ground snow
public:
	int groundSnowX[maxP], groundSnowY[maxP]; //ground snow positions
	int effectSnowX[SNOWDENSITY], effectSnowY[SNOWDENSITY]; //snowing effect overlay positions

	//GROUND SNOW//
	void CreateGroundSnow() {
		for (int i = 0; i < maxP; i++) 
		{
			groundSnowX[i] = IRand(ScreenWidth);
			groundSnowY[i] = IRand(ScreenHeight);
		}
	}

	void UpdateGroundSnow(int objX,int objY, Surface* screen, int color) {
		for (int i = 0; i < maxP; i++)
		{
			//Checking bounds
			//if a "snow pixel" gets outside the screen will got reset at the botom of the screen with a random x 
			//this allows for infinite snow background generation
			if (groundSnowY[i]<0) {
				groundSnowY[i] = groundSnowY[i] + ScreenHeight;
				groundSnowX[i] = IRand(ScreenWidth);
			}

			//-2 ---> scrolling speed
			groundSnowY[i]=groundSnowY[i]-2;


			//leaving trail behind you by moving the snow that is too close to you

			float dx = groundSnowX[i] - objX, dy = groundSnowY[i] - objY;
			float dist = sqrtf(dx * dx + dy * dy);
			if (dist < 10)
				groundSnowX[i] += dx / dist * SNOWFORCE, groundSnowY[i] += dy / dist * SNOWFORCE;
			
			screen->Plot((int)groundSnowX[i], (int)groundSnowY[i], color);
			
		}
	}

	///////////////


	//SNOW EFFECT OVERLAY//
	void CreateSnowEffect() {
		for (int i = 0; i < SNOWDENSITY; i++) {
			effectSnowX[i] = IRand(ScreenWidth);
			effectSnowY[i] = IRand(ScreenHeight);
		}
	}

	void UpdateSnowEffect(Surface* screen,int color) {
		for (int i = 0; i < SNOWDENSITY; i++) {
			//giving bounds and reseting snowflakes that are off screen
			if (effectSnowY[i] > ScreenHeight) {
				effectSnowY[i] = 0;
			}
			if (effectSnowX[i] > ScreenWidth) {
				effectSnowX[i] = 0;
			}

			//calculating different fall speed for each snowflakes "layer" to achieve a random and natural effect
			effectSnowX[i]++;
			if (i % 10 >= 0 && i % 10 <= 3) {
				effectSnowY[i] = effectSnowY[i] + 1;
			}
			else if (i % 10 >= 5 && i % 10 <= 9) {
				effectSnowY[i] = effectSnowY[i] + 2;
			}
			else
			{
				effectSnowY[i] = effectSnowY[i] + 3;
			}
			

			//Drawing the snowflake in an "X" shape
			screen->Plot((int)effectSnowX[i], (int)effectSnowY[i], color);
			screen->Plot((int)effectSnowX[i] + 1, (int)effectSnowY[i] + 1, color);
			screen->Plot((int)effectSnowX[i] - 1, (int)effectSnowY[i] - 1, color);
			screen->Plot((int)effectSnowX[i] - 1, (int)effectSnowY[i] + 1, color);
			screen->Plot((int)effectSnowX[i] + 1, (int)effectSnowY[i] - 1, color);
		}
	}

	////////////////////////


};


class Player : public Entity {
public:
	Sprite* playerSprite = nullptr;

	Player(Sprite* sprite) {
		playerSprite = sprite;
		y = 120;
	}

	void SkiMovement(Surface* screen,float dist, float speed, int objX,int objY) 
	{

		if (abs(x - objX) > dist || abs(y - objY > dist)) {
			if (x - objX < 0) {
				x += 1 * speed;
			}
			
			if (x - objX> 0) {
				x -= 1 * speed;
			}
		}
		//y += 1 * speed;

		playerSprite->Draw(screen, (int)x - 20, (int)y - 20);
	}
	

};
}; // namespace Tmpl8