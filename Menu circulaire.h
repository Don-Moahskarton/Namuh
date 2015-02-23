/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_MENU_CIRC_MANAGER// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_MENU_CIRC_MANAGER

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

enum Menuci_Type
{
 Menuci_Type_Two		= 0,
 Menuci_Type_Three		= 1,
 Menuci_Type_Four       = 2,
 Menuci_Type_Five		= 3,
 Menuci_Type_Six        = 4,
 Menuci_Type_Seven		= 5,
 Menuci_Type_Heigt		= 6,
 Menuci_Type_MainMenu	= 100
};

class Menuci // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
	
	CEGUI::Window *curseur,
				  *highlight,
				  *Menu,
				  *ParentWin;
	public:
	static int next_id;
	int id,
		output,
		posX,
		posY,
		Current_highlighted;
	bool isVisible;
	static bool isthereOneVisible;
	Menuci_Type type;
	
    Menuci(void);
    ~Menuci(void);
    void create(Menuci_Type, int requested_posX = sys0.vp->getActualWidth()/2, int requested_posY = sys0.vp->getActualHeight()/2, string ParentWindow = "root");
	void setAllwaysOnTop(bool AllwaysOnTop = true);
	void update(void);
};

#endif // CLASS_PERSO


















































