#include "stdafx.h"
#include "Viper.h"
 const std::string Viper::mMeshName = "viper.mesh";
 Viper::Viper(Ogre::SceneManager *sceneMgr, btCollisionWorld *world,  Ogre::Vector3 position, std::string name)
{
	mSceneMgr = sceneMgr;
	mEntity = mSceneMgr->createEntity(mMeshName);
	//mEntity->setMaterialName("blue");
	mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(position);
	mSceneNode->attachObject(mEntity);
	mWorld = world;
	BtOgre::StaticMeshToShapeConverter * mConverter = new BtOgre::StaticMeshToShapeConverter(mEntity);
	mShape = mConverter->createConvex(); 
	delete mConverter;
	mColObject = new btCollisionObject();
	mColObject->setCollisionShape(mShape);
	mWorld->addCollisionObject(mColObject);
	this->mDirection = Ogre::Vector3::ZERO;
	this->mSpeed = 0;
	this->mLife = Viper::defaultLife;
	this->mScore = 0;
	this->mTeam = 0;
	if(name == "")
		name = ""; // here comes something like - this->mTeamManager->getNewViperName();
	this->mName = name;
	mAccelerating = true;
	OgreFramework::getSingletonPtr()->mRoot->addFrameListener(this);
}
 void Viper::setSpeed(double speed)
 {
	 if(speed <= Viper::maxSpeed && speed >= -Viper::maxSpeed) mSpeed = speed;
	 else if(speed < -Viper::maxSpeed) mSpeed = -Viper::maxSpeed;
	 else mSpeed = Viper::maxSpeed;
 }
 bool Viper::frameRenderingQueued(const Ogre::FrameEvent& evt)
 {
	 this->transform(Ogre::Quaternion::IDENTITY, mDirection*mSpeed*evt.timeSinceLastFrame);
	 return true;
 }