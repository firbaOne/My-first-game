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
	mShape->setMargin(0.0001f);
	mColObject->setCollisionShape(mShape);
	mWorld->addCollisionObject(mColObject, COL_VIPER);
	this->mDirection = Ogre::Vector3::ZERO;
	this->mSpeed = 0;
	this->mLife = Viper::defaultLife;
	this->mScore = 0;
	this->mLastOrientation = Ogre::Quaternion::IDENTITY;
	this->mName = name;
	mTeam = team;
	mEntity->setMaterialName(materialName);
	lastCollidedWith = mColObject; // I must set this because of testing in GameState::destroyDeadVipers()
	mSceneNode->createChildSceneNode(mName + "leftCanon", Ogre::Vector3(10,0.3, -2.55));
	mSceneNode->createChildSceneNode(mName + "rightCanon", Ogre::Vector3(10,0.3, 2.55));
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
	 updateBullets(evt);
	 return true;
 }
 void Viper::updateBullets(const Ogre::FrameEvent& evt)
 {
	 std::vector<ViperBullet *> bulletsToDestroy;
	for(std::vector<ViperBullet *>::iterator it = mBullets.begin(); it != mBullets.end(); it++)
	{
		(*it)->transform(Ogre::Quaternion::IDENTITY, (*it)->getDirection() * ViperBullet::speed * (evt.timeSinceLastFrame/1000));
		if(((*it)->getDistance() >= ViperBullet::firingRange) || (*it)->getHit() )
			bulletsToDestroy.push_back((*it));
	}
	for(std::vector<ViperBullet *>::iterator it = bulletsToDestroy.begin(); it != bulletsToDestroy.end(); it++)
	{
		removeBullet((*it));
		(*it)->destroy();
		
	}
 }
 void Viper::shoot()
 {
	 mBullets.push_back(new ViperBullet(mSceneMgr, mWorld, this->getSceneNode()->_getDerivedOrientation().xAxis(),mSceneNode->getChild(mName+"leftCanon")->_getDerivedPosition(), mSceneNode->_getDerivedOrientation()));
	 mBullets.push_back(new ViperBullet(mSceneMgr, mWorld, this->getSceneNode()->_getDerivedOrientation().xAxis(), mSceneNode->getChild(mName+"rightCanon")->_getDerivedPosition(), mSceneNode->_getDerivedOrientation()));
 }
void Viper::removeBullet(ViperBullet * bullet)
{
	for(std::vector<ViperBullet *>::iterator it = mBullets.begin(); it != mBullets.end(); it++)
	{
		if((*it) == bullet) 
		{
			mBullets.erase(it);
			return;
		}
	}

}
Viper::~Viper()
{
	//OgreFramework::getSingletonPtr()->mRoot->removeFrameListener(this);
	/* MyEntity destrcutor is called */
}