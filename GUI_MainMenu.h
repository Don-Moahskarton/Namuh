
/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS__MAINMENU_GUI // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS__MAINMENU_GUI

#include "Namuh_Main_H.h"
#include <OgreVector3.h>
#include "GUI.h"
#include "GUI_MainMenu_Datafield.h"
#include "GUI_MainMenu_CEGUI_windows.h"


enum NetPartieStatus
{
	Partie_Locale		= 1,
	Partie_Server		= 2,
	Partie_Client		= 3

};

enum Submenu
{
	Submenu_Other,
	Submenu_Main,
	Submenu_Network,
	Submenu_EscMenu,
	Submenu_PlayerProfile,
	Submenu_PlayerProfileEditFavoritesConstructions
};

class PartieArg
{
	public:
		string Mapfile;
		bool isSandbox;
		NetPartieStatus Net_Status;
		string IPofSrv;
		PartieArg(): Mapfile(""), isSandbox(true), Net_Status(Partie_Locale), IPofSrv(""){};
};

struct MainMenuUpdaterOutput
{
	bool LancerPartie;
	PartieArg Param;
};

class Gui_MainMenu_manager : public Gui_manager
{
	public:

		Gui_MainMenu_manager();

		Menuci	Main,
				Network,
				EscMenu;
		MainMenu_Datafields Data;
		MainMenu_CEGUI_windows Window;
		Submenu		Current_Submenu,
					EscMenu_Previous_Submenu;
		MainMenuUpdaterOutput MenuOutData;
		bool	lastEscKey,
				IsBackGroundInitialised;

		void Initialise(void);
		void SetupBackground(void);
		void UpdateBackground(void);
		void DestroyBackground(void);

		MainMenuUpdaterOutput UpdateMainMenu(void);
		void ConstructSubmenu(Submenu);
		void DestructSubmenu(Submenu);

		void ConstructEscmenu(void);
		bool DestructEscmenu(const CEGUI::EventArgs& = *(new CEGUI::EventArgs));

		void ConstructPlayerProfilemenu(void);

		void ConstructLoadingScreen(void);




		// ============================================================================================
		//	=======================  Submenus handelers ===============================================
		// ============================================================================================

		// First profile 
		bool HandleFirstProfileNamePromtOk(const CEGUI::EventArgs&);	


		// Player profile editor
		bool HandlePlayerProfileClose(const CEGUI::EventArgs&);	
		bool HandlePlayerProfileEditFavoriteConstruction(const CEGUI::EventArgs&);	
		bool HandlePlayerProfileEditFavoriteConstructionClose(const CEGUI::EventArgs&);	
		bool HandlePlayerProfileEditFavoriteConstructionSaveAndClose(const CEGUI::EventArgs&);
		bool HandlePlayerProfileEditFavoriteConstructionSave(const CEGUI::EventArgs&);
		bool HandlePlayerProfileEditFavoriteConstructionAddBttn(const CEGUI::EventArgs&);	
		bool HandlePlayerProfileEditFavoriteConstructionEditBttn(const CEGUI::EventArgs&);	
		bool HandlePlayerProfileEditFavoriteConstructionDelBttn(const CEGUI::EventArgs&);	

		// Sandbox map selector
		bool HandleSandboxMapSelectorMapLst(const CEGUI::EventArgs&);	
		bool HandleSandboxMapSelectorLaunchBttn(const CEGUI::EventArgs&);	
		bool HandleSandboxMapSelectorCancelBttn(const CEGUI::EventArgs&);	

	//	bool HandleSandboxMapSelectorLaunchBttn(const CEGUI::EventArgs&);	
		
			
		
		
};

// Déclaration du gestionnaire d'interface
extern Gui_MainMenu_manager GuiMainMenuMan;


#endif // CLASS_GUI









































