#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
//this is the SUPER CLASS
class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, StudentWorld* ptr, bool isDamageable, bool isBlock, bool isEnemy, bool isGoal, bool isGoodie, bool isBall, int dir = 0, int depth = 0, double size = 1.0);
	virtual void doSomething() = 0;
	
	StudentWorld* getWorld();
	virtual void bonk();

	bool getIsAlive();
	bool getIsDamageable();
	bool getIsBlock();
	bool getIsEnemy();
	bool getIsGoal();
	bool getIsGoodie();
	bool getIsBall();

	void setIsAlive(bool TrueOrFalse);

	virtual ~Actor();
private:
	bool isDamageable;
	bool isAlive;
	StudentWorld* SWPtr;
	bool isBlock;
	bool isEnemy;
	bool isGoal;
	bool isGoodie;
	bool isBall;

};


class Peach : public Actor
{
public:
	Peach(int imageID, int x, int y, StudentWorld* ptr);
	virtual void doSomething();
	bool getStarPower();
	bool getShootPower();
	bool getJumpPower();
	bool getTempInvinc();

	virtual void bonk();

	void setStarPower(int a);
	void setTempInvinc(int a);
	void setShootPower(bool a);
	void setJumpPower(bool a);

	void setHitPoints(int a);
	void setRecharge();
	
	int getHitPoints();
	int getRecharge();
	int getJumpDist();
	bool canPeachMove(double FX, double FY, bool fromFall); //returns true if peach can move

	virtual ~Peach();
private:
	int tempInvinc;
	int starPower;
	int shootPower;
	int jumpPower;
	int hitPoints;

	int time_to_recharge_before_next_fire;
	
	int remaining_jump_distance;
};

//Stationary is SUPER CLASS for BLOCKS and PIPES
class Stationary : public Actor
{
public:
	virtual void doSomething();
	virtual void bonk();
	virtual void bonkEdit() = 0;
	Stationary(int imageID, bool isBlock, bool isGoodie, int startX, int startY, StudentWorld* ptr, int dir = 0, int depth = 2, double size = 1.0);

	virtual ~Stationary();

private:

};

class Pipe : public Stationary
{
public:
	Pipe(int imageID, int x, int y, StudentWorld* ptr, bool isBlock);
	virtual void bonkEdit();

	virtual ~Pipe();

private:
};

class Block : public Stationary
{
public:
	Block(int imageID, int x, int y, bool isBlock, bool hasGoodie, bool flower, bool shroom, bool star, bool isGoodie, StudentWorld* ptr, int depth);
	virtual void bonkEdit();
	bool getFlower();
	bool getShroom();
	bool getStar();
	bool getHasGoodie();
	void setHasGoodie();

	virtual ~Block();

private:
	bool hasGoodie;
	bool flower;
	bool star;
	bool shroom;
};

//GOAL////// BASE FOR MARIO AND FLAG
class Goal : public Actor
{
public:
	Goal(int imageID, int x, int y, bool isFlag, StudentWorld* ptr);
	bool goalReached();
	void doSomething();

	virtual ~Goal();

private:
	bool isFlag;
};

//GOODIE////////////////
class Goodie :public Block
{
public:
	Goodie(int imageID, double x, double y, bool isBlock, bool hasGoodie, bool flower, bool shroom, bool star, StudentWorld* ptr, int depth);
	virtual void doSomething();
	void move();

	virtual ~Goodie();

private:
};
//Actor::Actor(int imageID, int startX, int startY, StudentWorld* ptr, bool isDamageable, bool isBlock, bool isEnemy, bool isGoal, bool isGoodie, int dir, int depth, double size)

class Projectile :public Actor
{
public:
	Projectile(int imageID, int startX, int startY, StudentWorld* ptr, bool isDamageable, bool isBlock, bool isEnemy, bool isGoal, bool isGoodie, bool isBall, int dir, int depth, double size);
	virtual void doSomething();
	void moveMech();
	virtual int moveMechEdit();

	virtual ~Projectile();


private:	
};


class Shell : public Projectile
{
public:
	Shell(int x, int y, int dir, StudentWorld* ptr);
	virtual ~Shell();
private:

};

class PeachFireBall :public Projectile
{
public:
	PeachFireBall(int x, int y, int dir, StudentWorld* ptr);
	virtual ~PeachFireBall();

private:
};

class PirFireBall :public Projectile
{
public:
	PirFireBall(int x, int y, int dir, StudentWorld* ptr);
	virtual void doSomething();
	int moveMechEdit();

	virtual ~PirFireBall();

private:
};

class Enemy : public Actor
{
public:
	Enemy(int imageID, int x, int y, int dir, int depth, StudentWorld* ptr);
	virtual void doSomething();
	virtual void bonk();
	virtual void bonkEdit();

	virtual ~Enemy();

private:
};

class Piranha : public Enemy
{
public:
	Piranha(int imageID, int x, int y, int dir, int depth, StudentWorld* ptr);
	virtual void doSomething();
	bool canFire();
	void setFireDelay();
	void decFireDelay();

	virtual ~Piranha();

private:
	int fireDelay = 0;
};

class Goomba : public Enemy
{
public:
	Goomba(int imageID, int x, int y, int dir, int depth, StudentWorld* ptr);

	virtual ~Goomba();

private:
};

class Koopa : public Enemy
{
public:
	Koopa(int imageID, int x, int y, int dir, int depth, StudentWorld* ptr);
	void bonkEdit();

	virtual ~Koopa();

private:
};

#endif // ACTOR_H_








