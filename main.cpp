/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test main Source File -------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
========================================================================================

ldbg.send("File "+ string(__FILE__) +" passed line n°"+ StringConverter::toString(__LINE__)); // Debug =================================

**/
#include "Namuh_Main_H.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
{
	 try 
	 {		
		while(true)
		{
			MainMenuUpdaterOutput MenuOut;
			MenuOut.LancerPartie = false;
			{
				Gui_MainMenu_manager GuiMainMenuMan;
				GuiMainMenuMan.Initialise();
				sys1.logmsg("*** Main Menu Loop reached ***");
				while(!MenuOut.LancerPartie)
				{  
					MenuOut = GuiMainMenuMan.UpdateMainMenu();
					if(sys1.MustQuit == 1){sys1.logmsg("*** (sys1) MustQuit signal recived, shutting down ***"); return false;};
				}
				GuiMainMenuMan.ConstructLoadingScreen();
				sys1.update(1/60);
				GuiMainMenuMan.Update(1);
			}

			sys1.escQuit = true;
			{
				Partie Namuh(MenuOut.Param);
				sys1.logmsg("*** Main Game loop reached ***");
				while(!sys1.MustQuit)
					Namuh.update();

			}
			sys1.resetMustQuit();
		}
	}
	catch(Ogre::Exception& e){MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!",MB_OK | MB_ICONERROR | MB_TASKMODAL);}
	return 0;
}