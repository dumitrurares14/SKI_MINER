#pragma once
#include "surface.h"
#include "template.h"
#include <SDL.h>
#include <cstdio> //printf





namespace Tmpl8 {
#define GAMESPEED 2
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
	void KeyDown(int key) {
		keyPressed = key;
	}
	void KeyUp(int key) 
	{
		keyPressed = key;
		isPressed = false;
	}

	bool KeyPressed(int key) {
		if (!isPressed) {
			keyPressed = key;
			isPressed = true;
			return true;
		}
		else {
			return false;
		}
	}
	
private:
	Surface* screen;
	int mousex, mousey;
	int mouseButton;
	int keyPressed=0;
	bool isPressed=false;

};


class Entity {
public:
	Sprite* entitySprite = new Sprite(new Surface("assets/cube1.png"), 1);
	int x, y;


	Entity() {
		x = 0;
		y = 0;
	}

	Entity(Sprite* sprite) {
		entitySprite = sprite;
		x = 0;
		y = 0;
	}

	void SetSprite(Sprite* sprite) {
		entitySprite = sprite;
	}
	void RenderThis(Surface* screen,int x, int y) {
		entitySprite->Draw(screen, x, y);
	}

	void RenderThis(Surface* screen)
	{
		entitySprite->Draw(screen,x,y);
	}

	bool check_collision(Entity A, Entity B)
	{
		//The sides of the box
		int leftA, leftB;
		int rightA, rightB;
		int topA, topB;
		int bottomA, bottomB;

		//Calculate the sides of box for Entity 1
		leftA = A.x;
		rightA = A.x + A.entitySprite->GetWidth();
		topA = A.y;
		bottomA = A.y + A.entitySprite->GetHeight();

		//Calculate the sides of box for Entity 2
		leftB = B.x;
		rightB = B.x + B.entitySprite->GetWidth();
		topB = B.y;
		bottomB = B.y + B.entitySprite->GetHeight();

		//If any of the sides from A are outside of B
		if (bottomA <= topB)
		{
			return false;
		}

		if (topA >= bottomB)
		{
			return false;
		}

		if (rightA <= leftB)
		{
			return false;
		}

		if (leftA >= rightB)
		{
			return false;
		}

		//If none of the sides from A are outside B
		return true;
	}

	bool check_collision(Entity B)
	{
		//The sides of the rectangles
		int leftA, leftB;
		int rightA, rightB;
		int topA, topB;
		int bottomA, bottomB;

		//Calculate the sides of box for this entity
		leftA = x;
		rightA = x + entitySprite->GetWidth();
		topA = y;
		bottomA = y + entitySprite->GetHeight();

		//Calculate the sides of box for entity 2
		leftB = B.x;
		rightB = B.x + B.entitySprite->GetWidth();
		topB = B.y;
		bottomB = B.y + B.entitySprite->GetHeight();

		//If any of the sides from A are outside of B
		if (bottomA <= topB)
		{
			return false;
		}

		if (topA >= bottomB)
		{
			return false;
		}

		if (rightA <= leftB)
		{
			return false;
		}

		if (leftA >= rightB)
		{
			return false;
		}

		//If none of the sides from A are outside B
		return true;
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
			//if a "snow pixel" gets outside the screen will get reset at the botom of the screen with a random x 
			//this allows for infinite snow background generation
			if (groundSnowY[i]<0) {
				groundSnowY[i] = groundSnowY[i] + ScreenHeight;
				groundSnowX[i] = IRand(ScreenWidth);
			}

			//-2 ---> scrolling speed
			groundSnowY[i]=groundSnowY[i]-GAMESPEED;


			//leaving trail behind you(for each ski) by moving the snow that is too close to you

			float dx = groundSnowX[i] - objX-20, dy = groundSnowY[i] - objY-10;
			float dist = sqrtf(dx * dx + dy * dy);
			if (dist < 10)
				groundSnowX[i] += dx / dist * SNOWFORCE, groundSnowY[i] += dy / dist * SNOWFORCE;

			float dx2 = groundSnowX[i] - objX -5, dy2 = groundSnowY[i] - objY - 10;
			float dist2 = sqrtf(dx2 * dx2 + dy2 * dy2);
			if (dist2 < 10)
				groundSnowX[i] += dx2 / dist2 * SNOWFORCE, groundSnowY[i] += dy2 / dist2 * SNOWFORCE;
			
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
	int score=0;
	Sprite* pickDebugSprite = new Sprite(new Surface("assets/PickDebug.png"), 1);
	Sprite* playerSprite = new Sprite(new Surface("assets/miner_full.png"), 5);;
	Entity leftPick;
	Entity rightPick;
	int leftPickDist =-50;
	int rightPickDist = 40;
	bool isMiningLeft = false;
	bool isMiningRight = false;

	Player() {
		score = 0;
		y = 120;
		leftPick.SetSprite(pickDebugSprite);
		rightPick.SetSprite(pickDebugSprite);
	}

	void SkiMovement(Surface* screen,float dist, float speed, int objX,int objY,int key) 
	{
		bool isMining = isMiningLeft || isMiningRight;
		leftPick.y = y;
		rightPick.y = y;

		leftPick.x = (x + leftPickDist);
		rightPick.x = (x + rightPickDist);

		//leftPick.RenderThis(screen); //DEBUG MINE 
		//rightPick.RenderThis(screen); //DEBUG MINE

		if (isMiningLeft) {
			playerSprite->SetFrame(3);
		}

		if (isMiningRight) {
			playerSprite->SetFrame(4);
		}

		if (abs(x - objX) > dist || abs(y - objY > dist)) {
			if (x - objX < 0) {
				x += 1 * speed;
				if (!isMining)
				playerSprite->SetFrame(2);
			}
			else if (x - objX > 0) {
				x -= 1 * speed;
				if (!isMining)
				playerSprite->SetFrame(1);
			}
		}
		else {
			if (!isMining)
			playerSprite->SetFrame(0);
		}
		
		//y += 1 * speed;

		playerSprite->Draw(screen, x, y);
	}

	void AddScore(int newScore) 
	{
		score = score + newScore;
	}



};

class Ore : public Entity {
//different values based on the type of ore
#define COALVALUE 10
#define GOLDVALUE 25
#define DMDVALUE 50
#define GEMVALUE 100
public: 
	enum Type { coal, gold, diamond, gem};
	int oreValue = 0;
	bool isMined = false;

	//constructor with specified ore type
	Ore(Type type) {
		switch (type)
		{
		case coal:
			this->SetSprite(new Sprite(new Surface("assets/coal.png"), 1));
			oreValue = COALVALUE;
			break;
		case gold:
			this->SetSprite(new Sprite(new Surface("assets/gold.png"), 1));
			oreValue = GOLDVALUE;
			break;
		case diamond:
			this->SetSprite(new Sprite(new Surface("assets/diamond.png"), 1));
			oreValue = DMDVALUE;
			break;
		case gem:
			this->SetSprite(new Sprite(new Surface("assets/gem.png"), 1));
			oreValue = GEMVALUE;
			break;
		default:
			break;
		}
	}

	//constructor with random ore type
	Ore() {
		RegenerateOre();
	}

	//Generating a random  ore type with certain weight/ rarity
	Type GenerateRandomType() {
		int randomNumber = rand() % 100;
		if (randomNumber < 70)return Type::coal;
		else if (randomNumber > 70 && randomNumber < 90) return Type::gold;
		else if (randomNumber > 90 && randomNumber < 97) return Type::diamond;
		else if (randomNumber>97 && randomNumber <101)return gem;
	}

	//assigning the ore another random type 
	void RegenerateOre() {
		Type type = GenerateRandomType();
		switch (type)
		{
		case coal:
			this->SetSprite(new Sprite(new Surface("assets/coal.png"), 1));
			oreValue = COALVALUE;
			break;
		case gold:
			this->SetSprite(new Sprite(new Surface("assets/gold.png"), 1));
			oreValue = GOLDVALUE;
			break;
		case diamond:
			this->SetSprite(new Sprite(new Surface("assets/diamond.png"), 1));
			oreValue = DMDVALUE;
			break;
		case gem:
			this->SetSprite(new Sprite(new Surface("assets/gem.png"), 1));
			oreValue = GEMVALUE;
			break;
		default:
			break;
		}
	}

	void ScoreThisOre(Player player) {
		player.AddScore(100);
	}
};

class OreGenerator {
#define ORES 3 // amount of max ores
public:

	Ore* ores[ORES]{
		new Ore(),
		new Ore(),
		new Ore(),
	};

	
	//Initialize the first ores (spawning them outside the screen)
	void InitOreGeneration() 
	{
		for (int i = 0; i < ORES; i++) {
			ores[i]->x= IRand(ScreenWidth);
			ores[i]->y= ScreenHeight + rand() % 600 + 50;
		}
	}

	//Ore behaviour
	void UpdateOres(Surface* screen,Player &player,bool isMining,int key) {
		for (int i = 0; i < ORES; i++) {	
			
			//checking if player collided with an ore
			if (player.check_collision(static_cast<Entity>(*ores[i]))) 
			{
				//the player just hit an ore hp-- and all that stuff
				//to be added
				printf("PLAYER COLIDED!");
			}

			if( (player.leftPick.check_collision(static_cast<Entity>(*ores[i])) && isMining==1 && key==20))
			{
				ores[i]->isMined = true;
				player.isMiningLeft = true;
				player.score += ores[i]->oreValue;
			}

			if (player.rightPick.check_collision(static_cast<Entity>(*ores[i])) && isMining == 1 && key == 8)
			{
				ores[i]->isMined = true;
				player.isMiningRight = true;
				player.score += ores[i]->oreValue;
			}
			

			if (ores[i]->isMined == true) {
				int dist = abs(player.y - ores[i]->y);
				if (dist > 35) {
					ores[i]->RegenerateOre();
					ores[i]->y = ScreenHeight + rand() % 600 + 50;
					ores[i]->x = IRand(ScreenWidth - 50);
					player.isMiningLeft = false;
					player.isMiningRight = false;
					ores[i]->isMined = false;
				}
			}
			

			//if the ore is outside the screen we "regenerate the ore" and gives it another random position under the screen.
			if (ores[i]->y < -50) {
				ores[i]->RegenerateOre();
	
				ores[i]->y = ScreenHeight + rand() % 600 + 50;
				ores[i]->x = IRand(ScreenWidth - 50);
			}

			//rendering the ore
			//we move the ore to get the illusion of player movement. 
			//The player is just moving on the x and everything else moves towards or away from the player.
			ores[i]->RenderThis(screen, ores[i]->x, ores[i]->y);
			ores[i]->y = (ores[i]->y) - GAMESPEED;
		}
	}

};

}; // namespace Tmpl8