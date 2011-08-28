//|||||||||||||||||||||||||||||||||||||||||||||||
#include "stdafx.h"
#include "DemoApp.hpp"

#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

DemoApp::DemoApp()
{
	mAppStateManager = 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

DemoApp::~DemoApp()
{
	delete mAppStateManager;
    delete OgreFramework::getSingletonPtr();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void DemoApp::startDemo()
{
	new OgreFramework();
	if(!OgreFramework::getSingletonPtr()->initOgre("AdvancedOgreFramework", 0, 0))
		return;

	OgreFramework::getSingletonPtr()->mLog->logMessage("Demo initialized!");

	mAppStateManager = new AppStateManager();

	MenuState::create(mAppStateManager, "MenuState");
	GameState::create(mAppStateManager, "GameState");
    PauseState::create(mAppStateManager, "PauseState");

	mAppStateManager->start(mAppStateManager->findByName("GameState")); // due to debuging in production mode switch to MenuState
}

//|||||||||||||||||||||||||||||||||||||||||||||||