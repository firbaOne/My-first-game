//|||||||||||||||||||||||||||||||||||||||||||||||
#include "stdafx.h"
#include "MenuState.hpp"
#include <string>
//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

MenuState::MenuState()
{
    m_bQuit         = false;
    m_FrameEvent    = Ogre::FrameEvent();
	rootWindow = 0;
	actualWindow = 0;
	
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
	CEGUI::MouseCursor::getSingleton().show();
	
	CEGUI::System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
	CEGUI::MouseCursor::getSingleton().setImage( CEGUI::System::getSingleton().getDefaultMouseCursor());
    createScene();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::createScene()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window *guiRoot = wmgr.loadWindowLayout("Menu.layout"); 
	CEGUI::System::getSingleton().setGUISheet(guiRoot);
	generateGraphicsMenu();
	rootWindow = (CEGUI::FrameWindow *) wmgr.getWindow("Root/Menu");
	actualWindow = rootWindow;
	OgreFramework::getSingleton().mSoundManager->setSceneManager(mSceneMgr);
	OgreFramework::getSingleton().mSoundManager->createSound("MenuBackgroundMusic", "background_music.ogg", false, true, true) ;
	OgreFramework::getSingleton().mSoundManager->getSound("MenuBackgroundMusic")->play();

	/* Main menu Buttons event binding */
	CEGUI::PushButton* pQuitButton = (CEGUI::PushButton *)wmgr.getWindow("Root/Menu/buttonQuit");
	pQuitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::quit, this));

	CEGUI::FrameWindow * pFrameWindow = (CEGUI::FrameWindow *)wmgr.getWindow("Root/Menu");
	pQuitButton = pFrameWindow->getCloseButton();
	pQuitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::quit, this));

	CEGUI::PushButton* pOptionsButton = (CEGUI::PushButton *)wmgr.getWindow("Root/Menu/buttonOptions");
	pOptionsButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::showOptions, this));

	pOptionsButton = (CEGUI::PushButton *)wmgr.getWindow("Root/Menu/buttonStart");
	pOptionsButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::showStart, this));
	
	/*Options menu event binding */
	pFrameWindow = (CEGUI::FrameWindow *)wmgr.getWindow("Root/Options");
	pQuitButton = pFrameWindow->getCloseButton();
	pQuitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::hideOptions, this));

	CEGUI::PushButton* pOptionsGraphicsBut = (CEGUI::PushButton *)wmgr.getWindow("Root/Options/buttonGraphics");
	pOptionsGraphicsBut->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::showGraphicsOptions, this));
	
	/* Options/Graphics menu event binding */
	pFrameWindow = (CEGUI::FrameWindow *)wmgr.getWindow("Root/Options/Graphics");
	pQuitButton = pFrameWindow->getCloseButton();
	pQuitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::hideGraphicsOptions, this));

	pQuitButton = (CEGUI::PushButton *)wmgr.getWindow("Root/Options/Graphics/buttonBack");
	pQuitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::hideGraphicsOptions, this));

	pQuitButton = (CEGUI::PushButton *)wmgr.getWindow("Root/Options/Graphics/buttonApply");
	pQuitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MenuState::applyGraphicsOptions, this));
}
bool MenuState::showStart(const CEGUI::EventArgs &e)
{
	 changeAppState(findByName("GameState"));
	return true;
}
bool MenuState::hideStart(const CEGUI::EventArgs &e)
{
	/* this function should hide start game dialog, for now it does nothing, as there is no start game dialog */
	return true;
}
bool MenuState::applyGraphicsOptions(const CEGUI::EventArgs &e)
{
	/*Ogre::RenderSystem *rs = OgreFramework::getSingletonPtr()->mRoot->getRenderSystem();
	rs->setConfigOption("Full Screen", "Yes");
	rs->reinitialise();*/
	return true;
}
void MenuState::generateGraphicsMenu()
{
	/* Basic code just copied from OgreConfigDialog.h */
	CEGUI::FrameWindow *pOptionsWindow = (CEGUI::FrameWindow *)CEGUI::WindowManager::getSingleton().getWindow("Root/Options/Graphics");
	static const RenderSystemList * lstRend;
    RenderSystemList::const_iterator pRend;
    static ConfigOptionMap opts;
	Ogre::RenderSystem * selectedRenderSystem = OgreFramework::getSingletonPtr()->mRoot->getRenderSystem();
	lstRend = &OgreFramework::getSingletonPtr()->mRoot->getAvailableRenderers();

	pRend = lstRend->begin();            
    int i = 0;
    while (pRend != lstRend->end())
    {
		
		
        if (*pRend == selectedRenderSystem)
        {
	
            // Get options from render system
            opts = (*pRend)->getConfigOptions();
           
            // Iterate through options
            ConfigOptionMap::iterator pOpt = opts.begin();
			

			int it = 0;
			CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
			CEGUI::Window * parent = wmgr.getWindow("Root/Options/Graphics");
            while( pOpt!= opts.end() )
            {
				std::vector<CEGUI::ListboxTextItem *>  items ;
				comboboxs.push_back((CEGUI::Combobox *) wmgr.createWindow("TaharezLook/Combobox",(*pRend)->getName() + pOpt->second.name ));
				comboboxs[it]->setSize(CEGUI::UVector2(CEGUI::UDim(0.40, 0), CEGUI::UDim(0.15, 0)));
				comboboxs[it]->setVisible(true);
				comboboxs[it]->setPosition(CEGUI::UVector2(CEGUI::UVector2(CEGUI::UDim(0.50, 0), CEGUI::UDim(0.07 *( it+1), 0))));
				comboboxs[it]->setText(pOpt->second.currentValue);
				parent->addChildWindow(comboboxs[it]);
				unsigned int iter = 0;
				while(iter < pOpt->second.possibleValues.size())
				{
					items.push_back( new CEGUI::ListboxTextItem(pOpt->second.possibleValues[iter]));
					if(pOpt->second.possibleValues[iter] == pOpt->second.currentValue)
					{
						items[iter]->setSelected(true);
					}
					comboboxs[it]->addItem(items[iter]);
					iter++;
				}
				

				staticTexts.push_back( wmgr.createWindow("TaharezLook/StaticText" ));
				staticTexts[it]->setText(pOpt->second.name);
				staticTexts[it]->setSize(CEGUI::UVector2(CEGUI::UDim(0.40, 0), CEGUI::UDim(0.05, 0)));
				staticTexts[it]->setVisible(true);
				staticTexts[it]->setPosition(CEGUI::UVector2(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0.07 * (it+1), 0))));
				parent->addChildWindow(staticTexts[it]);
                ++pOpt;
				it++;
            }
			
        }
		
        ++pRend;
        ++i;
    }

	
}
bool MenuState::quit(const CEGUI::EventArgs &e)
{
	m_bQuit = true;
	return true;
}
bool MenuState::showOptions(const CEGUI::EventArgs &e)
{
	CEGUI::FrameWindow * pOptionsWindow = (CEGUI::FrameWindow *)CEGUI::WindowManager::getSingleton().getWindow("Root/Menu");
	pOptionsWindow->setVisible(false);
	pOptionsWindow = (CEGUI::FrameWindow *)CEGUI::WindowManager::getSingleton().getWindow("Root/Options");
	pOptionsWindow->setVisible(true);
	actualWindow = pOptionsWindow;
	return true;
}
bool MenuState::hideOptions(const CEGUI::EventArgs &e)
{
	CEGUI::FrameWindow * pOptionsWindow = (CEGUI::FrameWindow *)CEGUI::WindowManager::getSingleton().getWindow("Root/Options");
	pOptionsWindow->setVisible(false);
	pOptionsWindow = (CEGUI::FrameWindow *)CEGUI::WindowManager::getSingleton().getWindow("Root/Menu");
	pOptionsWindow->setVisible(true);
	actualWindow = rootWindow;
	return true;
}
bool MenuState::hideGraphicsOptions(const CEGUI::EventArgs &e)
{
	CEGUI::FrameWindow * pOptionsWindow = (CEGUI::FrameWindow *)CEGUI::WindowManager::getSingleton().getWindow("Root/Options/Graphics");
	pOptionsWindow->setVisible(false);
	pOptionsWindow = (CEGUI::FrameWindow *)CEGUI::WindowManager::getSingleton().getWindow("Root/Options");
	pOptionsWindow->setVisible(true);
	actualWindow = pOptionsWindow;
	return true;
}
bool MenuState::showGraphicsOptions(const CEGUI::EventArgs &e)
{
	CEGUI::FrameWindow * pOptionsWindow = (CEGUI::FrameWindow *)CEGUI::WindowManager::getSingleton().getWindow("Root/Options");
	pOptionsWindow->setVisible(false);
	pOptionsWindow = (CEGUI::FrameWindow *)CEGUI::WindowManager::getSingleton().getWindow("Root/Options/Graphics");
	pOptionsWindow->setVisible(true);
	actualWindow = pOptionsWindow;
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||

void MenuState::exit()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Leaving MenuState...");

    mSceneMgr->destroyCamera(mCamera);
    if(mSceneMgr)
        OgreFramework::getSingletonPtr()->mRoot->destroySceneManager(mSceneMgr);
	OgreFramework::getSingleton().mSoundManager->destroyAllSounds();
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
	CEGUI::MouseCursor::getSingleton().hide();
	comboboxs.clear();
	staticTexts.clear();
	//CEGUI::System::destroy();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool MenuState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(OgreFramework::getSingletonPtr()->mKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        if(actualWindow == rootWindow)
		{
			m_bQuit = true;
		}
		else
		{
			
			actualWindow->getCloseButton()->fireEvent(CEGUI::PushButton::EventClicked, CEGUI::EventArgs());
		}
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