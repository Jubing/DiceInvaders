#include "game.h"
#include "Const.h"



/*
* @Description: Structor function of Game
*			
* @param
*      handler of IDiceInvaders
*
*
*@return
*		
*/
Game::Game(IDiceInvaders* system)
{
	playerLife = 3;
	iDiceInvaders = system;
	iDiceInvaders->init(width, height);
	ISprite* sprite = system->createSprite(playerBMP);

	ship = new Ship(sprite, playerLife, position(width / 2, (height - pixelV)));
	activeEnemy.resize(lines+1);
	timeDelta = 0.0f;
}


/*
* @Description: update all the sprites movement and also including create/update sprites, 
*				drop bombs and play fire also happened in this function.
*
* @param
*      handler of IDiceInvaders
*
*
*@return void
*
*/
void Game::update(IDiceInvaders* system)
{
	float newTime, lastTime, elapsedTime;
	lastTime = iDiceInvaders->getElapsedTime();

	IDiceInvaders::KeyStatus keys;
	char scoreInfo[50], lifeInfo[50];


	while (system->update()) {
		newTime = system->getElapsedTime();
		elapsedTime = newTime - lastTime;

		lastTime = newTime;
		timeDelta = timeDelta + elapsedTime;

		sprintf_s(scoreInfo, "Your Score: %d", ship->getScore());
		sprintf_s(lifeInfo, "Life      Left: %d", ship->getLifeLeft());
		system->drawText(500, 0, scoreInfo);
		system->drawText(500, 15, lifeInfo);

		//upgrade to next level if requirement met.
		levelUpgrade();

		system->getKeyStatus(keys);

		if (timeDelta >= updateInterval) { //In order to keep they moving smoothly. Only move these sprites while the interval > updateInterval.

			timeDelta = 0.0f;
			bombUpdate();
			enemyUpdate();
			rocketUpdate();
			shipUpdate(keys);
			if (gameOver()) {
				return;
			}

		}
		else {
			//In other system update, it just refresh sprites' position as now.
			keepPostion();
		}


	}
}


/*
* @Description: upgrade to harder level if needed
*
* @param
*      handler of IDiceInvaders
*
*
*@return
*
*/
void Game::levelUpgrade()
{
	int score = ship->getScore();
	if ((level.equal(EASY)) && (score >= levelUp.upgradeTo2ndLevel)) {
		level = MED;
	}
	else if (level.equal(MED) && (score >= levelUp.upgradeTo3rdLevel)) {
		level = HARD;
	}
}


/*
* @Description: refresh sprites current position.
*
* @param
*
*
* @return void
*
*/
void Game::keepPostion() {
	//refresh ship 
	ship->update();

	//refresh all active Enemies
	list<Enemy>::iterator enemyIter;
	for (int i = lines - 1; i >= 0; i--) {
		for (enemyIter = activeEnemy[i].begin(); enemyIter != activeEnemy[i].end(); enemyIter++) {
			enemyIter->update();
		}
	}


	//refresh all active Bombs
	list<Bomb>::iterator bombIterator, tempIter;
	for (bombIterator = activeBombs.begin(); bombIterator != activeBombs.end(); bombIterator++) {
		bombIterator->update();
	}

	//refress all active Rockets
	list<Rocket>::iterator rocketIter;
	for (rocketIter = activeRocket.begin(); rocketIter != activeRocket.end(); rocketIter++) {
		rocketIter->update();
	}

}


/*
* @Description: update all bombs movement. If it goes out of frame or collides ship, 
*				then move it from active list to free list.
*
* @param
*
*
* @return void
*
*/
void Game::bombUpdate()
{
	list<Bomb>::iterator bombIter, tempIter;
	for (bombIter = activeBombs.begin(); bombIter != activeBombs.end(); ) { //traversing all Bombs
		
		//tempIter point to next element
		tempIter = bombIter;
		tempIter++; 

		//update bomb position
		position pos = bombIter->getPos();

		//next position according the moveSpeed
		pos.y = pos.y + level.bombMoveSpeed;
		bombIter->update(pos);

		if (pos.y > height) {   //bomb has droped out of frame. move it to free list
			bombIter->goToFree(freePostion);
			freeBombs.splice(freeBombs.begin(), activeBombs, bombIter);
		}
		else 	if (bombIter->getPos().y > (height - pixelV)) {
			//if the bomb has reach the bottom, then check whether bomb collise ship.

			if (collisionDetect(bombIter->getPos(), ship->getPos())) {

				//decrease ship's life;
				ship->lifeDrecrease();

				bombIter->goToFree(freePostion);

				//move the bomb from active list to freelist
				freeBombs.splice(freeBombs.begin(), activeBombs, bombIter);
			}
		}
		//move to next Element
		bombIter = tempIter;
	}
}

/*
* @Description: udpate all enemies movement. also drop bombs 
*
* @param
*
*
* @return void
*
*/
void Game::enemyUpdate()
{

	unsigned seed = (unsigned)(timeDelta * 1000000);
	srand(seed);
	list<Enemy>::iterator enemyIter;

	//From the low to high line
	for (int i = lines - 1; i >= 0; i--) {
		//go through elements in the line
		for (enemyIter = activeEnemy[i].begin(); enemyIter != activeEnemy[i].end(); enemyIter++) {
			position pos = enemyIter->getPos();

			//Drop a bomb or not.
			if (enemyIter->dropBombOrNot(iDiceInvaders->getElapsedTime())) {

				//no bombs availabe, new bomb created.
				if ((freeBombs.size() == 0) && (Bomb::noOfInstance < level.bombMax)) { 

					ISprite* iBomp = iDiceInvaders->createSprite(bombBMP);
					Bomb * bomp = new Bomb(iBomp, pos);
					activeBombs.push_front(*bomp);
					bomp->update(pos);
				}
				else if (freeBombs.size() > 0) {
					//there are free bombs. move it to active Bomb
					list<Bomb>::iterator iter = freeBombs.begin();
					iter->goToActive(pos);
					activeBombs.splice(activeBombs.begin(), freeBombs, iter);
				}
				enemyIter->setLastBombTime(iDiceInvaders->getElapsedTime());
			}

			//Move the enemies at the edge to next line.
			if (((i % 2 == 0) && (pos.x == (width - pixelH))) || ((i % 2 == 1) && (pos.x == 0))) {  //Change lines;
				pos.y = pos.y + (height - pixelV) / lines;
				enemyIter->update(pos);
				activeEnemy[i + 1].splice(activeEnemy[i + 1].begin(), activeEnemy[i], enemyIter);
				break;
			}


			//move the enemys.
			int move = level.enemyMoveSpeed;
			if (i % 2 == 0) { 
				//In even lines, direction is from left o right
				pos.x = ((pos.x + move) >= (width - pixelH)) ? (width - pixelH) : (pos.x + move);
			}
			else {
				//In odd lines, direction is from right to left
				pos.x = ((pos.x - move) >= 0) ? (pos.x - move) : 0;
			}
			enemyIter->update(pos);
		}
	}

	//Create new enemy if it is time to create a new enemy
	enemyCreate();
}


/*
* @Description: if needed, create a new Enemy
*
* @param
*
*
* @return void
*
*/
void Game::enemyCreate()
{
	//should a new enemy runs in the game?
	srand((unsigned)time(0));
	int x = rand() % (width / level.enemyInOneLine) + 2 * pixelH;
	position pos(x, 0);
	list<Enemy>::iterator iter = activeEnemy[0].begin();

	//get the first enemy position in line 0
	if (iter != activeEnemy[0].end())
		pos = iter->getPos();

	//If the enemy is far enough from the begin point, then generate a new enemy
	if ((pos.x >= (x))) {
		//If there is no free Enemy in the free list.
		if ((freeEnemy.size() == 0)) {

			//If number of Enemies have not reached the max. then create new enemy
			if (Enemy::noOfInstance < level.enemyMax) {

				Enemy* newenemy = newEnemy();

				//add to the begin of activeEnemy first line.
				activeEnemy[0].push_front(*newenemy);
			}
		}
		else { //there are some enemies in free list. so move one from free to active list.
			pos.x = 0;
			pos.y = 0;
			list<Enemy>::iterator enemyIter = freeEnemy.begin();
			enemyIter->goToActive(pos);
			activeEnemy[0].splice(activeEnemy[0].begin(), freeEnemy, enemyIter);//add to active list

		}
	}
}


/*
* @Description: create a new enemy, randomly between green and red.
*
* @param
*
*
* @return void
*
*/
Enemy* Game::newEnemy() {
	srand((unsigned)clock());
	int random = rand() % 2;
	ISprite *enemySprite;
	position pos(0, 0);
	if (random == 0)
		enemySprite = iDiceInvaders->createSprite(greenEnemyBMP);
	else
		enemySprite = iDiceInvaders->createSprite(redEnemyBMP);

	return new Enemy(enemySprite, pos);

}

/*
* @Description: create a new bomb
*
* @param
*
*
* @return void
*
*/
Bomb * Game::newBomb()
{
	ISprite *sprite;
	position pos(0, 0);
	sprite = iDiceInvaders->createSprite(bombBMP);

	return new Bomb(sprite, pos);
}

/*
* @Description: create a new rocket
*
* @param
*
*
* @return void
*
*/
Rocket * Game::newRocket()
{
	ISprite *sprite;
	position pos(0, 0);
	sprite = iDiceInvaders->createSprite(rocketBMP);

	return new Rocket(sprite, pos);
}

/*
* @Description: create a new player
*
* @param
*
*
* @return void
*
*/
Ship * Game::newPlayer()
{
	ISprite *sprite;
	position pos(0, 0);
	sprite = iDiceInvaders->createSprite(playerBMP);

	return nullptr;//new Ship(sprite, pos);
}


/*
* @Description: update rockets and check collision. if goes out of frame or collided, move it to free list.
*
* @param
*
*
* @return void
*
*/
void Game::rocketUpdate()
{
	list<Enemy>::iterator iter, tempIter;
	list<Rocket>::iterator rocketIter, tempRocketIter;
	bool collision = false;

	//Go through rocket active list
	for (rocketIter = activeRocket.begin(); rocketIter != activeRocket.end(); ) {

		//tempRockIter point to next element of rocketIter
		tempRocketIter = rocketIter;
		tempRocketIter++;

		//move the rocket according to move speed.
		position pos = rocketIter->getPos();
		pos.y = pos.y - level.rocketMoveSpeed;
		rocketIter->update(pos);

		//if the rocket still in the frame.
		if ((pos.y) >= 0) {

			//Then we could find which line the rocket head is in.
			int line = pos.y / (height / lines);
			int belowLine = line + 1;

			//then go through all the line and the (line+1), check collision.
			while ((line < lines) && (line <= belowLine) && (!collision)) {
				for (iter = activeEnemy[line].begin(); (iter != activeEnemy[line].end());) {
					tempIter = iter;
					tempIter++;
					if (collisionDetect(rocketIter->getPos(), iter->getPos())) {
						//move this Rocket from active to free list
						rocketIter->goToFree(freePostion);
						freeRocket.splice(freeRocket.end(), activeRocket, rocketIter);


						//move this Enemy from active to free list
						iter->goToFree(freePostion);
						freeEnemy.splice(freeEnemy.end(), activeEnemy[line], iter);
						collision = true;
						ship->increaseScore();
						break;
					}
					iter = tempIter;
				}
				//check elements in line+1
				line++;
			}
		}
		else { //it is out of frame. add it to free list.
			rocketIter->goToFree(freePostion);
			freeRocket.splice(freeRocket.end(), activeRocket, rocketIter);
		}

		//move to next rocket
		rocketIter = tempRocketIter;
	}

}

/*
* @Description: update ship position according to key direction. send out a rocket if fire key enabled.
*
* @param : key status of system
*
*
* @return void
*
*/

void Game::shipUpdate(IDiceInvaders::KeyStatus keys)
{

	int x = ship->getPos().x;

	if (keys.right) {	//If it is right, then the maximum is (width - pixelH) 
		x = (x < width - pixelH - level.playerMoveSpeed ? x + level.playerMoveSpeed : width - pixelH);
	}
	else if (keys.left) { //If it is left key, the minimum is 0;
		x = (x - level.playerMoveSpeed > 0 ? x - level.playerMoveSpeed : 0);
	}

	if (keys.fire) { //If it is fire key
		fire();
	}
	position pos(x, height - pixelV);
	ship->update(pos);
}

/*
* @Description: send out a rocket at player position
*
* @param
*
*
* @return void
*
*/
void Game::fire()
{
	position pos = ship->getPos();


	//check whether it could meet the fire interval requirement.
	if (ship->notFire(iDiceInvaders->getElapsedTime())) {
		return;
	}

	//if no rocket in the free list
	if (freeRocket.size() == 0) {
		if (Rocket::noOfInstance < level.rocketMax) //and it hasn't reach the maximum
		{
			//create a new Rocket and add it to rocket active list
 			ISprite * sprite = iDiceInvaders->createSprite(rocketBMP);
			Rocket* rocket = new Rocket(sprite, pos);
			activeRocket.push_front(*rocket);
			rocket->update(pos);

		}
	}
	else {//otherwise, move one rocket from free list to active list
		list<Rocket>::iterator iter = freeRocket.begin();
		activeRocket.splice(activeRocket.begin(), freeRocket, iter);
		activeRocket.begin()->goToActive(pos);
	}

	//record the fire time;
	ship->setLastRocketTime(iDiceInvaders->getElapsedTime());
}

/*
* @Description: check the game is over or not
*
* @param
*
*
* @return void
*
*/
bool Game::gameOver()
{
	//if the life reachs 0 or one enemy reach the bottom, then game over;
	if (activeEnemy[lines].size() != 0 || ship->getLifeLeft() <= 0)
		return true;
	return false;
}


/*
* @Description: check whether the two positions collide
*
* @param: two positions to be checked.
*
*
* @return void
*
*/
bool Game::collisionDetect(position pos1, position pos2)
{
	//If the distance between pos1 and pos2 are less then collitionMargin, then they are collided.
	return (abs(pos1.x - pos2.x) < collisionMargin) && (abs(pos1.y - pos2.y) < collisionMargin);
}


Game::~Game()
{
	delete ship;
	activeBombs.clear();
	freeBombs.clear();
	activeRocket.clear();
	freeRocket.clear();
	freeEnemy.clear();
	for (int i = 0; i <= lines; i++) {
		activeEnemy[i].clear();
	}
	activeEnemy.clear();
}
