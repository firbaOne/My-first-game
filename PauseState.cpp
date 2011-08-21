//|||||||||||||||||||||||||||||||||||||||||||||||
#include "stdafx.h"
#include "PauseState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

PauseState::PauseState()
{
    m_bQuit             = false;
    m_bQuestionActive   = false;
    m_FrameEvent        = Ogre::FrameEvent();
	mInject = true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void PauseState::enter()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Entering PauseState...");

    mSceneMgr = OgreFramework::getSingletonPtr()->mRoot->createSceneManager(ST_GENERIC, "PauseSceneMgr");
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    mCamera = mSceneMgr->createCamera("PauseCam");
    mCamera->setPosition(Vector3(0, 25, -50));
    mCamera->lookAt(Vector3(0, 0, 0));
    mCamera->setNearClipDistance(1);

    mCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->mViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->mViewport->getActualHeight()));
	OgreFramework::getSingletonPtr()->mViewport->setBackgroundColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    OgreFramework::getSingletonPtr()->mViewport->setCamera(mCamera);

    /* CEGUI initialization */
	CEGUI::MouseCursor::getSingleton().show();
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
	CEGUI::MouseCursor::getSingleton().setImage( CEGUI::System::getSingleton().getDefaultMouseCursor());
	OgreFramework::getSingleton().mSoundManager->setSceneManager(mSceneMgr);
    m_bQuit = false;

    createScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void PauseState::createScene()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *guiRoot = wmgr.loadWindowLayout("PauseMenu.layout"); 
	CEGUI::System::getSingleton().setGUISheet(guiRoot);
	OgreFramework::getSingleton().mSoundManager->setSceneManager(mSceneMgr);
	OgreFramework::getSingleton().mSoundManager->createSound("MenuBackgroundMusic", "background_music.ogg", false, true, true) ;
	OgreFramework::getSingleton().mSoundManager->getSound("MenuBackgroundMusic")->play();
	
	/* CEGUI event bidnings */
	CEGUI::PushButton* pQuitButton = (CEGUI::PushButton *)wmgr.getWindow("Root/Menu/buttonQuit");
	pQuitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseState::quit, this));

	CEGUI::FrameWindow * pFrameWindow = (CEGUI::FrameWindow *)wmgr.getWindow("Root/Menu");
	pQuitButton = pFrameWindow->getCloseButton();
	pQuitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseState::quit, this));

	CEGUI::PushButton* pOptionsButton = (CEGUI::PushButton *)wmgr.getWindow("Root/Menu/buttonMenu");
	pOptionsButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseState::showMainMenu, this));

	pOptionsButton = (CEGUI::PushButton *)wmgr.getWindow("Root/Menu/buttonContinue");
	pOptionsButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PauseState::Continue, this));
}

//|||||||||||||||||||||||||||||||||||||||||||||||
bool PauseState::quit(const CEGUI::EventArgs &e)
{
	m_bQuit = true;
	return true;
}
bool PauseState::Continue(const CEGUI::EventArgs &e)
{
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
	CEGUI::MouseCursor::getSingleton().hide();
	popAppState();
	return true;
}
bool PauseState::showMainMenu(const CEGUI::EventArgs &e)
{
	/*CEGUI::WindowManager::getSingleton().destroyAllWindows();
	CEGUI::MouseCursor::getSingleton().hide();*/
	findByName("GameState")->exit();
	changeAppState(findByName("MenuState"));
	return true;
}
void PauseState::exit()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Leaving PauseState...");
	mInject = false;
    mSceneMgr->destroyCamera(mCamera);
    if(mSceneMgr)
        OgreFramework::getSingletonPtr()->mRoot->destroySceneManager(mSceneMgr);
	OgreFramework::getSingleton().mSoundManager->destroyAllSounds();
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
	CEGUI::MouseCursor::getSingleton().hide();

}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool PauseState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
  
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool PauseState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool PauseState::mouseMoved(const OIS::MouseEvent &evt)
{
    CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
	// Scroll wheel.
	if (evt.state.Z.rel)
		sys.injectMouseWheelChange(evt.state.Z.rel / 120.0f);
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool PauseState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	 CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool PauseState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{

	CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void PauseState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    
    if(m_bQuit == true)
    {
       // popAppState();
		shutdown();
        return;
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||


//|||||||||||||||||||||||||||||||||||||||||||||||

void PauseState::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
    if(yesHit == true)
        shutdown();
    else

    m_bQuestionActive = false;
}

//|||||||||||||||||||||||||||||||||||||||||||||||