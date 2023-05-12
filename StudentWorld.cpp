#include "StudentWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath), level(false), game(false)
{
}

bool StudentWorld::isThereFloor(double x, double y, int mSpeed)
{
    int a = SPRITE_WIDTH - mSpeed;//subtract the width with speed to check that many units ahead the sprite is already "there" in a sense
    for (unsigned i = 0; i < allObj.size(); i++)                                                                            
    {
        if (allObj[i]->getX() - a <= x && allObj[i]->getX() + a >= x && allObj[i]->getY() + 8 == y && allObj[i]->getIsBlock())
        {
            return true;
        }
    }
    return false;
}

bool StudentWorld::isThereWall(double x, double y, int mSpeed)
{
    int a = SPRITE_WIDTH - mSpeed;//subtract the width with speed to check that many units ahead the sprite is already "there" in a sense
    for (unsigned i = 0; i < allObj.size(); i++)
    {
        if (allObj[i]->getIsBlock())
        {
            if (allObj[i]->getX() - 8 <= x && allObj[i]->getX() + 8 >= x && allObj[i]->getY() + a >= y && allObj[i]->getY() - a <= y)
            {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::isOverlappingCoord(double x, double y, int mSpeed)//for actor to peach
{
    int a = 6 + mSpeed;//add the width with speed because we are seeing if they share a space
    for (unsigned i = 0; i < allObj.size(); i++)
    {
        if (allObj[i]->getX() - a <= x && allObj[i]->getX() + a >= x && allObj[i]->getY() - a <= y && allObj[i]->getY() + a >= y)
        {
            if (peachObj[0]->getX()-a <= x && peachObj[0]->getX() + a >= x && peachObj[0]->getY() - a <= y && peachObj[0]->getY() + a >= y)
            {
                return true;
            }
        }
    }
    return false;
}

int StudentWorld::isOverlappingBall(double x, double y, Projectile* ptr)  //for peachfirebball and enemy
{
    int a;
    for (unsigned i = 0; i < allObj.size(); i++)
    {
        if (ptr->getDirection() != allObj[i]->getDirection())
        {
            a = 10;
        }
        else a = 6;
        if (allObj[i]->getX() - a <= x && allObj[i]->getX() + a >= x && allObj[i]->getY() - 4 <= y && allObj[i]->getY() + 4 >= y)
        {
            if(allObj[i]->getIsDamageable())
            {
                ptr->setIsAlive(false);
                allObj[i]->bonk();
                return 2;//needed differnt return value when the ptr is dead because I would follow a nullptr otherwise and crash the program
            }
            return 1; 
        }
    }
    return 0;
}

bool StudentWorld::isOverlapping(double FX, double FY, bool fromFall)//for peach to actor
{
    for (unsigned i = 0; i < allObj.size(); i++)
    {
        if (allObj[i]->getX() == peachObj[0]->getX() + FX && allObj[i]->getY() == peachObj[0]->getY() + FY)
        {
            if (allObj[i]->getIsBlock())
            {
                if (!fromFall)
                    allObj[i]->bonk();
                if (FY > 0)
                    allObj[i]->bonk();
                return true;
            }
            if (allObj[i]->getIsEnemy() && peachObj[0]->getStarPower())
            {
                allObj[i]->bonk();
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::getGameEnd()
{
    return game;
}

bool StudentWorld::getLevelEnd()
{
    return level;
}

void StudentWorld::setLevelEnd(bool a)
{
    level = a;
}
void StudentWorld::setGameEnd(bool a)
{
    game = a;
}
int StudentWorld::init()
{
    ostringstream lvl;
    ostringstream num;
    num << setw(2);
    num.fill('0');
    num << number; 
    lvl << setw(5);
    lvl << "level" << num.str() << ".txt";  //combine the two strings
    string level = lvl.str();
    Level lev(assetPath());
    string level_file = level;
    Level::LoadResult result = lev.loadLevel(level_file);
    //Level::LoadResult result = lev.loadLevel("level02.txt");
    if (result == Level::load_fail_file_not_found ||
        result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_success)
    {
        Level::GridEntry ge;
        for (int x = 0; x != GRID_WIDTH; x++)
        {
            for (int y = 0; y != GRID_HEIGHT; y++)
            {
                ge = lev.getContentsOf(x, y);
                switch (ge)
                {
                case Level::peach:
                {
                    Peach* p = new Peach(IID_PEACH, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this);
                    peachObj.push_back(p);
                    break;
                }
                case Level::empty:
                    break;

                case Level::pipe:
                {
                    Actor* b = new Pipe(IID_PIPE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, this, true);
                    allObj.push_back(b);
                    break;
                }
                case Level::block:
                {                                                                  
                    Actor* b = new Block(IID_BLOCK, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true, false, false, false, false, false, this, 2);
                    allObj.push_back(b);
                    break;
                }
                case Level::star_goodie_block:                                        //BLock|hasGoodie|flower|shroom|star|isgoodie
                {
                    Actor* b = new Block(IID_BLOCK, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true, true, false, false, true, false, this, 2);
                    allObj.push_back(b);
                    break;
                }
                case Level::mushroom_goodie_block:
                {
                    Actor* b = new Block(IID_BLOCK, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true, true, false, true, false, false, this, 2);
                    allObj.push_back(b);
                    break;
                }
                case Level::flower_goodie_block:
                {
                    Actor* b = new Block(IID_BLOCK, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true, true, true, false, false, false, this, 2);
                    allObj.push_back(b);
                    break;
                }
                case Level::flag:
                {
                    Actor* b = new Goal(IID_FLAG, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, true, this);
                    allObj.push_back(b);
                    break;
                }
                case Level::mario:
                {
                    Actor* b = new Goal(IID_MARIO, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, false, this);
                    allObj.push_back(b);
                    break;
                }
                case Level::piranha:
                {
                    Actor* b = new Piranha(IID_PIRANHA, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, randDir() , 0, this);
                    allObj.push_back(b);
                    break;
                } 
                case Level::goomba: 
                {
                    Actor* b = new Goomba(IID_GOOMBA, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, randDir(), 0, this);
                    allObj.push_back(b);
                    break;
                }
               
                case Level::koopa:
                {
                    Actor* b = new Koopa(IID_KOOPA, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, randDir(), 0, this);
                    allObj.push_back(b);
                    break;
                }

                default:
                    break;

                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::randDir()
{
    if (randInt(0, 1))
    {
        return 180;
    }
    else return 0;
}

int StudentWorld::move()
{ 
    peachObj[0]->doSomething();
    for (unsigned i = 0; i < allObj.size(); i++)
    {
        if (allObj[i]->getIsAlive())
        {
            allObj[i]->doSomething();
        }
        else
        {
            vector<Actor*>::iterator it = allObj.begin();
            for (; it != allObj.end();)
            {
                if (!((*it)->getIsAlive()))
                {
                    delete (*it);
                    it = allObj.erase(it);
                }
                else
                    it++;
            }
        }
    } 
    if (!(peachObj[0]->getIsAlive()))
    {
        decLives();
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    if (getLevelEnd())
    {
        number++;
        setLevelEnd(false);
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    if (getGameEnd())
    {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }

    ostringstream oss;
    ostringstream points;
    points.fill('0');
    points << setw(6) << getScore();
    oss << "Lives:  " << getLives() << "  " << "Level:  " << getLevel() << "  " << "Points:  " << points.str();
    if (peachObj[0]->getStarPower())
    {
        oss << "  " << "StarPower!";
    }
    if (peachObj[0]->getShootPower())
    {
        oss << "  " << "ShootPower!";
    }
    if (peachObj[0]->getJumpPower())
    {
        oss << "  " << "JumpPower!";
    }
    setGameStatText(oss.str());
    return GWSTATUS_CONTINUE_GAME;
}



void StudentWorld::makeGoodie(int type, double x, double y)
{
    if (peachObj[0]->getY() == y - (SPRITE_HEIGHT*2)) //ensures peach is bonking from below
    {
        playSound(SOUND_POWERUP_APPEARS);

        if (type == 0) //flower
        {
            Actor* b = new Goodie(IID_FLOWER, x, y, false, true, true, false, false, this, 1);
            allObj.push_back(b);
        }
        if (type == 1) //shroom
        {
            Actor* b = new Goodie(IID_MUSHROOM, x, y, false, true, false, true, false, this, 1);
            allObj.push_back(b);
        }
        if (type == 2) //star
        {
            Actor* b = new Goodie(IID_STAR, x, y, false, true, false, false, true, this, 1);
            allObj.push_back(b);
        }
    }
}


Peach* StudentWorld::peachPtr()
{
    return peachObj[0];
}

void StudentWorld::makeFireball(int type, double x, double y, int dir)
{
    if (type == 1)
    {
        if (dir == 0)
        {
            Actor* b = new PeachFireBall(x + 4, y, dir, this);
            allObj.push_back(b);
        }
        else
        {
            Actor* b = new PeachFireBall(x - 4, y, dir, this);
            allObj.push_back(b);
        }
    }
    if (type == 2)
    {
        if (dir == 0)
        {
            Actor* b = new PirFireBall(x, y, dir, this);
            allObj.push_back(b);
        }
        else
        {
            Actor* b = new PirFireBall(x, y, dir, this);
            allObj.push_back(b);
        }
    }
}

void StudentWorld::makeShell(double x, double y, int dir)
{
    vector<Actor*>::iterator it = allObj.begin();//you have to delete the dead koopa before spawning the shell or else theres are infiite loop
    for (; it != allObj.end();)//the shell keeps attempting to kill the koopa
    {
        if (!((*it)->getIsAlive()))
        {
            delete (*it);
            it = allObj.erase(it);
        }
        else
            it++;
    }
    int a;
    if (dir == 0)
    {
        a = 2;
    }
    else a = -2;
    Actor* p = new Shell(x+a, y, dir, this);
    allObj.push_back(p);
    return;
}
void StudentWorld::cleanUp()
{ 
    delete peachObj[0];
    peachObj.clear();
    vector<Actor*>::iterator it = allObj.begin();
    for (; it != allObj.end(); )
    {
        delete (*it);
        it = allObj.erase(it);
    }
    allObj.clear();
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}










