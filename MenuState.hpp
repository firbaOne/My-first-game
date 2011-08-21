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
	void generateGraphicsMenu();
	/* CEGUI events handler functions */
	bool quit(const CEGUI::EventArgs &e); /* quit whole application */
	bool showOptions(const CEGUI::EventArgs &e);
	bool hideOptions(const CEGUI::EventArgs &e);
	bool showGraphicsOptions(const CEGUI::EventArgs &e);
	bool applyGraphicsOptions(const CEGUI::EventArgs &e);
	bool hideGraphicsOptions(const CEGUI::EventArgs &e);

	bool showStart(const CEGUI::EventArgs &e);
	bool hideStart(const CEGUI::EventArgs &e);
private:
	bool m_bQuit;
	std::vector<CEGUI::Combobox *> comboboxs ;
	std::vector<CEGUI::Window *>  staticTexts ;
	
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||