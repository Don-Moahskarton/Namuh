#include "Namuh_Main_H.h"
using namespace Ogre;

bool Gui_sandbox_manager::HandleSaveConstructionAsOk(const CEGUI::EventArgs&)
{
	if (Constructeur.getConstruction_num() != -1)
	{
		// Sécurité sur le nom de fichier
		string Filename =  winMgr->getWindow("SaveAsFrameWin/EditBox")->getText().c_str();
		if(Filename == "")
			Filename = "Untitled" + StringConverter::toString(sys0.timer.getMicroseconds());

		// SAvegarde le construction
		CstrMgr.SaveAs(Constructeur.BuiltCstr[Constructeur.getConstruction_num()],Filename);

		// Ferme la fenètre
		sys->getGUISheet()->removeChildWindow("SaveAsFrameWin");
		winMgr->getWindow("SaveAsFrameWin")->destroy();

		lastFileNameSaved = Filename;
		winMgr->getWindow("Sandbox_interface/Menubar/FileButton/FileMenu/Save")->enable();
	}
	else
		CreateMsgBox(BttnOk,"There is nothing to save.");
	return true;
}



bool Gui_sandbox_manager::HandleSaveConstructionAsCancel(const CEGUI::EventArgs&)
{
	// Ferme la fenètre
	sys->getGUISheet()->removeChildWindow("SaveAsFrameWin");
	winMgr->getWindow("SaveAsFrameWin")->destroy();
	return true;
}

































































