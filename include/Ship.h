#pragma once
#include "Const.h"
#include "Sprite.h"

class Ship:public Sprite
{
public:


	Ship(ISprite* sprite,int life, position pos);
	int getLifeLeft();
	static int lifeDrecrease();
	static int getScore();
	bool notFire(float currTime);
	void setLastRocketTime(float lastTime);
	void increaseScore();
	
	~Ship();

private:
	static int score;
	static int lifeLeft;
	float  lastRocketTime;
};


