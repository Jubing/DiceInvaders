#include "Rocket.h"


int Rocket::noOfInstance = 0;
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
Rocket::Rocket(ISprite * sprite, position pos) :Sprite(sprite, pos)
{
	noOfInstance++;
}

Rocket::~Rocket()
{
	noOfInstance--;
}
