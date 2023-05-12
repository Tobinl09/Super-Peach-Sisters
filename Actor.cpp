#include "Actor.h"
#include "StudentWorld.h"
#include "GameWorld.h"
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR/////////////////////////////////////////////////////////////////////////////

Actor::Actor(int imageID, int startX, int startY, StudentWorld* ptr, bool isDamageable, bool isBlock, bool isEnemy, bool isGoal, bool isGoodie, bool isBall, int dir, int depth, double size)
	:GraphObject(imageID, startX, startY, dir, depth, size), isAlive(true), SWPtr(ptr), isBlock(isBlock), isDamageable(isDamageable), isEnemy(isEnemy), isGoal(isGoal), isGoodie(isGoodie), isBall(isBall)
{}
Actor::~Actor()
{}

void Actor::bonk()
{
	return;
}

bool Actor::getIsAlive()
{
	return isAlive;
}
void Actor::setIsAlive(bool TrueOrFalse)
{
	isAlive = TrueOrFalse;
}
bool Actor::getIsDamageable()
{
	return isDamageable;
}
StudentWorld* Actor::getWorld()
{
	return SWPtr;
}

bool Actor::getIsBlock()
{
	return isBlock;
}
bool Actor::getIsEnemy()
{
	return isEnemy;
}
bool Actor::getIsGoal()
{
	return isGoal;
}
bool Actor::getIsGoodie()
{
	return isGoodie;
}
bool Actor::getIsBall()
{
	return isBall;
}


// damageable|block|enemy|flag|isGoodie
//PEACH////////////////////////////////////////////////////////////////////////

Peach::Peach(int imageID, int x, int y, StudentWorld* ptr)
	:Actor(imageID, x, y, ptr, true, false, false, false, false, false, 0, 0, 1.0), tempInvinc(0), starPower(0),
	shootPower(0), jumpPower(0), hitPoints(1), remaining_jump_distance(0), time_to_recharge_before_next_fire(0)
{}
Peach::~Peach()
{}
void Peach::bonk()
{
	if (tempInvinc > 0 || getStarPower())
	{
		return;
	}
	if (getHitPoints() == 2)
	{
		setHitPoints(1);
		setTempInvinc(10);
		setShootPower(false);
		setJumpPower(false);
		getWorld()->playSound(SOUND_PLAYER_HURT);
	}
	else
	{
		setIsAlive(false);
	}	return;
}


void Peach::doSomething()
{
	if (!getIsAlive()) //(1)
	{
		return;
	}
	if (getStarPower()) //(2)
	{
		starPower--;
	}
	if (getTempInvinc()) //(3)
	{
		tempInvinc--;
	}
	if (getRecharge() > 0) //(4)
	{
		time_to_recharge_before_next_fire--;
	}

	getWorld()->isOverlapping(0, 0, false);//(5)

	if (getJumpDist() != 0) //jump mechanism
	{//check to see if peach can move in the "in between: areas of the block
		if (canPeachMove(4, 8, true) && canPeachMove(0, 8, true) && canPeachMove(-4, 8, true))
		{
			moveTo(getX(), getY() + 4);
			remaining_jump_distance--;
		}
		else remaining_jump_distance = 0;
	}
	if (getJumpDist() == 0)
	{
		if (canPeachMove(-4, -8, true) && canPeachMove(0, -8, true) && canPeachMove(4, -8, true))
		{
			moveTo(getX(), getY() - 4);
		}
	}

	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			setDirection(180);
			if (canPeachMove(-8, 0, false) && canPeachMove(-8, 4, false) && canPeachMove(-8, -4, false))
				moveTo(getX() - 4, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(0);
			if (canPeachMove(8, 0, false) && canPeachMove(8, 4, false) && canPeachMove(8, -4, false))
				moveTo(getX() + 4, getY());
			break;
		case KEY_PRESS_UP:
			if (!(canPeachMove(-4, -8, true) && canPeachMove(0, -8, true) && canPeachMove(4, -8, true))) //prevetns jumping in mid air
			{
				
				if (getJumpPower())
				{
					remaining_jump_distance = 12;
				}
				else
				{
					remaining_jump_distance = 8;
				}
				getWorld()->playSound(SOUND_PLAYER_JUMP);
			}

			break;
		case KEY_PRESS_SPACE:
			if (shootPower && getRecharge() == 0)
			{
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				time_to_recharge_before_next_fire = 8;

				if (getDirection() == 0)
				{
					if (getWorld()->isThereWall(getX() + 4, getY(), 2))//wall to the right
						return;
					if (!getWorld()->isThereFloor(getX()+4, getY(), 2) && getWorld()->isThereFloor(getX() + 8, getY(), 2))//wall to the right and 2 blocks down. basically checking if there i a space between two blocks
						return;
				}
				if (getDirection() == 180)
				{
					if (getWorld()->isThereWall(getX() - 4, getY(), 2))//wall to the left
						return;
					if (!getWorld()->isThereFloor(getX()-4, getY(), 2) && getWorld()->isThereFloor(getX() - 8, getY(), 2)) //spec didnt speciify what to do when peach is half off a block so i followed the sample
						return;
				}
				getWorld()->makeFireball(1, getX(), getY(), getDirection());
				
			}
			break;

			break;
			return;
		}
		return;
	}
	else return;


}

bool Peach::canPeachMove(double eightX, double eightY, bool fromFall) //tests to see if peach will overlap at peach x+egithx, y+eightY
{
	if (getWorld()->isOverlapping(eightX, eightY, fromFall))
	{
		return false;
	}
	else return true;
}

bool Peach::getStarPower()
{
	if (starPower > 0)
		return true;
	else return false;
}
void Peach::setStarPower(int a)
{
	starPower = a;
}
void Peach::setShootPower(bool a)
{
	shootPower = a;
}
bool Peach::getShootPower()
{
	if (shootPower > 0)
		return true;
	else return false;
}

bool Peach::getJumpPower()
{
	if (jumpPower > 0)
		return true;
	else return false;
}
void Peach::setJumpPower(bool a)
{
	jumpPower = a;
}
bool Peach::getTempInvinc()
{
	if (tempInvinc > 0)
		return true;
	else return false;
}
void Peach::setTempInvinc(int a)
{
	tempInvinc = a;
}
int Peach::getHitPoints()
{
	return hitPoints;
}
void Peach::setHitPoints(int a)
{
	hitPoints = a;
}


int Peach::getJumpDist()
{
	return remaining_jump_distance;
}

int Peach::getRecharge()
{
	return time_to_recharge_before_next_fire;
}
void Peach::setRecharge()
{
	time_to_recharge_before_next_fire = 8;
}


// damageable|block|enemy|goal
//STATIONARY/////////////////base class for pipe and blocks////////////
Stationary::Stationary(int imageID, bool isBlock, bool isGoodie, int startX, int startY, StudentWorld* ptr, int dir, int depth, double size)
	:Actor(imageID, startX, startY, ptr, false, isBlock, false, false, false, isGoodie, dir = 0, depth, size)
{

}

Stationary::~Stationary()
{}
void Stationary::doSomething()
{
	return;
}


void Stationary::bonk()
{
	getWorld()->playSound(SOUND_PLAYER_BONK);
	bonkEdit();
}

//PIPE/////////////////////////////////////////////////
Pipe::Pipe(int imageID, int x, int y, StudentWorld* ptr, bool isBlock)
	:Stationary(imageID, isBlock, false, x, y, ptr, 0, 2, 1)
{}
Pipe::~Pipe()
{}
void Pipe::bonkEdit()
{
	return;
}

//BLOCK//////////////////////////////////////////
Block::Block(int imageID, int x, int y, bool isBlock, bool hasGoodie, bool flower, bool shroom, bool star, bool isGoodie, StudentWorld* ptr, int depth)
	: Stationary(imageID, isBlock, isGoodie, x, y, ptr, 0, depth, 1), hasGoodie(hasGoodie), flower(flower), shroom(shroom), star(star)
{}

Block::~Block()
{}

bool Block::getFlower()
{
	return flower;
}
bool Block::getShroom()
{
	return shroom;
}
bool Block::getStar()
{
	return star;
}
bool Block::getHasGoodie()
{
	return hasGoodie;
}
void Block::setHasGoodie()
{
	hasGoodie = false;
}

void Block::bonkEdit()
{
	if (getFlower() && getHasGoodie())//when flower
	{
		getWorld()->makeGoodie(0, getX(), getY() + 8);
		setHasGoodie();
	}
	if (getShroom() && getHasGoodie())//when mushroom
	{
		getWorld()->makeGoodie(1, getX(), getY() + 8);
		setHasGoodie();
	}
	if (getStar() && getHasGoodie())//when star
	{
		getWorld()->makeGoodie(2, getX(), getY() + 8);
		setHasGoodie();
	}
}
// damageable|block|enemy|goal
//GOAL////// BASE FOR MARIO AND FLAG
Goal::Goal(int imageID, int x, int y, bool isFlag, StudentWorld* ptr)
	:Actor(imageID, x, y, ptr, false, false, false, true, false, false, 0, 1, 1), isFlag(isFlag)
{}
Goal::~Goal()
{}

void Goal::doSomething()
{
	if (!getIsAlive())
	{
		return;
	}
	if (goalReached())
	{
		getWorld()->increaseScore(1000);
		if (isFlag)
		{
			getWorld()->playSound(SOUND_FINISHED_LEVEL);
			getWorld()->setLevelEnd(true);
		}
		else getWorld()->setGameEnd(true);
	}
}
bool Goal::goalReached()
{
	if (getWorld()->isOverlappingCoord(getX(), getY(), 0))
	{
		return true;
	}
	else return false;
}


//GOODIE////////////////
Goodie::Goodie(int imageID, double x, double y, bool isBlock, bool hasGoodie, bool flower, bool shroom, bool star, StudentWorld* ptr, int depth)
	:Block(imageID, x, y, isBlock, hasGoodie, flower, shroom, star, true, ptr, depth)
{}
Goodie::~Goodie()
{}

void Goodie::move()
{
	if (getWorld()->isThereFloor(getX(), getY(), 2))//there is floor
	{
		if (getWorld()->isThereWall(getX(), getY(), 2))//there is wall
		{
			if (getDirection() == 0 && getWorld()->isThereWall(getX()+2, getY(), 2))//there is wall to the right
				setDirection(180);
			if (getDirection() == 180 && getWorld()->isThereWall(getX() - 2, getY(), 2))//there is wall to the left
			 setDirection(0);
		}

		if (getDirection() == 0)
		{
			moveTo(getX() + 2, getY());
		}
		else moveTo(getX() - 2, getY());
		
	}
	else
	{
		moveTo(getX(), getY() - 2);
		
		if (getDirection() == 0 && getWorld()->isThereWall(getX() + 2, getY(), 2))//spec didnt relaly specifiy how goodies move when falling so i copied the smaple
		{
			setDirection(180);
			return;
		}
		
		if (getDirection() == 180 && getWorld()->isThereWall(getX() - 2, getY(), 2))
		{
			setDirection(0);
			return;
		}

		if (getDirection() == 0)
		{
			moveTo(getX() + 2, getY());
		}
		else moveTo(getX() - 2, getY());

	}
	
}

void Goodie::doSomething()
{
	move();

	if (getWorld()->isOverlappingCoord(getX(), getY(), 2))
	{
		getWorld()->increaseScore(50);
		if (getFlower())
		{
			getWorld()->peachPtr()->setShootPower(true);
			getWorld()->peachPtr()->setHitPoints(2);

		} 
		if (getShroom())
		{
			getWorld()->peachPtr()->setJumpPower(true);
			getWorld()->peachPtr()->setHitPoints(2);

		}
		if (getStar())
		{
			getWorld()->peachPtr()->setStarPower(150);
		}
		setIsAlive(false);
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
		
		return;		

	}
}

Projectile::Projectile(int imageID, int startX, int startY, StudentWorld* ptr, bool isDamageable, bool isBlock, bool isEnemy, bool isGoal, bool isGoodie, bool isBall, int dir, int depth, double size)
	: Actor(imageID, startX, startY, ptr, isDamageable, isBlock, isEnemy, isGoal, isGoodie, isBall, dir, depth, size)
{}

Projectile::~Projectile()
{}

void Projectile::moveMech()
{
	int a = moveMechEdit();
	if (a == 1)
	{
		a = 0;
	}
	if (a == 2)
	{
		return;
	}
	if (getWorld()->isThereFloor(getX(), getY(), 2))//there is floor
	{
		if (getWorld()->isThereWall(getX() + a, getY(), 2) && getWorld()->isThereWall(getX(), getY(), 2))//the int a is there so the piranha doesnt fire it into itself
		{
			setIsAlive(false);
			return;
		}
		if (getDirection() == 0)
		{
			moveTo(getX() + 2, getY());
		}
		else moveTo(getX() - 2, getY());

	}
	if (!getWorld()->isThereFloor(getX(), getY(), 2))
	{
		if (getDirection() == 180)
		{
			if (getWorld()->isThereWall(getX() - 2, getY() - 2, 2) || getWorld()->isThereWall(getX() - 2, getY() - 4, 2) && getWorld()->isThereFloor(getX() - 4, getY(), 2))
			{
				setIsAlive(false);
			}
			if (getWorld()->isThereWall(getX() - 2, getY() - 4, 2) && getWorld()->isThereFloor(getX() - 4, getY(), 2))
			{
				return;
			}
		}
		if (getDirection() == 0)
		{
			if (getWorld()->isThereWall(getX() + 2, getY() - 2, 2) || getWorld()->isThereWall(getX() + 2, getY() - 4, 2) && getWorld()->isThereFloor(getX() + 4, getY(), 2))
			{
				setIsAlive(false);
			}
			if (getWorld()->isThereWall(getX() + 2, getY() - 4, 2) && getWorld()->isThereFloor(getX() + 4, getY(), 2)) //for cases when the ball just goesd over space cuxz it would go downa and forwards too many pixels
			{
				return;
			}
		}

		moveTo(getX(), getY() - 2);
		if (getDirection() == 0)
		{
			moveTo(getX() + 2, getY());
		}
		if (getDirection() == 180)
		{
			moveTo(getX() - 2, getY());
		}
	}
	if (getWorld()->isThereFloor(getX(), getY(), 2))
	{
		if (getWorld()->isThereWall(getX(), getY(), 2))
		{
			setIsAlive(false);
			return;
		}

	}
	
}

void Projectile::doSomething()
{
	moveMech();
}

int Projectile::moveMechEdit()
{
	if (getDirection() == 0)
		return getWorld()->isOverlappingBall(getX() - 2, getY(), this);
	else
		return getWorld()->isOverlappingBall(getX() + 2, getY(), this);

}
//PEACH FIRE BALL///////
PeachFireBall::PeachFireBall(int x, int y, int dir,StudentWorld* ptr)
:Projectile(IID_PEACH_FIRE, x, y, ptr, false, false, false, false, false, true, dir,1,1)

{}

PeachFireBall::~PeachFireBall()
{}


//PIRANHA FIRE BALL ///////////
PirFireBall::PirFireBall(int x, int y, int dir, StudentWorld* ptr)
	:Projectile(IID_PIRANHA_FIRE, x, y,ptr, false, false, true,false, false, true, dir,1,1)

{}

PirFireBall::~PirFireBall()
{}

int PirFireBall::moveMechEdit()
{
	if (getDirection() == 0)
	{
		return 4;
	}
	else return -4;
}

void PirFireBall::doSomething()
{
	if (getWorld()->isOverlappingCoord(getX(), getY(), 2))
	{
		getWorld()->peachPtr()->bonk();
		setIsAlive(false);
		return;
	}
	moveMech();

}
//SHELL/////////////////
Shell::Shell(int x, int y, int dir, StudentWorld* ptr)
	: Projectile(IID_SHELL, x, y, ptr, false, false, false, false, false, false, dir, 1, 1)

{}

Shell::~Shell()
{}



//ENEMY//////////////////////////////////////////
 
Enemy::Enemy(int imageID, int x, int y, int dir, int depth, StudentWorld* ptr)
	: Actor(imageID, x, y, ptr, true, false, true, false, false, false, dir, depth,1)
{}

Enemy::~Enemy()
{}

void Enemy::bonk()
{
	getWorld()->playSound(SOUND_PLAYER_KICK);
	getWorld()->increaseScore(100);
	setIsAlive(false);
	bonkEdit();
}


void Enemy::doSomething()//the commeneted numbers correlete to the numbers in the spec sheet
{
	if (!getIsAlive()) //(1)
	{
		return;
	}
	if (getWorld()->isOverlappingCoord(getX(), getY(), 1)) //(2)
	{
		getWorld()->peachPtr()->bonk();
		return;
	}

	if (getWorld()->isThereWall(getX(), getY(), 1)) //(3)
	{
		if (getDirection() == 0)
		{
			if (!getWorld()->isThereWall(getX() - 1, getY(), 1))
				setDirection(180);
		}
		if (getDirection() == 180)
		{
			if (!getWorld()->isThereWall(getX() + 1, getY(), 1))
				setDirection(0);
		}
	}

	if (getWorld()->isThereFloor(getX(), getY(), 1)) //(4)
	{

		if (getDirection() == 0)
		{
			if (!getWorld()->isThereFloor(getX() + 8, getY(), 1))
			{
				setDirection(180);
			}
		}
		else
		{
			if (!getWorld()->isThereFloor(getX() - 8, getY(), 1))
			{
				setDirection(0);
			}
		}
	}

	if (getDirection() == 0) //(5)
	{
		if (!getWorld()->isThereWall(getX() - 1, getY()-2, 1))
			return;
		else moveTo(getX() + 1, getY());
	}
	if (getDirection() == 180)
	{
		if (!getWorld()->isThereWall(getX() + 1, getY()-2, 1))
			return;
		else moveTo(getX() - 1, getY());
	}
}

void Enemy::bonkEdit()
{
	return;
}

//GOOMBA////////////
Goomba::Goomba(int imageID, int x, int y, int dir, int depth, StudentWorld* ptr)
	:Enemy(imageID, x, y, dir, depth, ptr)
{
	
}

Goomba::~Goomba()
{}

//KOOPA////////////////

Koopa::Koopa(int imageID, int x, int y, int dir, int depth, StudentWorld* ptr)
	:Enemy(imageID, x, y, dir, depth, ptr)
{

}

Koopa::~Koopa()
{}

void Koopa::bonkEdit()
{
	getWorld()->makeShell(getX(), getY(), getDirection());
}


//PIRANHA
Piranha::Piranha(int imageID, int x, int y, int dir, int depth, StudentWorld* ptr)
	:Enemy(imageID, x, y, dir, depth, ptr)
{}

Piranha::~Piranha()
{}

bool Piranha::canFire()
{
	if (fireDelay == 0)
	{
		return true;
	}
	else return false;
}
void Piranha::setFireDelay()
{
	fireDelay = 40;
}
void Piranha::decFireDelay()
{
	fireDelay--;
}


void Piranha::doSomething()
{
	if (!getIsAlive()) //(1)
	{
		return;
	}
	increaseAnimationNumber(); //(2)

	if (getWorld()->isOverlappingCoord(getX(), getY(), 0)) //(3)
	{
		getWorld()->peachPtr()->bonk();
		if (getWorld()->peachPtr()->getStarPower())
		{
			bonk();
		}
		return;
	}

	if (!(getY() + 1.5 * SPRITE_HEIGHT >= getWorld()->peachPtr()->getY() && getY() - 1.5 * SPRITE_HEIGHT <= getWorld()->peachPtr()->getY())) //(4)
	{
		return;
	}

	if (getX() - getWorld()->peachPtr()->getX() > 0) //(6)
	{
		setDirection(180);
	}
	else setDirection(0);
	
	if (!canFire()) //(7)
	{
		decFireDelay();
		return;
	}
	else //(8)
	{
		int a = getX();
		int b = getWorld()->peachPtr()->getX();
		int c;
		if (a < b)
		{
			c = b - a;
		}
		else c = a - b;

		if (c < 8 * SPRITE_WIDTH)
		{
			getWorld()->makeFireball(2, getX(), getY(), getDirection());
			getWorld()->playSound(SOUND_PIRANHA_FIRE);
			setFireDelay();
		}
	}

}
