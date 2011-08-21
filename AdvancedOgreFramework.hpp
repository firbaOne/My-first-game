//|||||||||||||||||||||||||||||||||||||||||||||||
#include "stdafx.h"
#ifndef OGRE_FRAMEWORK_HPP
#define OGRE_FRAMEWORK_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||



//|||||||||||||||||||||||||||||||||||||||||||||||

class OgreFramework : public Ogre::Singleton<OgreFramework>, OIS::KeyListener, OIS::MouseListener
{
public:
	OgreFramework();
	~OgreFramework();

	bool initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MouseListener *pMouseListener = 0);
	void updateOgre(double timeSinceLastFrame);

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	Ogre::Root*					mRoot;
	Ogre::RenderWindow*			mRenderWnd;
	Ogre::Viewport*				mViewport;
	Ogre::Log*					mLog;
	Ogre::Timer*				mTimer;

	OIS::InputManager*			mInputMgr;
	OIS::Keyboard*				mKeyboard;
	OIS::Mouse*					mMouse;
	OgreOggSound::OgreOggSoundManager* mSoundManager ;
	CEGUI::OgreRenderer *mRenderer;

private:
	OgreFramework(const OgreFramework&);
	OgreFramework& operator= (const OgreFramework&);

};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||