#include "stdafx.h"
#include "ViperBullet.h"
const std::string ViperBullet::meshName = "ViperBulletMesh";
const std::string ViperBullet::materialName = "blue"; //placeholder
ViperBullet::ViperBullet(Ogre::SceneManager *sceneMgr, btCollisionWorld * world , Ogre::Vector3 direction,   Ogre::Vector3 position, Ogre::Quaternion  quat)
{
	mSceneMgr = sceneMgr;
	mEntity = mSceneMgr->createEntity(ViperBullet::meshName);
	mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(position);
	mSceneNode->attachObject(mEntity);
	mSceneNode->roll(Ogre::Degree(90));
	mWorld = world;
	BtOgre::StaticMeshToShapeConverter * mConverter = new BtOgre::StaticMeshToShapeConverter(mEntity);
	mShape = mConverter->createBox(); 
	delete mConverter;
	mColObject = new btCollisionObject();
	mShape->setMargin(0.0001f);
	mColObject->setCollisionShape(mShape);
	mWorld->addCollisionObject(mColObject, COL_VIPERBULLET, ~COL_VIPERBULLET);
	//int i = mColObject->getCollisionFlags();
	mColObject->setCollisionFlags(COL_VIPERBULLET);
	this->mDirection = direction;
	this->mDirection.normalise();
	mEntity->setMaterialName(ViperBullet::materialName);
	this->mStartPosition = position;
	/*Ogre::Quaternion quat;
	quat.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3(0,0,1));*/
	this->transform(quat, Ogre::Vector3::ZERO);
	this->mDistance = 0.f;
	this->mHit = false;
}
void ViperBullet::transform(Ogre::Quaternion quat, Ogre::Vector3 vec)
{
	mDistance += vec.length();
	
	MyEntity::transform(quat, vec);
}
void ViperBullet::destroy()
{
	delete this;
}
ViperBullet::~ViperBullet()
{
	
}