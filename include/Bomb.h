#pragma once
#include "DiceInvaders.h"
#include "Enemy.h"
using namespace std;

class Bomb:public Sprite
{
public:
	Bomb(ISprite * sprite, position pos);
	~Bomb();

	//recording how many instances of Bomb have bee created.
	static int noOfInstance;
};

