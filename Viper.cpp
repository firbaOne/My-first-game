#include "stdafx.h"
#include "Viper.h"
 const std::string Viper::mMeshName = "viper.mesh";
 Viper::Viper(Ogre::SceneManager *sceneMgr, btCollisionWorld * world , TeamManager *team, Ogre::Vector3 position , Ogre::String materialName , std::string name )
{
	mSceneMgr = sceneMgr;
	mEntity = mSceneMgr->createEntity(mMeshName);
	mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(position);
	mSceneNode->attachObject(mEntity);
	mWorld = world;
	BtOgre::StaticMeshToShapeConverter * mConverter = new BtOgre::StaticMeshToShapeConverter(mEntity);
	mShape = mConverter->createConvex(); 
	delete mConverter;
	mColObject = new btCollisionObject();
	mShape->setMargin(0.f);
	mColObject->setCollisionShape(mShape);
	mWorld->addCollisionObject(mColObject);
	this->mDirection = Ogre::Vector3::ZERO;
	this->mSpeed = 0;
	this->mLife = Viper::defaultLife;
	this->mScore = 0;
	this->mLastOrientation = Ogre::Quaternion::IDENTITY;
	this->mName = name;
	mTeam = team;
	mEntity->setMaterialName(materialName);
	lastCollidedWith = mColObject; // I must set this because of testing in GameState::destroyDeadVipers()
	//OgreFramework::getSingletonPtr()->mRoot->addFrameListener(this);
}
 void Viper::setSpeed(double speed)
 {
	 if(speed <= Viper::maxSpeed && speed >= -Viper::maxSpeed) mSpeed = speed;
	 else if(speed < -Viper::maxSpeed) mSpeed = -Viper::maxSpeed;
	 else mSpeed = Viper::maxSpeed;
 }
 bool Viper::update(const Ogre::FrameEvent& evt)
 {
	 
	 this->transform(Ogre::Quaternion::IDENTITY, mDirection*mSpeed*(evt.timeSinceLastFrame/1000));
	 return true;
 }
 

Viper::~Viper()
{
	//OgreFramework::getSingletonPtr()->mRoot->removeFrameListener(this);
	/* MyEntity destrcutor is called */
}