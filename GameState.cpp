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
    m_bSettingsMode     = false;


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
    mCamera->setPosition(Vector3(5, 60, 60));
    mCamera->lookAt(Vector3(5, 20, 0));
    mCamera->setNearClipDistance(5);

    mCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->mViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->mViewport->getActualHeight()));

    OgreFramework::getSingletonPtr()->mViewport->setCamera(mCamera);
    mCurrentObject = 0;

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
    mSceneMgr->createLight("Light")->setPosition(75,75,75);

    DotSceneLoader* pDotSceneLoader = new DotSceneLoader();
    pDotSceneLoader->parseDotScene("CubeScene.xml", "General", mSceneMgr, mSceneMgr->getRootSceneNode());
    delete pDotSceneLoader;

    mSceneMgr->getEntity("Cube01")->setQueryFlags(CUBE_MASK);
    mSceneMgr->getEntity("Cube02")->setQueryFlags(CUBE_MASK);
    mSceneMgr->getEntity("Cube03")->setQueryFlags(CUBE_MASK);

    mOgreHeadEntity = mSceneMgr->createEntity("Cube", "ogrehead.mesh");
    mOgreHeadEntity->setQueryFlags(OGRE_HEAD_MASK);
    mOgreHeadNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode");
    mOgreHeadNode->attachObject(mOgreHeadEntity);
    mOgreHeadNode->setPosition(Vector3(0, 0, -25));

    mOgreHeadMat = mOgreHeadEntity->getSubEntity(1)->getMaterial();
    mOgreHeadMatHigh = mOgreHeadMat->clone("OgreHeadMatHigh");
    mOgreHeadMatHigh->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
    mOgreHeadMatHigh->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(m_bSettingsMode == true)
    {
        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_S))
        {
            
           
        }

        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_W))
        {
        }
    }

    if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        pushAppState(findByName("PauseState"));
        return true;
    }

    if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_I))
    {
       
    }

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
    if(m_bSettingsMode == false)
    {
        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_A))
            m_TranslateVector.x = -m_MoveScale;

        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_D))
            m_TranslateVector.x = m_MoveScale;

        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_W))
            m_TranslateVector.z = -m_MoveScale;

        if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_S))
            m_TranslateVector.z = m_MoveScale;
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

    getInput();
    moveCamera();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void GameState::buildGUI()
{
    
}

//|||||||||||||||||||||||||||||||||||||||||||||||



//|||||||||||||||||||||||||||||||||||||||||||||||