#include "stdafx.h"
#ifndef __TEAM_MANAGER_H__
#define __TEAM_MANAGER_H__
#include "Viper.h"
//#include "GameState.hpp"
class GameState;
class TeamManager
{
public:
	TeamManager() {}
	TeamManager(Ogre::String material, GameState * state, std::string teamName = "");
	void addViper(Viper * viper);
	GameState * getState(){return mState;}
private:
	Ogre::String  mMaterial;
	std::string mName;
	std::vector<Viper *> mVipers;
	unsigned int mID;
	GameState *mState;
};
class TeamManagerCounter
{
public: 
	static unsigned int TeamCount;
	static std::vector<TeamManager *> TeamVector;
};

#endif