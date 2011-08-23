#include "stdafx.h"
#include "TeamManager.h"
unsigned int TeamManagerCounter::TeamCount = 0;
std::vector<TeamManager *> TeamManagerCounter::TeamVector;

TeamManager::TeamManager(Ogre::String material, std::string teamName)
{
	
	mID = ++TeamManagerCounter::TeamCount;
	TeamManagerCounter::TeamVector.push_back(this);
	mMaterial = material;
	if(teamName == "")
		teamName = "team " + TeamManagerCounter::TeamCount;
	mName = teamName;
}
void TeamManager::addViper(Viper * viper)
{
	viper->getEntity()->setMaterialName(mMaterial);
	viper->setTeam(this);
	mVipers.push_back(viper);
}
