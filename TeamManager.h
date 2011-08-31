#include "stdafx.h"
#ifndef __TEAM_MANAGER_H__
#define __TEAM_MANAGER_H__
#include "Viper.h"
class TeamManager
{
public:
	TeamManager() {}
	TeamManager(Ogre::String material, GameState * state, Ogre::SceneManager *sceneMgr, btCollisionWorld * world ,std::string teamName = "");
	~TeamManager();
	void addViper(Ogre::Vector3 position = Ogre::Vector3::ZERO, std::string name = "");
	GameState * getState(){return mState;}
	std::vector<Viper *> getAllVipers(){return mVipers;}
	void removeViper(Viper * viper);
	void destroyAllVipers();
	Viper * getViperByCollisionObject(const btCollisionObject * colObj);
	Viper * getViperByName(std::string name);
	//bool playerDestroyed(Viper * viper){return mState->playerDestroyed(viper);}
private:
	Ogre::String  mMaterial;
	std::string mName;
	std::vector<Viper *> mVipers;
	unsigned int mID;
	GameState *mState;
	Ogre::SceneManager * mSceneMgr;
	btCollisionWorld * mWorld;
};
class TeamManagerCounter
{
public: 
	static unsigned int TeamCount;
	static std::vector<TeamManager *> TeamVector;
	static void addViper(Viper * viper) {mVipers.push_back(viper);}
	static Viper * getViperByCollisionObject(const btCollisionObject *);
	static Viper * getViperByName(std::string);
	static std::vector<Viper *> getAllVipers(){return mVipers;}
private:
	static std::vector<Viper *> mVipers;
protected:
	static void removeViper(Viper * viper);
	static void destroyAllVipers();
	friend class TeamManager;
};

#endif