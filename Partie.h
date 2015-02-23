/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PARTIE// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PARTIE

#include "Namuh_Main_H.h"
#include <OgreVector3.h>
class Partie // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
	public:
		long double gametime;
		int bouclecount;
		ulong lastDebugTextUpdateTimeStamp;
		ulong lastHydraxUpdateTimeStamp;
		ulong lastframeUpdateTimeStamp;
		ulong lastHydraxRTTUpdateTimeStamp;
		Ogre::FrameEvent e;
		float inv_FPS;
		bool	isSandBox;
		NetPartieStatus Net_Partie_Status;
		Gui_FPS_manager GuiFPSMan;
		Amnesic_Faune_mgr Amnesic_Faune_man;

		Partie(PartieArg);
		~Partie(void);
		void update(void);
		void updatePlayers(void);
};

#endif // CLASS_PERSO


