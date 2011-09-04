//|||||||||||||||||||||||||||||||||||||||||||||||
#include "stdafx.h"
#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.hpp"

#include "TeamManager.h"

#define DEBUG
//|||||||||||||||||||||||||||||||||||||||||||||||

enum QueryFlags
{
	OGRE_HEAD_MASK	= 1<<0,
    CUBE_MASK		= 1<<1
};

//|||||||||||||||||||||||||||||||||||||||||||||||

class GameState : public AppState
{
public:
	GameState();

	DECLARE_APPSTATE_CLASS(GameState)

	void enter();
	void createScene();
	void exit();
	bool pause();
	void resume();

	void moveCamera();
	void getInput();
    void buildGUI();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	void onLeftPressed(const OIS::MouseEvent &evt);
   

	void update(double timeSinceLastFrame);
	bool playerDestroyed(Viper * viper);
	Viper * findViperByCollisionObject(btCollisionObject * colObj)
	{if(team1->getViperByCollisionObject(colObj) )return team1->getViperByCollisionObject(colObj); return team2->getViperByCollisionObject(colObj);}
private:
	Ogre::SceneNode*			mOgreHeadNode;
	Ogre::Entity*				mOgreHeadEntity;
	Ogre::MaterialPtr			mOgreHeadMat;
	Ogre::MaterialPtr			mOgreHeadMatHigh;

	void loadActionKeys();
	bool						m_bQuit;

	Ogre::Vector3				m_TranslateVector;
	Ogre::Real					m_MoveSpeed;
	Ogre::Degree				m_RotateSpeed;
	float						m_MoveScale;
	Ogre::Degree				m_RotScale;
	Ogre::RaySceneQuery*		mRSQ;
	Ogre::SceneNode*			mCurrentObject;
	Ogre::Entity*				mCurrentEntity;
	Ogre::SceneNode *			mCameraNode;
	bool						m_bLMouseDown, m_bRMouseDown;
	Ogre::Quaternion			mCameraDefaultOrientation;
	bool						isPlayerAlive;
	void generateEnvironment();
	void checkCollisions();
	void checkCollisionForViper(Viper * viper);
	void checkBulletsCollisionForViper(Viper * viper);
	void destroyDeadVipers();
	void destroyViper(Viper * viper);
	void updateAllVipers();
	void checkAllBulletsCollisions(const Ogre::FrameEvent& evt);
#ifdef DEBUG
	bool						m_bSettingsMode; /* I will use it as Debug Mode */
#endif
	/* Actions key codes */
	OIS::KeyCode mActionForward;
	OIS::KeyCode mActionBackward;
	OIS::KeyCode mActionLeft;
	OIS::KeyCode mActionRight;
	OIS::KeyCode mActionUp;
	OIS::KeyCode mActionDown;
	OIS::KeyCode mActionStop;

	/* Bullet variables */
	btCollisionWorld *mWorld;
	BtOgre::DebugDrawer * mDebugDrawer;
	btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;

	/* player variables */
	Viper * player;
	Ogre::Quaternion * playerQuat;
	TeamManager * team1; // player team
	TeamManager * team2; // enemy team
	bool playerShooting;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||