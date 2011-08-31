#include "stdafx.h"
#ifndef __VIPERBULLET_H__
#define __VIPERBULLET_H__
#include "MyEntity.h"
class ViperBullet: public MyEntity
{
public: 
	ViperBullet(){}
	ViperBullet(Ogre::SceneManager *sceneMgr, btCollisionWorld * world , Ogre::Vector3 direction ,  Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::Quaternion quat = Ogre::Quaternion::IDENTITY);
	~ViperBullet();
	static const int speed = 300;
	static const int firingRange = 1000;
	static const int bulletDamage = 50;
	Ogre::Vector3 getDirection() { return mDirection;}
	float getDistance(){return mDistance;}
	void transform(Ogre::Quaternion, Ogre::Vector3);
	void destroy();
	void setHit(bool h){mHit = h;}
	bool getHit(){return mHit;}
private:
	static const std::string materialName;
	static const std::string meshName;

	Ogre::Vector3 mDirection;
	Ogre::Vector3 mStartPosition;
	float mDistance;
	bool mHit; // just the helper variable to check whether this bullet has already collided with some object
};
#endif