#pragma once
#include "Const.h"
#include "DiceInvaders.h"
#include <list>
#include "Ship.h"
#include <vector>
#include "Rocket.h"
#include "Bomb.h"
#include "Enemy.h"

using namespace std;

class Game
{
public:
	Game(IDiceInvaders* system);
	void update(IDiceInvaders* system);
	void levelUpgrade();
	void bombUpdate();
	void enemyUpdate();
	void enemyCreate();
	Enemy* Game::newEnemy();
	Bomb*  Game::newBomb();
	Rocket* Game::newRocket();
	Ship* Game::newPlayer();

	void rocketUpdate();
	void shipUpdate(IDiceInvaders::KeyStatus keys);
	void fire();
	bool gameOver();
	void keepPostion();
	bool collisionDetect(position pos1, position pos2);
	~Game();
private:
	int playerLife;
	float timeDelta;
	IDiceInvaders* iDiceInvaders;
	list<Bomb> activeBombs;
	list<Bomb> freeBombs;
	list<Rocket> activeRocket;
	list<Rocket> freeRocket;
	vector< list<Enemy> > activeEnemy;
	list<Enemy> freeEnemy;
	Ship *ship;
};

