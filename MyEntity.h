#include "stdafx.h"
#ifndef __MYENTITY_H__
#define __MYENTITY_H__
/* This class wil be used to contain both physical and graphical mesh and to control it */
enum CollisionShapes
{
	CONVEX = 1,
	BOX,
	SPHERE
};
class MyEntity
{
public:
	MyEntity(): mSceneNode(0), mEntity(0), mShape(0), mColObject(0) {}
	MyEntity(std::string meshFileName, Ogre::SceneManager *sceneMgr, btCollisionWorld * world, Ogre::Vector3 position = Ogre::Vector3::ZERO ,CollisionShapes shape = CONVEX );
	//MyEntity(std::string meshFileName,Ogre::SceneManager * sceneMgr, btCollisionWorld * world, Ogre::SceneNode * parentSceneNode ,CollisionShapes shape);
	virtual ~MyEntity();
	void transform(btTransform * trans);
	void transform(Ogre::Quaternion, Ogre::Vector3);
	Ogre::SceneNode * getSceneNode() { return mSceneNode;}
	Ogre::Entity * getEntity() {return mEntity;}
	void setScale(Ogre::Vector3 scale);
	//void setScale(btVector3 scale){setScale(BtOgre::Convert::toOgre(scale));}
private:
	Ogre::SceneNode * mSceneNode;
	Ogre::Entity *mEntity;
	btCollisionShape *mShape;
	btCollisionObject * mColObject;
	Ogre::SceneManager * mSceneMgr;
	btCollisionWorld * mWorld;
	friend class Viper;
};
#endif
