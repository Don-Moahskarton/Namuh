#include "Namuh_Main_H.h"
using namespace Ogre;

void Gui_sandbox_manager::Initialise(void)
{
	this->initialise_CEGUI(); 

	layout = CEGUI::WindowManager::getSingleton().loadWindowLayout("Sandbox.layout"); 
	sys->setGUISheet(layout);
	ldbg.enable(); // A partir de la, le Live Debugger est opérationnel

	Info_Text = winMgr->getWindow("Sandbox_interface/System_info/Info_text");
	Info_Text->setText("Loading Sandbox");
	toaster = winMgr->getWindow("Sandbox_interface/Toaster");
	toaster->setText("Press Left Ctrl to display mouse");
	toasterLifeMs = -1000;
	camctrl = true;
	cameravolanteswitch = true;
	isHideWing = false;
	isHideBounce = false;
	CEGUI::MouseCursor::getSingleton().setVisible(false);
	lastFileNameSaved = "";

	// Inscription des handelers
	this->Suscribe();
	
};

void Gui_sandbox_manager::UpdateSandbox(float inv_FPS)
{
	// Gestion de la souris
	if(camctrl)
	{
		local_player.OrientationInput.x = -mouseX*0.4*(!Menuci::isthereOneVisible);
		local_player.OrientationInput.y = -mouseY*0.4*(!Menuci::isthereOneVisible);
		mouseX = mouseY = 0;
	}

	// Gestion du clavier
	local_player.MotionInput = local_player.movespeed*Ogre::Vector4(	Ogre::Real(sys0.mKeyboard->isKeyDown(OIS::KC_W)),
																		Ogre::Real(sys0.mKeyboard->isKeyDown(OIS::KC_S)),
																		Ogre::Real(sys0.mKeyboard->isKeyDown(OIS::KC_A)),
																		Ogre::Real(sys0.mKeyboard->isKeyDown(OIS::KC_D)));
	local_player.JumpInput = sys0.mKeyboard->isKeyDown(OIS::KC_SPACE);// || !local_player.CharCtrl->hasCollidedDown();

	// Ctrl donne la souris
	if(sys0.kb_releasing(OIS::KC_LCONTROL))
	{
		camctrl = !camctrl;
		CEGUI::MouseCursor::getSingleton().setVisible(!CEGUI::MouseCursor::getSingleton().isVisible());
		mouseX = mouseY = 0;

		if (toaster->getText() == "Press Left Ctrl to display mouse")
			setToaster(1,""); // hide it

	}

	// Shift pour courir
	if(sys0.mKeyboard->isKeyDown( OIS::KC_LSHIFT ) || sys0.mKeyboard->isKeyDown( OIS::KC_RSHIFT ))
		local_player.movespeed=100*inv_FPS;
	else 
		local_player.movespeed=24*inv_FPS;

	// Gestion du Toaster

	if (toasterLifeMs > 0 )	
	{
		toaster->setVisible(true);
		toasterLifeMs -= 1000 * inv_FPS;
		if (toasterLifeMs < 0) // end of life : hide it
		{
			toasterLifeMs = 0;
			toaster->setVisible(false);
		}
	}
	if (toasterLifeMs < 0 )	
		toaster->setVisible(true);
	// negative life : infinite duration

}

void Gui_sandbox_manager::UpdateDebugtext(string additionalInfoToDisplay)
{	
	Debug_Text = "FPS Actuel : " + toOgStr(int(10*sys1.FPS)/10) + "\n";
	Debug_Text += "local_player.pos : " + toOgStr( local_player.pos ) + "\n";
	Debug_Text += additionalInfoToDisplay;
	Info_Text->setText(Debug_Text);
}

void Gui_sandbox_manager::Destroy(void)
{	
	ldbg.disable();
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
}



void Gui_sandbox_manager::setToaster(int lifeDuration, string message)	
{	
	toaster->setText(message);
	toasterLifeMs = lifeDuration;
	if(lifeDuration)		 // if life is not zero (which is the only case where we don't display anything)
		toaster->setVisible(true);
}








































































