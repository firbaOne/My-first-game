#include "stdafx.h"
#include "MyEntity.h"

MyEntity::MyEntity(std::string meshFileName, Ogre::SceneManager *sceneMgr, btCollisionWorld * world, Ogre::Vector3 position  ,CollisionShapes shape )
{
	mSceneMgr = sceneMgr;
	mEntity = mSceneMgr->createEntity(meshFileName);
	mSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(position);
	mSceneNode->attachObject(mEntity);
	mWorld = world;
	BtOgre::StaticMeshToShapeConverter * mConverter = new BtOgre::StaticMeshToShapeConverter(mEntity);
	switch(shape)
	{
	case BOX:
		mShape = mConverter->createBox(); 
		break;
	case SPHERE:
		mShape = mConverter->createSphere(); 
		break;
	case CONVEX: default:
		mShape = mConverter->createConvex(); 
		break;
	}
	delete mConverter;
	mColObject = new btCollisionObject();
	mShape->setMargin(0.f);
	mColObject->setCollisionShape(mShape);
	mWorld->addCollisionObject(mColObject, COL_OTHER);
	// some strange bug maybe, but the commented line not working, so I must transform created entity in order to synchronize graphical and physical position and rotation 
	// not working even with _getDerivedOrientation() and _getDerivedPosition()
	//mColObject->setWorldTransform(btTransform(BtOgre::Convert::toBullet(mSceneNode->getOrientation()), BtOgre::Convert::toBullet(mSceneNode->getPosition())));
	this->transform(Ogre::Quaternion::IDENTITY, Ogre::Vector3::ZERO);

}


void MyEntity::transform(Ogre::Quaternion  q, Ogre::Vector3  v)
{

	mSceneNode->translate(v, Ogre::Node::TransformSpace::TS_WORLD);
	mSceneNode->rotate(q, Ogre::Node::TransformSpace::TS_WORLD);
	mColObject->setWorldTransform(btTransform(BtOgre::Convert::toBullet(mSceneNode->_getDerivedOrientation()), BtOgre::Convert::toBullet(mSceneNode->_getDerivedPosition())));

}
btTransform MyEntity::getNewWorldTransform(Ogre::Quaternion  q, Ogre::Vector3  v)
{

	/*mSceneNode->translate(v, Ogre::Node::TransformSpace::TS_WORLD);
	mSceneNode->rotate(q, Ogre::Node::TransformSpace::TS_WORLD);*/
	return btTransform(BtOgre::Convert::toBullet(mSceneNode->_getDerivedOrientation()), BtOgre::Convert::toBullet(mSceneNode->_getDerivedPosition() + v));

}
void MyEntity::setScale(Ogre::Vector3 scale)
{
	mSceneNode->setScale(scale);
	mShape->setLocalScaling(BtOgre::Convert::toBullet(scale));
}
MyEntity::~MyEntity()
{
	mSceneNode->detachObject(mEntity);
	mSceneMgr->destroyEntity(mEntity);
	//mSceneNode->getParentSceneNode()->removeAndDestroyAllChildren();
	//mSceneNode = 0;
	mWorld->removeCollisionObject(mColObject);
	delete mShape;
	delete mColObject;
}