#include "Namuh_Main_H.h"
using namespace Ogre;

bool Gui_sandbox_manager::HandleLoadConstructionOk(const CEGUI::EventArgs&)
{
	string FileName = "";
	if(static_cast<CEGUI::Listbox*>(winMgr->getWindow("FileSelector/Filelist"))->getFirstSelectedItem() != NULL)
		FileName = static_cast<CEGUI::Listbox*>(winMgr->getWindow("FileSelector/Filelist"))->getFirstSelectedItem()->getText().c_str();
	if(FileName != "")
	{
		Ogre::Vector3 posToPlace = Constructeur.ComputeCursor3DPos() + Ogre::Vector3(0,15,0);
		ldbg.send("Will place construction to : " + toOgStr(posToPlace));
		Constructeur.BuiltCstr.push_back(CstrMgr.Load(FileName, posToPlace));
	

		// Crée un élément dans la liste
		CEGUI::Listbox* Cstrlist = static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("Sandbox_interface/Sandbox/Liste_constructions"));
		CEGUI::ListboxTextItem* CstrListItem = new CEGUI::ListboxTextItem(Constructeur.BuiltCstr.back()->name);
		CstrListItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
		Cstrlist->addItem(CstrListItem);

		
		Constructeur.setConstruction_num(Constructeur.BuiltCstr.size() - 1); // La construction en cours est celle que l'on viens de charger

		// Ferme la fenètre
		sys->getGUISheet()->removeChildWindow("FileSelector");
		winMgr->getWindow("FileSelector")->destroy();

	}
	return true;
}



bool Gui_sandbox_manager::HandleLoadConstructionCancel(const CEGUI::EventArgs&)
{
	// Ferme la fenètre
	sys->getGUISheet()->removeChildWindow("FileSelector");
	winMgr->getWindow("FileSelector")->destroy();
	return true;
}

































































