#include "stdafx.h"
#include "Viper.h"
 const std::string Viper::mMeshName = "viper.mesh";
Viper::Viper(Ogre::SceneManager *sceneMgr, btCollisionWorld *world, Ogre::Vector3 position)
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
}