#pragma once
#include "DiceInvaders.h"
#include "Const.h"
#include "Sprite.h"

class Enemy:public Sprite
{
public:
	Enemy(ISprite* enemy, position pos);
	void goToFree(position freePostion);
	bool dropBombOrNot(float currTime);

	void setLastBombTime(float currTime);


	//recording how many instances of Enemy have bee created.
	static int noOfInstance;

	~Enemy();
private:
	struct position pos;
	float lastBombTime;
};

