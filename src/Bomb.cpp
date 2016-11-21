#include "Bomb.h"


int Bomb::noOfInstance = 0;


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
Bomb::Bomb(ISprite * sprite, position pos) :Sprite(sprite, pos)
{
	noOfInstance++;
}

Bomb::~Bomb()
{
	noOfInstance--;
}
