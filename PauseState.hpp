//|||||||||||||||||||||||||||||||||||||||||||||||
#include "stdafx.h"
#ifndef PAUSE_STATE_HPP
#define PAUSE_STATE_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "AppState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

class PauseState : public AppState
{
public:
    PauseState();

    DECLARE_APPSTATE_CLASS(PauseState)

    void enter();
    void createScene();
    void exit();

    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);

    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);


    void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);

    void update(double timeSinceLastFrame);

private:
    bool m_bQuit;
    bool m_bQuestionActive;
	/* CEGUI events handler functions */
	bool quit(const CEGUI::EventArgs &e);
	bool showMainMenu(const CEGUI::EventArgs &e);
	bool Continue(const CEGUI::EventArgs &e);
	bool mInject;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||