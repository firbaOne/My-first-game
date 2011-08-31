#include "stdafx.h"
#ifndef __VIPER_H__
#define __VIPER_H__
#include "MyEntity.h"
#include "ViperBullet.h"
//class OgreFramework;
//class GameState;
//#include "AdvancedOgreFramework.hpp" // due to destructor

class TeamManager; // I was getting errors due to circular dependency, so here is forward declaration
class Viper: public MyEntity, public Ogre::FrameListener
{
public:
	Viper() {}
	Viper(Ogre::SceneManager *sceneMgr, btCollisionWorld * world ,TeamManager *team , Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::String materialName = "", std::string name = "");
	~Viper() ;
	static const int maxSpeed = 100;
	static const int defaultLife = 1000;
	static const int crashDamageConstant = 30;
	static const int acceleration = 40;
	static const std::string mMeshName;
	//void destroy();
	void shoot();
	void setTeam(TeamManager * team) { mTeam = team;}
	TeamManager * getTeam() {return mTeam;}
	Ogre::Vector3 getDirection(){return mDirection;}
	void setDirection(Ogre::Vector3  vec) {mDirection = vec; mDirection.normalise(); this->setSpeed(vec.length());}
	double getSpeed(){return mSpeed;}
	void setSpeed(double speed); // defined in Viper.cpp
	const btCollisionObject * getLastCollidedWith() {return lastCollidedWith;}
	void setLastCollidedWith(const btCollisionObject * obj) {lastCollidedWith = obj;}
	double getLife() {return mLife;}
	void setLife(double life) {mLife = life;}
	Ogre::Quaternion getLastOrientation() {return mLastOrientation;}
	void setLastOrientation(Ogre::Quaternion quat) {mLastOrientation = quat;}
	std::string getName() {return mName;}
	void setName(std::string name){mName = name;}
	virtual bool update(const Ogre::FrameEvent& evt);
	void updateBullets(const Ogre::FrameEvent& evt);
	void removeBullet(ViperBullet * bullet);
	std::vector<ViperBullet *> getBullets() {return mBullets;}
private: 
	Ogre::Vector3 mDirection;
	double mSpeed;
	double mLife;
	int mScore;
	std::string mName;
	TeamManager * mTeam;
	std::vector<ViperBullet *> mBullets;
	//GameState * mState;
	//btPairCachingGhostObject * mColObject;
	const btCollisionObject * lastCollidedWith;
	
	//void checkCollisions(const Ogre::FrameEvent& evt);
	Ogre::Quaternion mLastOrientation;
	

};
#endif