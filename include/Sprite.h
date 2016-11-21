#pragma once
#include "Const.h"
#include "DiceInvaders.h"
class Sprite
{
public:
	Sprite(ISprite* sprite, position pos);
	void update(position pos);
	void update();
	position getPos();
	void setPos(position pos);
	void goToFree(position freePos);
	void goToActive(position pos);
	~Sprite();

private:
	void draw();
	Sprite();

	ISprite *sprite;
	position pos;
};

