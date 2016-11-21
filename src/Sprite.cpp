#include "Sprite.h"



Sprite::Sprite()
{
}


Sprite::Sprite(ISprite* sprite, position pos)
{
	this->sprite = sprite;
	this->pos = pos;
	this->update();
}

void Sprite::update(position pos)
{
	this->pos = pos;
	this->update();
}

void Sprite::update()
{
	this->draw();
}

position Sprite::getPos()
{
	return pos;
}

void Sprite::setPos(position pos)
{
	this->pos = pos;
}

void Sprite::goToFree(position freePos)
{
	this->update(freePos);
}

void Sprite::goToActive(position pos)
{
	update(pos);
}


Sprite::~Sprite()
{
	sprite->destroy();
}

void Sprite::draw()
{
	sprite->draw(pos.x, pos.y);
}
