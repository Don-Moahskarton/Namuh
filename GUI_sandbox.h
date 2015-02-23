
/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS__SANDBOX_GUI // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS__SANDBOX_GUI

#include "Namuh_Main_H.h"
#include <OgreVector3.h>
#include "GUI.h"

class Gui_sandbox_manager : public Gui_manager
{
	public:
		bool camctrl,
			 cameravolanteswitch,
			 isHideWing,
			 isHideBounce;
		Menuci  menucirc_armesA,
				menucirc_armesE;
		
		Vie_Hud Affichage_vie;

		string lastFileNameSaved;

		int toasterLifeMs;


	
		CEGUI::Window	*Info_Text,
						*toaster;

		void Initialise(void);
		void Destroy(void);
		void Suscribe(void);
		void UpdateSandbox(float);
		void UpdateDebugtext(string additionalInfoToDisplay = "");
		void setToaster(int lifeDuration, string message);

		
		// =============== Handlers ================

		bool HandleSaveConstruction(const CEGUI::EventArgs&);

		bool HandleSaveConstructionAs(const CEGUI::EventArgs&);
			bool HandleSaveConstructionAsOk(const CEGUI::EventArgs&);
			bool HandleSaveConstructionAsCancel(const CEGUI::EventArgs&);

		bool HandleLoadConstruction(const CEGUI::EventArgs&);
			bool HandleLoadConstructionOk(const CEGUI::EventArgs&);
			bool HandleLoadConstructionCancel(const CEGUI::EventArgs&);

		bool Handlecameravolanteswitch(const CEGUI::EventArgs&);

		bool HandleSandboxRocher(const CEGUI::EventArgs&);

		bool HandleSandboxRondin(const CEGUI::EventArgs&);
			bool HandleSandboxRondinOrientation(const CEGUI::EventArgs&);
			bool HandleSandboxRondinLength(const CEGUI::EventArgs&);
			bool HandleSandboxRondinRadius(const CEGUI::EventArgs&);
			
		bool HandleSandboxDisk(const CEGUI::EventArgs&);
			bool HandleSandboxDiskOrientation(const CEGUI::EventArgs&);
			bool HandleSandboxDiskThichness(const CEGUI::EventArgs&);
			bool HandleSandboxDiskRadius(const CEGUI::EventArgs&);

		bool HandleSandboxTriangle(const CEGUI::EventArgs&);
			bool HandleSandboxTriangleOrientation(const CEGUI::EventArgs&);
			bool HandleSandboxTriangleDirection(const CEGUI::EventArgs&);
			bool HandleSandboxTriangleLength(const CEGUI::EventArgs&);
			bool HandleSandboxTriangleRadius(const CEGUI::EventArgs&);

		bool HandleSandboxHide(const CEGUI::EventArgs&);
			bool HandleSandboxHideWing(const CEGUI::EventArgs&);
			bool HandleSandboxHideBounce(const CEGUI::EventArgs&);

		bool HandleSwitchConstruction(const CEGUI::EventArgs&);

		bool HandleDelObj(const CEGUI::EventArgs&);	

		bool HandleNewLnk(const CEGUI::EventArgs&);	
			bool HandleNewLnkOk(const CEGUI::EventArgs&);
			bool HandleNewLnkCancel(const CEGUI::EventArgs&);	
			bool HandleNewLnkPickFrstObjBttn(const CEGUI::EventArgs&);	
			bool HandleNewLnkPickSecondObjBttn(const CEGUI::EventArgs&);

		bool HandleDelLnk(const CEGUI::EventArgs&);	

			

};

// Déclaration du gestionnaire d'interface
extern Gui_sandbox_manager GuiSandboxMan;

#endif // CLASS_GUI
