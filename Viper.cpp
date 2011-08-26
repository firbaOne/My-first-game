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
	mShape->setMargin(0.f);
	mColObject->setCollisionShape(mShape);
	mWorld->addCollisionObject(mColObject);
	this->mDirection = Ogre::Vector3::ZERO;
	this->mSpeed = 0;
	this->mLife = Viper::defaultLife;
	this->mScore = 0;
	this->mTeam = 0;
	this->mLastOrientation = Ogre::Quaternion::IDENTITY;
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
	 checkCollisions(evt);
	 if(mLife <= 0) destroy();
	 return true;
 }
 void Viper::checkCollisions(const Ogre::FrameEvent& evt)
 {
	  struct ContactSensorCallback : public btCollisionWorld::ContactResultCallback {
	
	//! Constructor, pass whatever context you want to have available when processing contacts
	/*! You may also want to set m_collisionFilterGroup and m_collisionFilterMask
	 *  (supplied by the superclass) for needsCollision() */
	ContactSensorCallback(btCollisionObject& tgtBody , Viper * me , const Ogre::FrameEvent& evnt/*, ... */)
		: btCollisionWorld::ContactResultCallback(), body(tgtBody), viper(me), evt(evnt) { }
	
	btCollisionObject& body; //!< The body the sensor is monitoring
	Viper * viper; //!< External information for contact processing
	const Ogre::FrameEvent& evt;
	//! If you don't want to consider collisions where the bodies are joined by a constraint, override needsCollision:
	/*! However, if you use a btCollisionObject for #body instead of a btRigidBody,
	 *  then this is unnecessary—checkCollideWithOverride isn't available */
	virtual bool needsCollision(btBroadphaseProxy* proxy) const {
		// superclass will check m_collisionFilterGroup and m_collisionFilterMask
		if(!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
			return false;
		// if passed filters, may also want to avoid contacts between constraints
		return body.checkCollideWith(static_cast<btCollisionObject*>(proxy->m_clientObject));
	}
	
	//! Called with each contact for your own processing (e.g. test if contacts fall in within sensor parameters)
	virtual btScalar addSingleResult(btManifoldPoint& cp,
		const btCollisionObject* colObj0,int partId0,int index0,
		const btCollisionObject* colObj1,int partId1,int index1)
	{
		btVector3 pt; // will be set to point of collision relative to body
		if(colObj0==&body) {
			pt = cp.m_localPointA;
		} else {
			assert(colObj1==&body && "body does not match either collision object");
			pt = cp.m_localPointB;
		}
		// do stuff with the collision point
		// viper cannot fly through other objects
		viper->transform(Ogre::Quaternion::IDENTITY, -viper->getDirection()*viper->getSpeed()*evt.timeSinceLastFrame);
		viper->getSceneNode()->setOrientation(viper->getLastOrientation());
		// it will cause some damage when crash to another object
		viper->setLife(viper->getLife() - (Viper::crashDamageConstant * viper->getSpeed()));
		viper->setSpeed(0);
		return 0; // not actually sure if return value is used for anything...?
	}
};

// USAGE:
//btRigidBody* tgtBody /* = ... */;
//YourContext foo;
ContactSensorCallback callback(*mColObject, this, evt);
mWorld->contactTest(mColObject,callback);
//this->transform(Ogre::Quaternion::IDENTITY, -mDirection*mSpeed*evt.timeSinceLastFrame);
 }
void Viper::destroy()
 {
	 mTeam->getState()->playerDestroyed(this);
	 //delete this;
 }
Viper::~Viper()
{
	OgreFramework::getSingletonPtr()->mRoot->removeFrameListener(this);
	/*mSceneNode->getParentSceneNode()->removeAndDestroyAllChildren();
	mSceneMgr->destroyEntity(mEntity);
	mWorld->removeCollisionObject(mColObject);
	delete mShape;
	delete mColObject;*/
	
}