#pragma once
#include "DiceInvaders.h"
#include "Const.h"
#include "Sprite.h"
class Rocket:public Sprite
{
public:
	Rocket(ISprite * sprite, position pos);
	~Rocket();

	//recording how many instances of Rocket have bee created.
	static int noOfInstance;
};

