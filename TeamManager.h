#include "stdafx.h"
#ifndef __TEAM_MANAGER_H__
#define __TEAM_MANAGER_H__
#include "Viper.h"


class TeamManager
{
public:
	TeamManager() {}
	TeamManager(Ogre::String material, std::string teamName = "");
	void addViper(Viper * viper);

private:
	Ogre::String  mMaterial;
	std::string mName;
	std::vector<Viper *> mVipers;
	unsigned int mID;
};
class TeamManagerCounter
{
public: 
	static unsigned int TeamCount;
	static std::vector<TeamManager *> TeamVector;
};
#endif