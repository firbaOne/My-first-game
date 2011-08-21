//|||||||||||||||||||||||||||||||||||||||||||||||
#include "stdafx.h"
#include "AdvancedOgreFramework.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> OgreFramework* Ogre::Singleton<OgreFramework>::ms_Singleton = 0;

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::OgreFramework()
{
    mRoot				= 0;
    mRenderWnd		= 0;
    mViewport			= 0;
    mLog				= 0;
    mTimer			= 0;

    mInputMgr			= 0;
    mKeyboard			= 0;
    mMouse			= 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

OgreFramework::~OgreFramework()
{
    OgreFramework::getSingletonPtr()->mLog->logMessage("Shutdown OGRE...");
    if(mInputMgr)		OIS::InputManager::destroyInputSystem(mInputMgr);
    if(mRoot)			delete mRoot;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
    Ogre::LogManager* logMgr = new Ogre::LogManager();

    mLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
    mLog->setDebugOutputEnabled(true);

    mRoot = new Ogre::Root("plugins.cfg");

    if(!mRoot->showConfigDialog())
        return false;
	/*Ogre::RenderSystemList renderList= mRoot->getAvailableRenderers();
	Ogre::RenderSystem * mRenderer = renderList[0];
	mRoot->setRenderSystem(mRenderer);*/

    mRenderWnd = mRoot->initialise(true, wndTitle);

    mViewport = mRenderWnd->addViewport(0);
    mViewport->setBackgroundColour(ColourValue(0.5f, 0.5f, 0.5f, 1.0f));

    mViewport->setCamera(0);

    unsigned long hWnd = 0;
    OIS::ParamList paramList;
    mRenderWnd->getCustomAttribute("WINDOW", &hWnd);

    paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

    mInputMgr = OIS::InputManager::createInputSystem(paramList);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputMgr->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputMgr->createInputObject(OIS::OISMouse, true));

    mMouse->getMouseState().height = mRenderWnd->getHeight();
    mMouse->getMouseState().width	 = mRenderWnd->getWidth();

    if(pKeyListener == 0)
        mKeyboard->setEventCallback(this);
    else
        mKeyboard->setEventCallback(pKeyListener);

    if(pMouseListener == 0)
        mMouse->setEventCallback(this);
    else
        mMouse->setEventCallback(pMouseListener);

    Ogre::String secName, typeName, archName;
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

  

    mTimer = new Ogre::Timer();
    mTimer->reset();

    mRenderWnd->setActive(true);
	mSoundManager = OgreOggSound::OgreOggSoundManager::getSingletonPtr();
	/* I am creating scene manager here , because SounndManager needs some SceneManager to be availabe in order to be able to init() */
	Ogre::SceneManager *mSceneMgr = OgreFramework::getSingletonPtr()->mRoot->createSceneManager(ST_GENERIC, "DefaultSceneMgr");
	mSoundManager->init();
	/* CEGUI init */
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(mKeyboard->isKeyDown(OIS::KC_SYSRQ))
    {
        mRenderWnd->writeContentsToTimestampedFile("AOF_Screenshot_", ".jpg");
        return true;
    }

    if(mKeyboard->isKeyDown(OIS::KC_O))
    {
       
    }
	CEGUI::System &sys = CEGUI::System::getSingleton();
	sys.injectKeyDown(keyEventRef.key);
	sys.injectChar(keyEventRef.text);

    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    CEGUI::System::getSingleton().injectKeyUp(keyEventRef.key);
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
    
	
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
   

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void OgreFramework::updateOgre(double timeSinceLastFrame)
{
}

//|||||||||||||||||||||||||||||||||||||||||||||||