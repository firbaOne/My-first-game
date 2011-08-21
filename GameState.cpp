//|||||||||||||||||||||||||||||||||||||||||||||||
#include "stdafx.h"
#include "GameState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

GameState::GameState()
{
    m_MoveSpeed			= 0.1f;
    m_RotateSpeed		= 0.3f;

    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
#ifdef DEBUG
    m_bSettingsMode     = false;
#endif

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
   
    mCamera->lookAt(Vector3(100, -30, 0));
	mCamera->lookAt(Vector3(100,-30,0));
    mCamera->setNearClipDistance(0.3);

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

	loadActionKeys();

    buildGUI();

    createScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::pause()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Pausing GameState...");

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
	Viper * player2 = new Viper(mSceneMgr, mWorld, Ogre::Vector3(100, 0,0));
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

    if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_I))
    {
       
    }
#ifdef DEBUG
    if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_TAB))
    {
        m_bSettingsMode = !m_bSettingsMode;
        return true;
    }

    if(m_bSettingsMode && OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_RETURN) ||
        OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_NUMPADENTER))
    {
    }

    if(!m_bSettingsMode || (m_bSettingsMode && !OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_O)))
        OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
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
	Ogre::Matrix3  mat =  Ogre::Matrix3();
	mat.FromEulerAnglesXYZ(Ogre::Degree(0), Ogre::Degree(evt.state.X.rel * -0.1f),Ogre::Degree(evt.state.Y.rel * -0.1f));
	Ogre::Quaternion quat = Ogre::Quaternion();
	quat.FromRotationMatrix(mat);
	player->transform(quat, Ogre::Vector3::ZERO);
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
    }

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::onLeftPressed(const OIS::MouseEvent &evt)
{
    if(mCurrentObject)
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
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::moveCamera()
{
    if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_LSHIFT))
        mCamera->moveRelative(m_TranslateVector);
    mCamera->moveRelative(m_TranslateVector / 10);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::getInput()
{

        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(mActionLeft))
            m_TranslateVector.x = -m_MoveScale;

        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(mActionRight))
            m_TranslateVector.x = m_MoveScale;

        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(mActionForward))
            m_TranslateVector.z = -m_MoveScale;

        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(mActionBackward))
            m_TranslateVector.z = m_MoveScale;
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

#ifdef DEBUG
	if(m_bSettingsMode)
	mDebugDrawer->step();
#endif
    getInput();
    moveCamera();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::buildGUI()
{
    
}

//|||||||||||||||||||||||||||||||||||||||||||||||



//|||||||||||||||||||||||||||||||||||||||||||||||