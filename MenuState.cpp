//|||||||||||||||||||||||||||||||||||||||||||||||
#include "stdafx.h"
#include "MenuState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

MenuState::MenuState()
{
    m_bQuit         = false;
    m_FrameEvent    = Ogre::FrameEvent();
	mRenderer = 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::enter()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Entering MenuState...");

    mSceneMgr = OgreFramework::getSingletonPtr()->mRoot->createSceneManager(ST_GENERIC, "MenuSceneMgr");
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

    mCamera = mSceneMgr->createCamera("MenuCam");
    mCamera->setPosition(Vector3(0, 25, -50));
    mCamera->lookAt(Vector3(0, 0, 0));
    mCamera->setNearClipDistance(1);

    mCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->mViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->mViewport->getActualHeight()));

    OgreFramework::getSingletonPtr()->mViewport->setCamera(mCamera);
	/* CEGUI initialization */
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    createScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::createScene()
{
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
	CEGUI::Window *guiRoot = CEGUI::WindowManager::getSingleton().loadWindowLayout("Menu.layout"); 
	CEGUI::System::getSingleton().setGUISheet(guiRoot);
	CEGUI::MouseCursor::getSingleton().setImage( CEGUI::System::getSingleton().getDefaultMouseCursor());
	
	CEGUI::PushButton* pQuitButton = (CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("Root/Menu/buttonQuit");
	pQuitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::quit, this));

	CEGUI::PushButton* pOptionsButton = (CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("Root/Menu/buttonOptions");
	pOptionsButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::showOptions, this));
	
	CEGUI::PushButton* pOptionsGraphicsBut = (CEGUI::PushButton *)CEGUI::WindowManager::getSingleton().getWindow("Root/Options/buttonGraphics");
	pOptionsGraphicsBut->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::showGraphicsOptions, this));
}
bool MenuState::quit(const CEGUI::EventArgs &e)
{
	m_bQuit = true;
	return true;
}
bool MenuState::showOptions(const CEGUI::EventArgs &e)
{
	CEGUI::FrameWindow * pOptionsWindow = (CEGUI::FrameWindow *)CEGUI::WindowManager::getSingleton().getWindow("Root/Options");
	pOptionsWindow->setVisible(true);
	return true;
}
bool MenuState::showGraphicsOptions(const CEGUI::EventArgs &e)
{
	CEGUI::FrameWindow * pOptionsWindow = (CEGUI::FrameWindow *)CEGUI::WindowManager::getSingleton().getWindow("Root/Options/Graphics");
	static const RenderSystemList* lstRend;
    RenderSystemList::const_iterator pRend;
    static ConfigOptionMap opts;
	Ogre::RenderSystem * selectedRenderSystem = OgreFramework::getSingletonPtr()->mRoot->getRenderSystem();
	lstRend = &Root::getSingleton().getAvailableRenderers();
    pRend = lstRend->begin();            
    int i = 0;
    while (pRend != lstRend->end())
    {

        if (*pRend == selectedRenderSystem)
        {
            // Select
            // Refresh Options
            // Get options from render system
            opts = (*pRend)->getConfigOptions();
            // Reset list box
           
            //SendMessage(hwndDlgItem, LB_RESETCONTENT, 0, 0);
            // Iterate through options
            ConfigOptionMap::iterator pOpt = opts.begin();
            String strLine;
            while( pOpt!= opts.end() )
            {
                strLine = pOpt->second.name + ": " + pOpt->second.currentValue;
           
                ++pOpt;
            }
        }

        ++pRend;
        ++i;
    }

	pOptionsWindow->setVisible(true);
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::exit()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Leaving MenuState...");

    mSceneMgr->destroyCamera(mCamera);
    if(mSceneMgr)
        OgreFramework::getSingletonPtr()->mRoot->destroySceneManager(mSceneMgr);

   
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        m_bQuit = true;
        return true;
    }

    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
 
    default:
        return CEGUI::LeftButton;
    }
}
bool MenuState::mouseMoved(const OIS::MouseEvent &evt)
{
    CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
	// Scroll wheel.
	if (evt.state.Z.rel)
		sys.injectMouseWheelChange(evt.state.Z.rel / 120.0f);
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
   

    if(m_bQuit == true)
    {
        shutdown();
        return;
    }
}

//|||||||||||||||||||||||||||||||||||||||||||||||



//|||||||||||||||||||||||||||||||||||||||||||||||