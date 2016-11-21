#include "Enemy.h"


int Enemy::noOfInstance = 0;
/*
* @Description: Constructor function of Rocket.
*               Initial sprite, position and add 1 to noOfInstance
*
* @param
*       ISprite: handler to this Rocket;
*		pos: the position of this Rocket.
*
*@return none
*/
Enemy::Enemy(ISprite * sprite, position pos):Sprite(sprite,pos)
{
	noOfInstance++;
	lastBombTime = 0.0f;
}

/*
* @Description: set the nemey to free.
*				draw it to a unvisible area;
*               set Bomb time to 0.0;
*
* @param
*      position: unsivible area position
*		
*
*@return void
*/
void Enemy::goToFree(position freePostion)
{
	Sprite::goToFree( freePostion);
	lastBombTime = 0.0f;
}


/*
* @Description: make judgement whether it is time to drop one bomb of the enmey.
*
* @param
*      currTime: time from the game starting till now.
*
*
*@return 
*		ture if it is time to drop a bomb.else false.
*/
bool Enemy::dropBombOrNot(float currTime)
{
	srand((unsigned)clock());
	int random = rand() % (level.bombRandScope);
	if (random <= 1) {
		return true;
    }
    return false;
}

void Enemy::setLastBombTime(float currTime)
{
	lastBombTime = currTime;
}

Enemy::~Enemy()
{
	noOfInstance--;
}
