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
	mColObject->setCollisionShape(mShape);
	mWorld->addCollisionObject(mColObject);
}

void MyEntity::transform(Ogre::Quaternion  q, Ogre::Vector3  v)
{
	mSceneNode->translate(v);
	mSceneNode->rotate(q);
	mColObject->setWorldTransform(btTransform(BtOgre::Convert::toBullet(mSceneNode->_getDerivedOrientation()), BtOgre::Convert::toBullet(mSceneNode->_getDerivedPosition())));
	
}