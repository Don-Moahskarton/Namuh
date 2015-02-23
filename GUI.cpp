#include "Namuh_Main_H.h"

using namespace Ogre;

static CEGUI::uint _considerNumpadKey(OIS::KeyCode argkey,unsigned int* text=NULL)
{
    switch (argkey)
    {
    case OIS::KC_NUMPAD0: if (text) *text=48; return 1;
    case OIS::KC_NUMPAD1: if (text) *text=49; return 2;
    case OIS::KC_NUMPAD2: if (text) *text=50; return 3;
    case OIS::KC_NUMPAD3: if (text) *text=51; return 4;
    case OIS::KC_NUMPAD4: if (text) *text=52; return 5;
    case OIS::KC_NUMPAD5: if (text) *text=53; return 6;
    case OIS::KC_NUMPAD6: if (text) *text=54; return 7;
    case OIS::KC_NUMPAD7: if (text) *text=55; return 8;
    case OIS::KC_NUMPAD8: if (text) *text=56; return 9;
    case OIS::KC_NUMPAD9: if (text) *text=57; return 10;
	case OIS::KC_DECIMAL: if (text) *text=46; return 46;
    }

    return argkey;
}

CEGUI::OgreRenderer* Gui_manager::mRenderer = 0;
CEGUI::System* Gui_manager::sys = 0;
CEGUI::WindowManager* Gui_manager::winMgr = 0;

void Gui_manager::initialise_CEGUI(void)
{
	
	CEGUI::MouseCursor::getSingleton().setVisible(!CEGUI::MouseCursor::getSingleton().isVisible());
	mouseX = mouseY = mouseZ = 0;
	sys0.mKeyboard->setEventCallback( this );
	sys0.mMouse->setEventCallback(this);
};

Gui_manager::~Gui_manager(void) //Destructeur
{
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
};


void Gui_manager::Update(float time) // Charge l'interface de la Sandbox
{

	//SetCursorPos(0,0); // le curseur win32 est indépendant de celui d'OIS
	sys0.mMouse->capture(); const OIS::MouseState &ms = sys0.mMouse->getMouseState(); // Met a jour la souris
	
	sys->injectTimePulse(time);

	sys->renderGUI();
	mouseX += ms.X.rel;
	mouseY += ms.Y.rel;
	mouseZ += ms.Z.rel;
};


 
bool Gui_manager::keyPressed( const OIS::KeyEvent &arg )
{
	Ogre::uint text = arg.text;
	sys->injectKeyDown(_considerNumpadKey(arg.key,&text));
	sys->injectChar(text);
     return true;
}
 
bool Gui_manager::keyReleased( const OIS::KeyEvent &arg )
{
 	sys->injectKeyUp(_considerNumpadKey(arg.key));
     return true;
}

 
bool Gui_manager::mouseMoved( const OIS::MouseEvent &e )
{
	sys->injectMouseMove(e.state.X.rel, e.state.Y.rel);
    return true;
}
 
bool Gui_manager::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
	if(e.state.buttonDown( OIS::MB_Right )) sys->injectMouseButtonDown(CEGUI::RightButton);
	else sys->injectMouseButtonUp(CEGUI::RightButton);
	if(e.state.buttonDown( OIS::MB_Left )) CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
	else sys->injectMouseButtonUp(CEGUI::LeftButton);
	if(e.state.buttonDown( OIS::MB_Middle )) sys->injectMouseButtonDown(CEGUI::MiddleButton);
	else sys->injectMouseButtonUp(CEGUI::MiddleButton);
    return true;
}
 
bool Gui_manager::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
	if(e.state.buttonDown( OIS::MB_Right )) sys->injectMouseButtonDown(CEGUI::RightButton);
	else sys->injectMouseButtonUp(CEGUI::RightButton);
	if(e.state.buttonDown( OIS::MB_Left )) CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
	else sys->injectMouseButtonUp(CEGUI::LeftButton);
	if(e.state.buttonDown( OIS::MB_Middle )) sys->injectMouseButtonDown(CEGUI::MiddleButton);
	else sys->injectMouseButtonUp(CEGUI::MiddleButton);
 
    return true;
}


 
void Gui_manager::CreateMsgBox(MsgBoxType type, string msg)
{
	ldbg.send("(CreateMsgBox TODO) Extend the API to create a wider variety of dialog boxes",false);
	ldbg.send("(CreateMsgBox TODO) Aw, and also make a standard prefix for all loaded layouts (Wich is time dependent, so avoid conflicts)",false);


	string prefix = "MGBox" + StringConverter::toString(sys0.timer.getMicroseconds());
	sys->getGUISheet()->addChildWindow(CEGUI::WindowManager::getSingleton().loadWindowLayout("MSGBox1Bttn.layout",prefix));
	winMgr->getWindow(prefix + "MSGBox1Bttn/msg")->setText(msg);
	winMgr->getWindow(prefix + "MSGBox1Bttn/OkBttn")->setText("Ok");

	// Suscribe new UI elements
	CEGUI::FrameWindow* msgWin = static_cast<CEGUI::FrameWindow*>(winMgr->getWindow(prefix+"MSGBox1Bttn"));
	msgWin->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&Gui_manager::HandleDeleteMsgBox, this));

	CEGUI::PushButton* Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow(prefix+"MSGBox1Bttn/OkBttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_manager::HandleDeleteMsgBox, this));
	
}


 
bool Gui_manager::HandleDeleteMsgBox(const CEGUI::EventArgs &e)
{
	// Ferme la fenètre
	CEGUI::Window *winToDestroy = static_cast<const CEGUI::WindowEventArgs&>(e).window;
	if(string(winToDestroy->getType().c_str()) == "TaharezLook/Button")
		winToDestroy = static_cast<const CEGUI::WindowEventArgs&>(e).window->getParent();

	sys->getGUISheet()->removeChildWindow(winToDestroy);
	winToDestroy->destroy();
	return true;
}



























































