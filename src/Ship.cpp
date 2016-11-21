#include "Ship.h"


int Ship::score = 0;
int Ship::lifeLeft = 3;


Ship::Ship(ISprite * sprite,int life, position pos):Sprite(sprite,pos)
{
	lifeLeft = life;
}

int Ship::getLifeLeft()
{
	return lifeLeft;
}


int Ship::lifeDrecrease()
{
	lifeLeft--;
	return lifeLeft;
}

int Ship::getScore()
{
	return score;
}



bool Ship::notFire(float currTime)
{
	if ((lastRocketTime != 0.0f)&&(currTime - lastRocketTime) <= rocketInterval) {
		return true;
	}
	return false;
}

void Ship::setLastRocketTime(float lastTime)
{
	lastRocketTime = lastTime;
}

void Ship::increaseScore()
{
	
	score = score +10;

}

Ship::~Ship()
{
}