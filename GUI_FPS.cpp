#include "Namuh_Main_H.h"
using namespace Ogre;

void Gui_FPS_manager::Initialise(void)
{

	this ->initialise_CEGUI(); 


	layout = CEGUI::WindowManager::getSingleton().loadWindowLayout("FPS.layout"); 
	sys->setGUISheet(layout);
	if(raknet_man.isServer)
	{
		//ldbg.enable(); // A partir de la, le Live Debugger est opérationnel
		GuiSandboxMan.winMgr->getWindow("Live_debugger")->setUserString("BackgroundEnabled","True");
	}
	else		
		Affichage_vie.initialise();
	ldbg.enable(); // A partir de la, le Live Debugger est opérationnel
	WinStats = winMgr->getWindow("FPS_interface/Stats");
	WinStats->setText("Loading....\n ");

	WinEndGame = winMgr->getWindow("FPS_interface/EndGameBoxFrameWindow");
	WinEndGame->setVisible(false);
	WinEndGameText = winMgr->getWindow("FPS_interface/EndGameBox");

	CEGUI::MouseCursor::getSingleton().setVisible(false);

	menucirc_armesA.create(Menuci_Type_Four);
	menucirc_armesE.create(Menuci_Type_Four);
	menucirc_cstr.create(Menuci_Type_Four);
};

void Gui_FPS_manager::UpdateFPS_UI(float dt)
{
	if(!raknet_man.isServer)
	{
		/* ======================================================== Code Client ===========================================================================
		***************************************************************************************************************************************************/
		if(!Ruler.EndOfGame)
		{
			// Gestion de la souris
			local_player.OrientationInput.x = -mouseX*0.4*(!Menuci::isthereOneVisible);
			local_player.OrientationInput.y = -mouseY*0.4*(!Menuci::isthereOneVisible);
			mouseX = mouseY = mouseZ = 0;

			// Grstion du tir
			local_player.FireInput = sys0.ms->buttonDown(OIS::MB_Left);

			// Gestion du clavier
			local_player.MotionInput = local_player.movespeed*Ogre::Vector4(	sys0.mKeyboard->isKeyDown(OIS::KC_W),
																				sys0.mKeyboard->isKeyDown(OIS::KC_S),
																				sys0.mKeyboard->isKeyDown(OIS::KC_A),
																				sys0.mKeyboard->isKeyDown(OIS::KC_D));
			local_player.JumpInput = sys0.mKeyboard->isKeyDown(OIS::KC_SPACE) || !local_player.CharCtrl->hasCollidedDown();

			//Update l'UI
			Update(dt/5);

			// Les menus circulaires de changement d'arme
			menucirc_armesA.isVisible = sys0.mKeyboard->isKeyDown(OIS::KC_Q);
			menucirc_armesE.isVisible = sys0.mKeyboard->isKeyDown(OIS::KC_E);
			menucirc_cstr.isVisible = sys0.ms->buttonDown(OIS::MB_Middle);
			menucirc_armesA.update();
			menucirc_armesE.update();
			menucirc_cstr.update();

			// Fabrication de construction
			if(menucirc_cstr.output == 1)this->MakeCstr(0);
			if(menucirc_cstr.output == 2)this->MakeCstr(1);
			if(menucirc_cstr.output == 3)this->MakeCstr(2);
			if(menucirc_cstr.output == 4)this->MakeCstr(3);


			// Changement d'arme
			if(menucirc_armesA.output == 1)local_player.SwichWeapon(0,true);// Lance Pierre
			if(menucirc_armesA.output == 2)local_player.SwichWeapon(2,true);// Javelot
			if(menucirc_armesA.output == 4)local_player.SwichWeapon(1,true);// Arc
		}
		else
		{
			if(!WinEndGame->isVisible()) WinEndGame->setVisible(true);
			//refaire cette fenêtre
		}
	}
	else
	{

		/* ======================================================== Code Serveur ===========================================================================
	       ************************************************************************************************************************************************* */
		if(!Ruler.EndOfGame)
		{
			
			std::map<RakNet::RakNetGUID, Perso*>::iterator PlayerIterator = PlayerList.begin();
			std::map<RakNet::RakNetGUID, KeyState>::iterator KeyIterator = KeyPlayer.begin();
			while ( PlayerIterator != PlayerList.end() && KeyIterator != KeyPlayer.end() )
			{
				if(PlayerIterator->second != &local_player)
				{
					// Prédiction des mouvements de la souris (Le client reste maitre de son orientation)
					PlayerIterator->second->OrientationInput = PlayerIterator->second->rotative_velocity*dt*1000;
					//*(sys0.timer.getMilliseconds()-PlayerIterator->second->Last_rotatetime);
					Debug_Text += "rotative_velocity.x : " + StringConverter::toString(float(PlayerIterator->second->rotative_velocity.x)) + "\n";

					// Gestion des movements
					PlayerIterator->second->MotionInput = ((*raknet_man.gametime-KeyIterator->second.timestamp)/dt) // facteur de prédiction des mouvement clientside (réduit la latence percue)
														  * PlayerIterator->second->movespeed*Ogre::Vector4(	KeyIterator->second.up,
																												KeyIterator->second.down,
																												KeyIterator->second.left,
																												KeyIterator->second.right);
					PlayerIterator->second->JumpInput = KeyIterator->second.jump || !PlayerIterator->second->CharCtrl->hasCollidedDown();

					// Gestion du tir
					PlayerIterator->second->FireInput = KeyIterator->second.fire; 
				}
				PlayerIterator++;
				KeyIterator++;
			}
		}
		else
		{
			if(!WinEndGame->isVisible()) WinEndGame->setVisible(true);
			WinEndGameText->setText("Fin du Jeu ! \n Player 1 : ");//+StringConverter::toString(Ruler.PointList[0])+"pts \n Player2 : "+StringConverter::toString(Ruler.PointList[1])+"pts");
		}
	}
	
}			

void Gui_FPS_manager::UpdateStats(void)
{	
	//Debug_Text += "Points : " + StringConverter::toString(Ruler.PointList[0]) + "\n";
	Debug_Text += "FPS : " + StringConverter::toString(sys1.FPS) + "\n";
	//Debug_Text += "gametime : " + StringConverter::toString(float(*raknet_man.gametime)) + "\n";
	//Debug_Text += "rotative_velocity.x : " + StringConverter::toString(float(local_player.rotative_velocity.x)) + "\n";
	WinStats->setText(Debug_Text);
	Debug_Text = "";
}

void Gui_FPS_manager::Destroy(void)
{	
	ldbg.disable();
	CEGUI::WindowManager::getSingleton().destroyAllWindows();
}



void Gui_FPS_manager::MakeCstr(Ogre::uint FavIndex)
{	

	if(FavIndex < ProfileMgr.getCurrent()->FavoritesConstructions.size())
		CstrMgr.Load(ProfileMgr.getCurrent()->FavoritesConstructions[FavIndex],local_player.pos);
	else
		ldbg.send("(Gui_FPS_manager MakeCstr  Exception) REquested to build a favorited construction with a number out of bound. Command ignored.");
}
























































