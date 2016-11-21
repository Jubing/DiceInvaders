#pragma once
#include "DiceInvaders.h"
#include "stddef.h"
#include <list>
#include "time.h"
using namespace std;

struct position 
{
public:
	int x;
	int y;
	position() {
		x = 0;
		y = 0;
	}
	position(int a, int b) {
		x = a; y = b;
	}
};

static const float updateInterval = 0.02f;
struct gameLevel {
	int enemyInOneLine;
	int enemyMax;
	int rocketMax;
	int bombMax;
	int bombsPerEnemyPer10s;
	int playerMoveSpeed;
	int enemyMoveSpeed;
	int rocketMoveSpeed;
	int bombMoveSpeed;
	int bombRandScope;//the rand function will be called every 10ms, it should be bombsPerEnemyPer10s/10/updateInterval
	gameLevel(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8,int a9) {
		enemyInOneLine = a1, enemyMax = a2, rocketMax = a3, bombMax = a4, bombsPerEnemyPer10s = a5, playerMoveSpeed = a6, enemyMoveSpeed = a7, rocketMoveSpeed = a8; bombMoveSpeed = a9; bombRandScope = (int)(a5 / updateInterval);
	}
	bool equal(gameLevel gameLev) {
		return((enemyInOneLine == gameLev.enemyInOneLine)\
			&& (enemyMax == gameLev.enemyMax) \
			&& (rocketMax == gameLev.rocketMax) \
			&& (bombMax == gameLev.bombMax) \
			&& (bombsPerEnemyPer10s == gameLev.bombsPerEnemyPer10s)\
			&& (playerMoveSpeed == gameLev.playerMoveSpeed) \
			&& (enemyMoveSpeed == gameLev.enemyMoveSpeed)\
			&& (bombMoveSpeed == gameLev.bombMoveSpeed));
	}
};

static const gameLevel EASY(5, 40, 5, 20, 2, 2, 1, 1, 1);
static const gameLevel MED(8, 60, 9, 30, 4, 4, 2, 2, 2);
static const gameLevel HARD(12, 100, 14, 50, 6, 6, 4, 4, 4);
static  gameLevel level = EASY;

struct levelUpgrade{
	int upgradeTo2ndLevel;
	int upgradeTo3rdLevel;
	levelUpgrade(int x, int y) { upgradeTo2ndLevel = x, upgradeTo3rdLevel = y; }
};
static const levelUpgrade levelUp(100, 1000);   //level upgrade.


static const int width = 640;    //width of frame
static const int height = 480;   //height of frame
static const int pixelH = 32;    //width of sprite
static const int pixelV = 32;    //height of sprite
static const int lines = 8;      //lines of enemies
static const float rocketInterval = 0.3f;   //rocketInterval before next fire
static const int  collisionMargin = 20;     //collision Margin, if distance is shorter than this value, then collided.

static const position freePostion(width, height);   //the position out of frame, in an unvisible area

#define playerBMP "data/player.bmp"
#define greenEnemyBMP "data/enemy2.bmp"
#define redEnemyBMP   "data/enemy1.bmp"
#define bombBMP       "data/bomb.bmp"
#define rocketBMP     "data/rocket.bmp"


