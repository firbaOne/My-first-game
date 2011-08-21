#include "stdafx.h"
#ifndef __VIPER_H__
#define __VIPER_H__
#include "MyEntity.h"
class Viper: public MyEntity
{
public:
	Viper() {}
	Viper(Ogre::SceneManager *sceneMgr, btCollisionWorld * world, Ogre::Vector3 position = Ogre::Vector3::ZERO);
	~Viper() {}
	static const int maxSpeed = 1000;
	static const int defaultLife = 1000;
	static const int acceleration = 100;
	static const std::string mMeshName;
	void destruct();
	void shoot();

private:
	Ogre::Vector3 mDirection;
	double mSpeed;
	double mLife;
	int mScore;
	std::string mName;


};
#endif