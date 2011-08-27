//|||||||||||||||||||||||||||||||||||||||||||||||
#include "stdafx.h"
#include "GameState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

GameState::GameState()
{
    m_MoveSpeed			= 0.1f;
   // m_RotateSpeed		= -0.05f;
	m_RotateSpeed       = 60;

    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
	mCameraDefaultOrientation = Ogre::Quaternion::IDENTITY;
#ifdef DEBUG
    m_bSettingsMode     = false;
#endif
	isPlayerAlive = true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::enter()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Entering GameState...");

    mSceneMgr = OgreFramework::getSingletonPtr()->mRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    mRSQ = mSceneMgr->createRayQuery(Ray());
    mRSQ->setQueryMask(OGRE_HEAD_MASK);

    mCamera = mSceneMgr->createCamera("GameCamera");
   
	mCamera->lookAt(Vector3(100,-30,0));
    mCamera->setNearClipDistance(0.3);
	mCameraDefaultOrientation = mCamera->getOrientation();
    mCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->mViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->mViewport->getActualHeight()));
	
    OgreFramework::getSingletonPtr()->mViewport->setCamera(mCamera);
	OgreFramework::getSingletonPtr()->mViewport->setBackgroundColour(Ogre::ColourValue(0.f, 0.f, 0.f));
    mCurrentObject = 0;
	/* Load Physics*/
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;
    mWorld = new btCollisionWorld(dispatcher,broadphase,collisionConfiguration);
	OgreFramework::getSingleton().mSoundManager->setSceneManager(mSceneMgr);
	loadActionKeys();

    buildGUI();

    createScene();

}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::pause()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Pausing GameState...");
	OgreFramework::getSingleton().mSoundManager->pauseAllSounds();
	CEGUI::WindowManager::getSingleton().getWindow("GameState")->setVisible(false);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::resume()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Resuming GameState...");

    //buildGUI();
	OgreFramework::getSingleton().mSoundManager->resumeAllPausedSounds();
	
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	//CEGUI::Window *guiRoot = wmgr.loadWindowLayout("game.layout"); 
	CEGUI::System::getSingleton().setGUISheet(CEGUI::WindowManager::getSingleton().getWindow("GameState"));
	wmgr.getWindow("GameState")->setVisible(true);

    OgreFramework::getSingletonPtr()->mViewport->setCamera(mCamera);
	OgreFramework::getSingletonPtr()->mViewport->setBackgroundColour(Ogre::ColourValue(0.f, 0.f, 0.f));
    m_bQuit = false;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::exit()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Leaving GameState...");

    mSceneMgr->destroyCamera(mCamera);
    mSceneMgr->destroyQuery(mRSQ);
    if(mSceneMgr)
        OgreFramework::getSingletonPtr()->mRoot->destroySceneManager(mSceneMgr);
	OgreFramework::getSingleton().mSoundManager->destroyAllSounds();
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::createScene()
{
	mSceneMgr->setSkyBox(true, "SkyBox");
    mDebugDrawer = new BtOgre::DebugDrawer(mSceneMgr->getRootSceneNode(), mWorld);
	mDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	mWorld->setDebugDrawer(mDebugDrawer);
	//player = new Viper( mSceneMgr, mWorld);
	
	team1 = new TeamManager("red",this, mSceneMgr, mWorld,  "Team01");
	team1->addViper(Ogre::Vector3(-20, 5, 0), "Player");
	player = team1->getViperByName("Player");
	Ogre::SceneNode * playerSceneNode = player->getSceneNode();
	mCameraNode = playerSceneNode->createChildSceneNode("CameraNode", Ogre::Vector3(-4, 5, 0));
	mCameraNode->attachObject(mCamera);
	//Viper * player2 = new Viper(mSceneMgr, mWorld, Ogre::Vector3(100, 0,0));
	//player2->setDirection(Ogre::Vector3(10,0,0));
	//player2->setSpeed(50);
	//player2->transform(Ogre::Quaternion::IDENTITY, Ogre::Vector3::ZERO);
	team2 = new TeamManager("blue",this,  mSceneMgr, mWorld,"Team02");
	team2->addViper(Ogre::Vector3(100, 0,0));
	OgreFramework::getSingleton().mSoundManager->createSound("GameBackgroundMusic", "background_music.ogg", false, true, true) ;
	OgreFramework::getSingleton().mSoundManager->getSound("GameBackgroundMusic")->play();
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *guiRoot = wmgr.loadWindowLayout("game.layout"); 
	CEGUI::System::getSingleton().setGUISheet(guiRoot);
	CEGUI::ProgressBar * bar = (CEGUI::ProgressBar *)wmgr.getWindow("GameState/Life");
	bar->setProgress(1.0f);
	generateEnvironment();
	

#ifndef DEBUG
	wmgr.destroyWindow("GameState/Debug");
#endif
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::generateEnvironment()
{
	MyEntity * ent = new MyEntity("asteroid1.mesh", mSceneMgr, mWorld, Ogre::Vector3(500, 100, 100));
	ent->setScale(Ogre::Vector3(10,20,30));
	

}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::loadActionKeys()
{
	/* TODO - set it from config file, for now, I just hardcoded it here*/
	mActionForward = OIS::KC_W;
	mActionBackward = OIS::KC_S;
	mActionLeft = OIS::KC_A;
	mActionRight = OIS::KC_D;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
#ifdef DEBUG
	if(m_bSettingsMode == true)
    {
        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_S))
        {
            
           
        }

        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_W))
        {
        }
    }
#endif
    if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        pushAppState(findByName("PauseState"));
        return true;
    }
#ifdef DEBUG
    if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_TAB))
    {
        m_bSettingsMode = !m_bSettingsMode;
        return true;
    }
#endif
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mouseMoved(const OIS::MouseEvent &evt)
{
    if(m_bRMouseDown)
    {
        mCamera->yaw(Degree(evt.state.X.rel * -0.1f));
        mCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
    }
	else
	{
		/*Ogre::Matrix3  mat =  Ogre::Matrix3();
		mat.FromEulerAnglesXYZ(Ogre::Degree(0), Ogre::Degree(evt.state.X.rel * -0.1f),Ogre::Degree(evt.state.Y.rel *  -0.1f));
		Ogre::Quaternion quat = Ogre::Quaternion();
		quat.FromRotationMatrix(mat);
		player->transform(quat, Ogre::Vector3::ZERO);*/
		if(isPlayerAlive)
		{
			player->setLastOrientation(player->getSceneNode()->getOrientation()); // for case that new rotation will cause collision
			player->getSceneNode()->yaw(Degree(evt.state.X.rel * -0.1f));
			player->getSceneNode()->roll(Degree(evt.state.Y.rel * -0.1f));
		}
	}
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
   

    if(id == OIS::MB_Left)
    {
        onLeftPressed(evt);
        m_bLMouseDown = true;
    }
    else if(id == OIS::MB_Right)
    {
        m_bRMouseDown = true;
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
  

    if(id == OIS::MB_Left)
    {
        m_bLMouseDown = false;
    }
    else if(id == OIS::MB_Right)
    {
        m_bRMouseDown = false;
		mCamera->setOrientation(mCameraDefaultOrientation);
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::onLeftPressed(const OIS::MouseEvent &evt)
{
/*    if(mCurrentObject)
    {
        mCurrentObject->showBoundingBox(false);
        mCurrentEntity->getSubEntity(1)->setMaterial(mOgreHeadMat);
    }

    Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(OgreFramework::getSingletonPtr()->mMouse->getMouseState().X.abs / float(evt.state.width),
        OgreFramework::getSingletonPtr()->mMouse->getMouseState().Y.abs / float(evt.state.height));
    mRSQ->setRay(mouseRay);
    mRSQ->setSortByDistance(true);

    Ogre::RaySceneQueryResult &result = mRSQ->execute();
    Ogre::RaySceneQueryResult::iterator itr;

    for(itr = result.begin(); itr != result.end(); itr++)
    {
        if(itr->movable)
        {
            OgreFramework::getSingletonPtr()->mLog->logMessage("MovableName: " + itr->movable->getName());
            mCurrentObject = mSceneMgr->getEntity(itr->movable->getName())->getParentSceneNode();
            OgreFramework::getSingletonPtr()->mLog->logMessage("ObjName " + mCurrentObject->getName());
            mCurrentObject->showBoundingBox(true);
            mCurrentEntity = mSceneMgr->getEntity(itr->movable->getName());
            mCurrentEntity->getSubEntity(1)->setMaterial(mOgreHeadMatHigh);
            break;
        }
    }*/
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::moveCamera()
{
	/*if(m_bRMouseDown)   
	{
		if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_LSHIFT))
			mCamera->moveRelative(m_TranslateVector);
		mCamera->moveRelative(m_TranslateVector / 10);
	}*/
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::getInput()
{
	Ogre::Quaternion quat = mCamera->getOrientation();

		if(m_bRMouseDown)
		{
			mCamera->setOrientation(mCameraDefaultOrientation);
		}
		Ogre::Vector3 dir = mCamera->getDerivedDirection();
		//dir.normalise();
        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(mActionLeft))
           player->getSceneNode()->pitch(Degree(m_RotateSpeed* (m_FrameEvent.timeSinceLastFrame/1000) ));

        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(mActionRight))
             player->getSceneNode()->pitch(Degree(-m_RotateSpeed* (m_FrameEvent.timeSinceLastFrame/1000) ));

        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(mActionForward))
		{
			if(player->getDirection() != Ogre::Vector3::ZERO)
			{player->setDirection((player->getDirection() * player->getSpeed()) + dir* (m_FrameEvent.timeSinceLastFrame/1000) *Viper::acceleration*2);}
			else
			{player->setDirection(dir);}
			//player->setSpeed(player->getSpeed() + Viper::acceleration * (m_FrameEvent.timeSinceLastFrame/1000));	
		}

        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(mActionBackward))
           {
				player->setDirection((player->getDirection() * player->getSpeed()) - dir* (m_FrameEvent.timeSinceLastFrame/1000) *Viper::acceleration*2);
				
				//player->setSpeed(player->getSpeed() - (Viper::acceleration/2) * (m_FrameEvent.timeSinceLastFrame/1000));
					
			}

		if(m_bRMouseDown)
		{
			mCamera->setOrientation(quat);
		}
	
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;

    if(m_bQuit == true)
    {
        popAppState();
        return;
    }

    

    m_MoveScale = m_MoveSpeed   * timeSinceLastFrame;
    m_RotScale  = m_RotateSpeed * timeSinceLastFrame;
    m_TranslateVector = Vector3::ZERO;
	
	//player->transform(Ogre::Quaternion(0, evt.state.X.rel, evt.state.Y.rel, 1), Ogre::Vector3(10,0,0) * evt.timeSinceLastFrame);
	static float cas;
	cas += timeSinceLastFrame;
	if(cas <0) cas =0;
	if(cas >500)
	{
		cas =0;
	}
	updateAllVipers();
	mWorld->performDiscreteCollisionDetection();
	checkCollisions();
	destroyDeadVipers();
	if(isPlayerAlive)
	{

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::ProgressBar * bar = (CEGUI::ProgressBar *)wmgr.getWindow("GameState/Life");
	bar->setProgress(player->getLife()/player->defaultLife);
	CEGUI::Window * text = wmgr.getWindow("GameState/Speed");
	char txt[20];
	sprintf(txt, "%.2f", player->getSpeed());
	std::string t = "Speed: ";
	t.append(txt);
	text->setText(t);
#ifdef DEBUG
	if(m_bSettingsMode)
	mDebugDrawer->step();
	CEGUI::Window * stats = wmgr.getWindow("GameState/Debug");
	char txt1[100];
	sprintf(txt1, "%.2f", OgreFramework::getSingleton().mRenderWnd->getAverageFPS());
	t = "FPS: ";
	t.append(txt1);
	/*t.append("\nBatches: ");
	sprintf(txt1, "%.2f", OgreFramework::getSingleton().mRenderWnd->getBatchCount());
	t.append(txt1);
	t.append("\nTriangles: ");
	sprintf(txt1, "%.2f", OgreFramework::getSingleton().mRenderWnd->getTriangleCount());
	t.append(txt1);*/
	stats->setText(t);
#endif
	}
    getInput();
    moveCamera();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::buildGUI()
{
    
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::playerDestroyed(Viper *viper)
{
	if(viper == player)
	{
		CEGUI::Window *win = CEGUI::WindowManager::getSingletonPtr()->getWindow("GameState/GameOver");
		win->setVisible(true);
		//team1->destroyAllVipers();
		//team2->destroyAllVipers();
		isPlayerAlive = false;
		return false;
	}
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::checkCollisionForViper(Viper * viper)
{
	struct ContactSensorCallback : public btCollisionWorld::ContactResultCallback {
	
	//! Constructor, pass whatever context you want to have available when processing contacts
	/*! You may also want to set m_collisionFilterGroup and m_collisionFilterMask
	 *  (supplied by the superclass) for needsCollision() */
	ContactSensorCallback(btCollisionObject& tgtBody , Viper * me , const Ogre::FrameEvent& evnt/*, ... */)
		: btCollisionWorld::ContactResultCallback(), body(tgtBody), viper(me), evt(evnt) { }
	
	btCollisionObject& body; //!< The body the sensor is monitoring
	Viper * viper; //!< External information for contact processing
	const Ogre::FrameEvent& evt;
	//! If you don't want to consider collisions where the bodies are joined by a constraint, override needsCollision:
	/*! However, if you use a btCollisionObject for #body instead of a btRigidBody,
	 *  then this is unnecessaryŚcheckCollideWithOverride isn't available */
	virtual bool needsCollision(btBroadphaseProxy* proxy) const {
		// superclass will check m_collisionFilterGroup and m_collisionFilterMask
		if(!btCollisionWorld::ContactResultCallback::needsCollision(proxy))
			return false;
		// if passed filters, may also want to avoid contacts between constraints
		return body.checkCollideWith(static_cast<btCollisionObject*>(proxy->m_clientObject));
	}
	
	//! Called with each contact for your own processing (e.g. test if contacts fall in within sensor parameters)
	virtual btScalar addSingleResult(btManifoldPoint& cp,
		const btCollisionObject* colObj0,int partId0,int index0,
		const btCollisionObject* colObj1,int partId1,int index1)
	{
		btVector3 pt; // will be set to point of collision relative to body
		if(colObj0==&body) {
			pt = cp.m_localPointA;
		} else {
			assert(colObj1==&body && "body does not match either collision object");
			pt = cp.m_localPointB;
		}
		// do stuff with the collision point
		// viper cannot fly through other objects
		viper->transform(Ogre::Quaternion::IDENTITY, -viper->getDirection()*viper->getSpeed()*(evt.timeSinceLastFrame/1000));
		Ogre::Vector3 vec = viper->getDirection();
		double rych = viper->getSpeed();
		viper->getSceneNode()->setOrientation(viper->getLastOrientation());
		viper->setLife(viper->getLife() - (Viper::crashDamageConstant * viper->getSpeed() ));
		viper->setSpeed(0);
		Viper *vp = viper;
		if(colObj0 == viper->getCollisionObject()) viper->setLastCollidedWith(colObj1);
		else viper->setLastCollidedWith(colObj0);
		return 0; // not actually sure if return value is used for anything...?
	}
};

// USAGE:
//btRigidBody* tgtBody /* = ... */;
//YourContext foo;
ContactSensorCallback callback(*viper->getCollisionObject(), viper, m_FrameEvent);
mWorld->contactTest(viper->getCollisionObject(),callback);
//this->transform(Ogre::Quaternion::IDENTITY, -mDirection*mSpeed*evt.timeSinceLastFrame);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::checkCollisions()
 {
	 std::vector<Viper *> vipers = TeamManagerCounter::getAllVipers();
	 for(std::vector<Viper *>::iterator it = vipers.begin(); it != vipers.end(); it++)
	{
			checkCollisionForViper( (*it));
	}
	/* vipers = team2->getAllVipers();
	  for(std::vector<Viper *>::iterator it = vipers.begin(); it != vipers.end(); it++)
	{
			checkCollisionForViper( (*it));
	}*/
 }
//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::destroyViper(Viper * viper)
 {
	TeamManager * team = viper->getTeam();
	team->removeViper(viper);
	this->playerDestroyed(viper); 
	delete viper;							 
 }

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::destroyDeadVipers()
{
	std::vector<Viper *> vipersToDestroy;
	std::vector<Viper *> vipers =TeamManagerCounter::getAllVipers();
	std::vector<Viper *>::iterator it;
	 for(it = vipers.begin(); it != vipers.end(); it++)
	{
		if((*it)->getLife() <=0 || ((*it)->getLastCollidedWith() != (*it)->getCollisionObject() && TeamManagerCounter::getViperByCollisionObject((*it)->getLastCollidedWith())->getLife() <= 0) )
			vipersToDestroy.push_back((*it));
	 }
	  for(it = vipersToDestroy.begin(); it != vipersToDestroy.end(); it++)
	{
		destroyViper((*it));
	}	

}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::updateAllVipers()
{
	std::vector<Viper *> vipers =TeamManagerCounter::getAllVipers();
	std::vector<Viper *>::iterator it;
	 for(it = vipers.begin(); it != vipers.end(); it++)
	{
		(*it)->update(m_FrameEvent);
	 }
}

//|||||||||||||||||||||||||||||||||||||||||||||||
