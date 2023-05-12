#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
#include "GraphObject.h"
using namespace std;
class Actor;
class Peach;
class Projectile;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	virtual ~StudentWorld();

	bool isOverlapping(double FX, double FY, bool fromFall);
	bool isOverlappingCoord(double x, double y, int mSpeed);
	int isOverlappingBall(double x, double y, Projectile* ptr);

	bool isThereFloor(double x, double y, int mSpeed);
	bool isThereWall(double x, double y, int mSpeed);

	void makeGoodie(int type, double x, double y);
	void makeFireball(int type, double x, double y, int dir);
	void makeShell(double x, double y, int dir);

	bool getLevelEnd();
	bool getGameEnd();

	void setLevelEnd(bool a);
	void setGameEnd(bool a);
	
	Peach* peachPtr();
	int randDir();
private:
	vector<Actor*> allObj;
	vector<Peach*> peachObj;
	int number = 1;
	bool level;
	bool game;

};


#endif // STUDENTWORLD_H_








