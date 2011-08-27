#include "stdafx.h"
#include "TeamManager.h"
unsigned int TeamManagerCounter::TeamCount = 0;
std::vector<TeamManager *> TeamManagerCounter::TeamVector;
std::vector<Viper *> TeamManagerCounter::mVipers;

TeamManager::TeamManager(Ogre::String material, GameState * state, Ogre::SceneManager *sceneMgr, btCollisionWorld * world,  std::string teamName)
{
	
	mID = ++TeamManagerCounter::TeamCount;
	TeamManagerCounter::TeamVector.push_back(this);
	mMaterial = material;
	if(teamName == "")
		teamName = "team " + TeamManagerCounter::TeamCount;
	mName = teamName;
	mState = state;
	mSceneMgr = sceneMgr;
	mWorld = world;
}
void TeamManager::addViper(Ogre::Vector3 position, std::string name)
{
	/*viper->getEntity()->setMaterialName(mMaterial);
	viper->setTeam(this);*/
	Viper * viper = new Viper(mSceneMgr, mWorld, this,   position, mMaterial, name);
	mVipers.push_back(viper);
	TeamManagerCounter::addViper(viper);
}
void TeamManager::removeViper(Viper *viper)
{
	for(std::vector<Viper *>::iterator it = mVipers.begin(); it != mVipers.end(); it++)
	{
		if((*it) == viper) 
		{
			mVipers.erase(it);
			TeamManagerCounter::removeViper(viper);
			return;
		}
	}

}
void TeamManager::destroyAllVipers()
{
	if(mVipers.size() > 0)
	{
		for(std::vector<Viper *>::iterator it = mVipers.begin(); it != mVipers.end(); it++)
		{
		
			
			TeamManagerCounter::removeViper((*it));	
			delete (*it);
				
		}
	}
}


Viper * TeamManager::getViperByCollisionObject(const btCollisionObject * colObj)
{
	for(std::vector<Viper *>::iterator it = mVipers.begin(); it != mVipers.end(); it++)
	{
		if((*it)->getCollisionObject() == colObj) 
		{
			
			return (*it);
		}
	}

	return false;
}
Viper * TeamManager::getViperByName(std::string name)
{
	for(std::vector<Viper *>::iterator it = mVipers.begin(); it != mVipers.end(); it++)
	{
		if((*it)->getName() == name) 
		{
			
			return (*it);
		}
	}

	return false;
}
Viper * TeamManagerCounter::getViperByName(std::string name)
{
	for(std::vector<Viper *>::iterator it = mVipers.begin(); it != mVipers.end(); it++)
	{
		if((*it)->getName() == name) 
		{
			
			return (*it);
		}
	}

	return false;
}

Viper * TeamManagerCounter::getViperByCollisionObject(const btCollisionObject * colObj)
{
	for(std::vector<Viper *>::iterator it = mVipers.begin(); it != mVipers.end(); it++)
	{
		if((*it)->getCollisionObject() == colObj) 
		{
			
			return (*it);
		}
	}

	return false;
}
void TeamManagerCounter::removeViper(Viper *viper)
{
	for(std::vector<Viper *>::iterator it = mVipers.begin(); it != mVipers.end(); it++)
	{
		if((*it) == viper) 
		{
			mVipers.erase(it);
			return;
		}
	}

}
void TeamManagerCounter::destroyAllVipers()
{
	mVipers.clear();
}
