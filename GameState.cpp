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
	isInitialized = false;
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
	isInitialized = true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::pause()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Pausing GameState...");
	OgreFramework::getSingleton().mSoundManager->destroyAllSounds();
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::resume()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Resuming GameState...");

    buildGUI();

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
	player = new Viper( mSceneMgr, mWorld);
	Ogre::SceneNode * playerSceneNode = player->getSceneNode();
	mCameraNode = playerSceneNode->createChildSceneNode("CameraNode", Ogre::Vector3(-4, 5, 0));
	mCameraNode->attachObject(mCamera);
	TeamManager * team1 = new TeamManager("red",this,  "Team01");
	team1->addViper(player);
	Viper * player2 = new Viper(mSceneMgr, mWorld, Ogre::Vector3(100, 0,0));
	//player2->transform(Ogre::Quaternion::IDENTITY, Ogre::Vector3::ZERO);
	TeamManager * team2 = new TeamManager("blue",this, "Team02");
	team2->addViper(player2);
	OgreFramework::getSingleton().mSoundManager->createSound("MenuBackgroundMusic", "background_music.ogg", false, true, true) ;
	OgreFramework::getSingleton().mSoundManager->getSound("MenuBackgroundMusic")->play();
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *guiRoot = wmgr.loadWindowLayout("game.layout"); 
	CEGUI::System::getSingleton().setGUISheet(guiRoot);
	CEGUI::ProgressBar * bar = (CEGUI::ProgressBar *)wmgr.getWindow("Root/Life");
	bar->setProgress(1.0f);
	generateEnvironment();
	

#ifndef DEBUG
	wmgr.destroyWindow("Root/Debug");
#endif
}
void GameState::generateEnvironment()
{
	MyEntity * ent = new MyEntity("asteroid1.mesh", mSceneMgr, mWorld, Ogre::Vector3(500, 100, 100));
	ent->setScale(Ogre::Vector3(10,20,30));
	

}
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
		player->setLastOrientation(player->getSceneNode()->getOrientation()); // for case that new rotation will cause collision
		player->getSceneNode()->yaw(Degree(evt.state.X.rel * -0.1f));
		player->getSceneNode()->roll(Degree(evt.state.Y.rel * -0.1f));
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
	mWorld->performDiscreteCollisionDetection();
	if(isInitialized)
	{

	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::ProgressBar * bar = (CEGUI::ProgressBar *)wmgr.getWindow("Root/Life");
	bar->setProgress(player->getLife()/player->defaultLife);
	CEGUI::Window * text = wmgr.getWindow("Root/Speed");
	char txt[20];
	sprintf(txt, "%.2f", player->getSpeed());
	std::string t = "Speed: ";
	t.append(txt);
	text->setText(t);
#ifdef DEBUG
	if(m_bSettingsMode)
	mDebugDrawer->step();
	CEGUI::Window * stats = wmgr.getWindow("Root/Debug");
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

void GameState::playerDestroyed(Viper *viper)
{
	if(viper == player)
	popAppState();
	//pushAppState(findByName("PauseState"));
}

//|||||||||||||||||||||||||||||||||||||||||||||||