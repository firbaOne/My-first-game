//|||||||||||||||||||||||||||||||||||||||||||||||
#include "stdafx.h"
#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

class MenuState : public AppState
{
public:
    MenuState();

	DECLARE_APPSTATE_CLASS(MenuState)

	void enter();
	void createScene();
	void exit();

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);


	void update(double timeSinceLastFrame);
	/* CEGUI events handler functions */
	bool quit(const CEGUI::EventArgs &e);
	bool showOptions(const CEGUI::EventArgs &e);
	bool showGraphicsOptions(const CEGUI::EventArgs &e);
private:
	bool m_bQuit;
	CEGUI::OgreRenderer *mRenderer;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||