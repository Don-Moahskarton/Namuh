/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.* 
=====================================================================================**/
#ifndef CLASS_GUI // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_GUI

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

enum MsgBoxType
{
	BttnOk,
	BttnOkCancel
};

class Gui_manager : public OIS::KeyListener, public OIS::MouseListener
{
	public:
		static CEGUI::OgreRenderer* mRenderer;
		static CEGUI::System* sys;
		static CEGUI::WindowManager* winMgr;

		CEGUI::Window* layout;
		float		  mouseX,
					  mouseY,
					  mouseZ;
		
   // Gui_manager(void); //Constructeur
    ~Gui_manager(void); //Destructeur
	void initialise_CEGUI(void);
	bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e ); 
    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );
	void Update(float);

	void CreateMsgBox(MsgBoxType type, string msg);
		bool HandleDeleteMsgBox(const CEGUI::EventArgs&);


};

// Déclaration du gestionnaire d'interface
//extern Gui_manager GuiMan; Non : on déclare juste les gestionnaire hérités (Car spécialisés dans chaque Layout)

#endif // CLASS_GUI
