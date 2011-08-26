#include "stdafx.h"
#ifndef __VIPER_H__
#define __VIPER_H__
#include "MyEntity.h"
#include "AdvancedOgreFramework.hpp"
#include "TeamManager.h"
#include "GameState.hpp"
class TeamManager; // I was getting errors due to circular dependency, so here is forward declaration
class Viper: public MyEntity, public Ogre::FrameListener
{
public:
	Viper() {}
	Viper(Ogre::SceneManager *sceneMgr, btCollisionWorld * world ,Ogre::Vector3 position = Ogre::Vector3::ZERO, std::string name = "");
	~Viper() ;
	static const int maxSpeed = 50;
	static const int defaultLife = 1000;
	static const int crashDamageConstant = 30;
	static const int acceleration = 20;
	static const std::string mMeshName;
	void destroy();
	void shoot();
	void setTeam(TeamManager * team) { mTeam = team;}
	Ogre::Vector3 getDirection(){return mDirection;}
	void setDirection(Ogre::Vector3  vec) {mDirection = vec; mDirection.normalise(); this->setSpeed(vec.length());}
	double getSpeed(){return mSpeed;}
	void setSpeed(double speed); // defined in Viper.cpp
	bool getAccelerating() {return mAccelerating;}
	void setAccelerating(bool acc) {mAccelerating = acc;}
	void revertAccelerating() {mAccelerating = !mAccelerating;}
	double getLife() {return mLife;}
	void setLife(double life) {mLife = life;}
	Ogre::Quaternion getLastOrientation() {return mLastOrientation;}
	void setLastOrientation(Ogre::Quaternion quat) {mLastOrientation = quat;}
private: 
	Ogre::Vector3 mDirection;
	double mSpeed;
	double mLife;
	int mScore;
	std::string mName;
	TeamManager * mTeam;
	//btPairCachingGhostObject * mColObject;
	bool mAccelerating;
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void checkCollisions(const Ogre::FrameEvent& evt);
	Ogre::Quaternion mLastOrientation;
	

};
#endif