
/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_FPS_GUI // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_FPS_GUI

#include "Namuh_Main_H.h"
#include <OgreVector3.h>
#include "GUI.h"

class Gui_FPS_manager : public Gui_manager
{
	public:
		bool	last_firekeyA,
				last_firekeyB;
		Menuci  menucirc_armesA,
				menucirc_armesE,
				menucirc_cstr;
		
		Vie_Hud Affichage_vie;


	
		CEGUI::Window	*WinEndGame,
						*WinEndGameText,
						*WinStats;

		void Initialise(void);
		void Destroy(void);
		void UpdateFPS_UI(float);
		void UpdateStats(void);
		void MakeCstr(Ogre::uint FavIndex = 0);

};



#endif // CLASS_GUI
