#pragma once
#include "surface.h"
#include "template.h"
#include <SDL.h>
#include <cstdio> //printf
#include <string>
#include <Windows.h>

static float GAMESPEED =80.0f;

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
	void KeyDown(int key) {
		keyPressed = key;
	}
	void KeyUp(int key) 
	{
		keyPressed = key;
		isPressed = false;	
	}

	bool KeyPressed(int key) {
		if (!isPressed && key==keyPressed) {
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
	float x, y;
	bool oneHitFlag = false;


	Entity() {
		x = 0;
		y = 0;
		oneHitFlag = false;
	}

	Entity(Sprite* sprite) {
		entitySprite = sprite;
		x = 0;
		y = 0;
		oneHitFlag = false;
	}

	void SetSprite(Sprite* sprite) {
		entitySprite = sprite;
	}
	void RenderThis(Surface* screen, int x, int y) {
		entitySprite->Draw(screen, x, y);
	}

	void RenderThis(Surface* screen)
	{
		entitySprite->Draw(screen, (int)x, (int)y);
	}

	float entityFrame = 0.0f;
	void AnimateThis(float start, float finish, float animationSpeed, float deltaTime) 
	{
		entityFrame += animationSpeed * deltaTime / 1000;
		if (entityFrame >= finish)entityFrame = start;
		entitySprite->SetFrame((int)floorf(entityFrame));
	}

	bool check_collision(Entity A, Entity B)
	{
		//The sides of the box
		float leftA, leftB;
		float rightA, rightB;
		float topA, topB;
		float bottomA, bottomB;

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
		float leftA, leftB;
		float rightA, rightB;
		float topA, topB;
		float bottomA, bottomB;

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

	void DebugCollision(Surface* screen)
	{
		//The sides of the rectangles
		float leftA;
		float rightA;
		float topA;
		float bottomA;

		//Calculate the sides of box for this entity
		leftA = x;
		rightA = x + entitySprite->GetWidth();
		topA = y;
		bottomA = y + entitySprite->GetHeight();

		screen->Box((int)leftA,(int)topA,(int)rightA,(int)bottomA,0xff0000);
	}

};

class Snow : public Entity{
#define maxP 200000 //amount of snow ground particles
#define SNOWDENSITY 200 // amount of snow particles on screen
#define SNOWFORCE 45.0f // force multiplier for moving ground snow
public:
	float groundSnowX[maxP] = { 0.0f};
	float groundSnowY[maxP] = { 0.0f}; //ground snow positions
	float effectSnowX[SNOWDENSITY] = {0.0f };
	float effectSnowY[SNOWDENSITY] = {0.0f }; //snowing effect overlay positions
	//GROUND SNOW//
	void CreateGroundSnow() {
		for (int i = 0; i < maxP; i++) 
		{
			groundSnowX[i] = Rand(ScreenWidth);
			groundSnowY[i] = Rand(ScreenHeight);
		}
	}

	void UpdateGroundSnow(int objX,int objY, Surface* screen, int color,float deltaTime) {
		for (int i = 0; i < maxP; i++)
		{
			//Checking bounds
			//if a "snow pixel" gets outside the screen will get reset at the botom of the screen with a random x 
			//this allows for infinite snow background generation
			if (groundSnowY[i]<0) {
				groundSnowY[i] = groundSnowY[i] + ScreenHeight;
				groundSnowX[i] = Rand(ScreenWidth) ;
			}
			

			//-2 ---> scrolling speed
			groundSnowY[i] = groundSnowY[i] - GAMESPEED * deltaTime/1000;

			groundSnowY[i] = floorf(groundSnowY[i]);


			//leaving trail behind you(for each ski) by moving the snow that is too close to you

			float dx = groundSnowX[i] - objX-20, dy = groundSnowY[i] - objY-10;
			float dist = sqrtf(dx * dx + dy * dy);
			if (dist < 10)
				groundSnowX[i] += dx / dist * SNOWFORCE * deltaTime/1000 , groundSnowY[i] += dy / dist * SNOWFORCE *deltaTime / 1000;

			float dx2 = groundSnowX[i] - objX -5, dy2 = groundSnowY[i] - objY - 10;
			float dist2 = sqrtf(dx2 * dx2 + dy2 * dy2);
			if (dist2 < 10)
				groundSnowX[i] += dx2 / dist2 * SNOWFORCE * deltaTime / 1000, groundSnowY[i] += dy2 / dist2 * SNOWFORCE * deltaTime / 1000;
			
			screen->Plot((int)groundSnowX[i], (int)groundSnowY[i], color);


			
		}
	}

	///////////////


	//SNOW EFFECT OVERLAY//
	void CreateSnowEffect() {
		for (int i = 0; i < SNOWDENSITY; i++) {
			effectSnowX[i] = Rand(ScreenWidth);
			effectSnowY[i] = Rand(ScreenHeight);
		}
	}

	void UpdateSnowEffect(Surface* screen, int color, float deltaTime) {
		for (int i = 0; i < SNOWDENSITY; i++) {
			//giving bounds and reseting snowflakes that are off screen
			if (effectSnowY[i] > ScreenHeight) {
				effectSnowY[i] = 0.0f;
			}
			if (effectSnowX[i] > ScreenWidth) {
				effectSnowX[i] = 0.0f;
			}

			//calculating different fall speed for each snowflakes "layer" to achieve a random and natural effect
			effectSnowX[i]+= 100 * deltaTime/1000;
			if (i % 10 >= 0 && i % 10 <= 3) {
				//effectSnowY[i] = effectSnowY[i] + 1 * deltaTime/1000;
				effectSnowY[i] += 100 * deltaTime / 1000;
			}
			else if (i % 10 >= 5 && i % 10 <= 9) {
				//effectSnowY[i] = effectSnowY[i] + 2 *deltaTime / 1000;
				effectSnowY[i] += 200 * deltaTime / 1000;
			}
			else
			{
				//effectSnowY[i] = effectSnowY[i] + 3  *deltaTime / 1000;
				effectSnowY[i] += 300* deltaTime / 1000;
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
	int health = 3;
	Sprite* pickDebugSprite = new Sprite(new Surface("assets/PickDebug.png"), 1);
	Sprite* playerSprite = new Sprite(new Surface("assets/miner_full.png"), 5);
	Sprite* heartSprite = new Sprite(new Surface("assets/heart.png"), 1);
	Entity leftPick;
	Entity rightPick;
	int leftPickDist =-50;
	int rightPickDist = 40;
	bool isMiningLeft = false;
	bool isMiningRight = false;
	bool gameOver = false;

	Player() {
		score = 0;
		health = 3;
		y = 120;
	}

	void SkiMovement(Surface* screen,float dist, float speed, int objX,int objY,int key,float deltaTime) 
	{
		bool isMining = isMiningLeft || isMiningRight;
		leftPick.y = y;
		rightPick.y = y;

		leftPick.x = x + leftPickDist;
		rightPick.x = x + rightPickDist;

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
				x += 1 * speed *deltaTime / 1000;
				if (!isMining)
				playerSprite->SetFrame(2);
			}
			else if (x - objX > 0) {
				x -= 1 * speed*deltaTime / 1000;
				if (!isMining)
				playerSprite->SetFrame(1);
			}
		}
		else {
			if (!isMining)
			playerSprite->SetFrame(0);
		}
		
		//y += 1 * speed;
		playerSprite->Draw(screen, (int)x, (int)y);
	}
	
	void ShowScore(Surface* screen)
	{
		std::string str = std::to_string(score) ;
		char* cstr = &str[0];
		screen->Print(cstr,(int)(x+15),(int)(y-20), 0xebad05);
	}

	void ShowScore(Surface* screen,int posX,int posY, int color)
	{
		std::string str = std::to_string(score);
		char* cstr = &str[0];
		screen->Print(cstr, posX, posY, color);
	}

	void ShowHighScore(Surface* screen, int posX, int posY, int color)
	{
		std::string str = std::to_string(GetHighScore());
		char* cstr = &str[0];
		screen->Print(cstr, posX, posY, color);
	}

	void SaveScore()
	{
		if (score > GetHighScore()) 
		{
			FILE* f = fopen("score.bin", "wb");
			fwrite(&score, sizeof(score), 1, f);
			fclose(f);
		}
	}
	int GetHighScore()
	{
		FILE* f = fopen("score.bin", "rb");
		int highScore=0;

		if (f!=NULL) {
			fread(&highScore, sizeof(highScore), 1, f);
			fclose(f);
		}
		else {
			FILE* g = fopen("score.bin", "wb");
			fwrite(&highScore, sizeof(highScore), 1, g);
			fclose(g);
		}
		return highScore;
	}

	void ShowHealth(Surface* screen) 
	{
		for (int k=20,i = 0; i < health; i++) {
			heartSprite->DrawScaled(k, 20, 40, 40, screen);
			k += 50;
		}
	}

	void TakeDamage() 
	{
		health--;
		if (health <= 0) {
			printf("\nGAME OVER!\n");
			SaveScore();
			gameOver = true;
		}
	}

	void PlayerReset() {
		score = 0;
		GAMESPEED = 80.0f;
		health = 3;
		y = 120;
	}

	int difficulty = 1;
	void AddScore(int value) 
	{
		score += value;

		if ((score > 100 && difficulty==1)||(score>350 && difficulty==2)||(score>650 && difficulty == 3)) {
			GAMESPEED += 40;
			difficulty++;
			printf("score is now: %i and GAMESPEED is now: %f\n", score, GAMESPEED);
		}
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
			ores[i]->x= Rand(ScreenWidth);
			ores[i]->y= ScreenHeight + rand() % 600 + 50.0f;
			ores[i]->oneHitFlag = false;
		}
	}

	//Ore behaviour
	void UpdateOres(Surface* screen,Player &player,bool isMining,int key,float deltaTime) {
		for (int i = 0; i < ORES; i++) {	
			
			//checking if player collided with an ore
			if (player.check_collision(static_cast<Entity>(*ores[i])) && !ores[i]->oneHitFlag)
			{
				//the player just hit an ore hp-- and all that stuff
				//to be added
				ores[i]->oneHitFlag = true;
				printf("PLAYER COLIDED!");
				player.TakeDamage();
				PlaySound("sounds/hurt.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
			

			if( (player.leftPick.check_collision(static_cast<Entity>(*ores[i])) && isMining==1 && key==20))
			{
				ores[i]->isMined = true;
				player.isMiningLeft = true;
				//player.score += ores[i]->oreValue;
				player.AddScore(ores[i]->oreValue);

				PlaySound("sounds/mine.wav", NULL, SND_FILENAME | SND_ASYNC);
			}

			if (player.rightPick.check_collision(static_cast<Entity>(*ores[i])) && isMining == 1 && key == 8)
			{
				ores[i]->isMined = true;
				player.isMiningRight = true;
				//player.score += ores[i]->oreValue;
				player.AddScore(ores[i]->oreValue);

				PlaySound("sounds/mine.wav", NULL, SND_FILENAME | SND_ASYNC);
			}

			

			if (ores[i]->isMined == true) {
				float dist = abs(player.y - ores[i]->y);
				if (dist > 35) {
					ores[i]->RegenerateOre();
					ores[i]->y = ScreenHeight + rand() % 600 + 50.0f;
					ores[i]->x = Rand(ScreenWidth - 50);
					ores[i]->oneHitFlag = false;
					player.isMiningLeft = false;
					player.isMiningRight = false;
					ores[i]->isMined = false;
				}
			}
			

			//if the ore is outside the screen we "regenerate the ore" and gives it another random position under the screen.
			if (ores[i]->y < -50) {
				ores[i]->RegenerateOre();
	
				ores[i]->y = ScreenHeight + rand() % 600 + 50.0f;
				ores[i]->x = Rand(ScreenWidth - 50) ;
			}

			//rendering the ore
			//we move the ore to get the illusion of player movement. 
			//The player is just moving on the x and everything else moves towards or away from the player.
			ores[i]->y = ores[i]->y - GAMESPEED * deltaTime/1000;
			ores[i]->y = floorf(ores[i]->y);
			ores[i]->RenderThis(screen, (int)ores[i]->x, (int)ores[i]->y);
		}
	}

};

class Environment {
#define TREES 2
public:
	Entity* trees[TREES]{
		new Entity(new Sprite(new Surface("assets/tree.png"), 1)),
		new Entity(new Sprite(new Surface("assets/tree.png"), 1)),
	};
	Entity* healthUp = new Entity(new Sprite(new Surface("assets/heart_pickup.png"), 4));
	
	Entity* npc =  new Entity(new Sprite(new Surface("assets/npc.png"),8));

	void InitTreeGeneration()
	{
		for (int i = 0; i < TREES; i++) {
			trees[i]->x = Rand(ScreenWidth);
			trees[i]->y = ScreenHeight + rand() % 600 + 50.0f;
			trees[i]->oneHitFlag = false;
		}
	}

	void InitPickUps()
	{
		healthUp->x = Rand(ScreenWidth);
		healthUp->y = ScreenHeight + rand() % 600 + 500.0f;
		healthUp->oneHitFlag = false;
		
	}

	void InitNpc() {
		npc->x = 0;
		npc->y = ScreenHeight + rand() % 600 + 1400;
		npc->oneHitFlag = false;
	}

	int npcDir = 1.0f;
	void UpdateNpc(Surface* screen, Player& player, float deltaTime) {


		if (player.check_collision(static_cast<Entity>(*npc)) && !npc->oneHitFlag)
		{
			npc->oneHitFlag = true;
			printf("PLAYER COLIDED!");
			player.TakeDamage();
			PlaySound("sounds/hurt.wav", NULL, SND_FILENAME | SND_ASYNC);

		}

		if (npc->y < -50) {
			npc->y = ScreenHeight + rand() % 600 + 1400;
			npc->x = Rand(ScreenWidth);
			npc->oneHitFlag = false;

			if (npc->x < ScreenHeight / 2) npcDir = 1;
			else npcDir = -1;
		}


		if(abs(player.y-npc->y)<500.0f)
			npc->x+= (GAMESPEED-10.0f) * npcDir *deltaTime/1000;

		if(npcDir > 0)
			npc->AnimateThis(0.0f, 4.0f, 6.0f, deltaTime);
		else 
			npc->AnimateThis(4.0f, 8.0f, 6.0f, deltaTime);

		
		npc->RenderThis(screen, (int)npc->x, (int)npc->y);
		npc->y -= GAMESPEED * deltaTime / 1000;
		npc->y = floorf(npc->y);
	}

	float healthFrame = 0.0f;
	void UpdateHealthUp(Surface* screen, Player& player, float deltaTime)
	{
		if (player.check_collision(static_cast<Entity>(*healthUp)) && !healthUp->oneHitFlag)
		{
			healthUp->oneHitFlag = true;
			if (player.health < 3) {
				player.health++;
				PlaySound("sounds/healthup.wav", NULL, SND_FILENAME | SND_ASYNC);
				healthUp->y = ScreenHeight + rand() % 600 + 50 * deltaTime / 1000;
				healthUp->x = Rand(ScreenWidth - 50);
				healthUp->oneHitFlag = false;
			}

		}

		if (healthUp->y < -50) {

			healthUp->y = ScreenHeight + rand() % 600 + 500 * deltaTime / 1000;
			healthUp->x = Rand(ScreenWidth - 50);
			healthUp->oneHitFlag = false;
		}

		//rendering the tree
		//we move the ore to get the illusion of player movement. 
		//The player is just moving on the x and everything else moves towards or away from the player.
		healthFrame += 3.0f * deltaTime / 1000;
		if (healthFrame >= 4)healthFrame = 0.0f;
		healthUp->entitySprite->SetFrame((int)floorf(healthFrame));

		healthUp->RenderThis(screen, (int)healthUp->x, (int)healthUp->y);
		healthUp->y -= GAMESPEED * deltaTime / 1000;
		healthUp->y = floorf(healthUp->y);
		
	}


	void UpdateTrees(Surface* screen, Player& player,float deltaTime) {	
		for (int i = 0; i < TREES; i++) {

			//checking if player collided with a tree

			if (player.check_collision(static_cast<Entity>(*trees[i])) && !trees[i]->oneHitFlag)
			{
				trees[i]->oneHitFlag = true;
				//the player just hit an ore hp-- and all that stuff
				//to be added
				//do stuff
				printf("PLAYER COLIDED!");
				player.TakeDamage();
				PlaySound("sounds/hurt.wav", NULL, SND_FILENAME | SND_ASYNC);
				
			}


			//if the tree is outside the screen we give it another random position under the screen.
			if (trees[i]->y < -50) {

				trees[i]->y = ScreenHeight + rand() % 600 + 50 * deltaTime / 1000;
				trees[i]->x = Rand(ScreenWidth - 50) ;
				trees[i]->oneHitFlag = false;
			}

			//rendering the env objects
			//we move the entity to get the illusion of player movement. 
			//The player is just moving on the x and everything else moves towards or away from the player.
			trees[i]->RenderThis(screen, (int)trees[i]->x, (int)trees[i]->y);
			trees[i]->y -= GAMESPEED * deltaTime/1000;
			trees[i]->y = floorf (trees[i]->y);
		}
	}
};

}; // namespace Tmpl8