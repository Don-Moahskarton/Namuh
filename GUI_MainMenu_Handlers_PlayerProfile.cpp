#include "Namuh_Main_H.h"
using namespace Ogre;


bool Gui_MainMenu_manager::HandlePlayerProfileClose(const CEGUI::EventArgs&)
{
	Current_Submenu = Submenu_EscMenu;

	// Ferme la fenètre
	sys->getGUISheet()->removeChildWindow("PlayerProfileWin");
	winMgr->destroyWindow("PlayerProfileWin");

	return true;
}


bool Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstruction(const CEGUI::EventArgs&)
{
	Current_Submenu = Submenu_PlayerProfileEditFavoritesConstructions;

	CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(sys0.vp->getActualWidth()/2,sys0.vp->getActualHeight()/2));

	//Charge la layout
	sys->getGUISheet()->addChildWindow(CEGUI::WindowManager::getSingleton().loadWindowLayout("PlayerProfileEditFavoritesConstructionsMenu.layout"));

	// Liste les constuctions favorites
	CEGUI::Listbox* FavLst = static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("PlayerProfileEditFavCstrsWin/FavoritedConstructionSelector"));
	FavLst->resetList();
	CEGUI::ListboxTextItem* FavListItem;
	for( Ogre::uint i = 0; i<ProfileMgr.getCurrent()->FavoritesConstructions.size(); ++i)
	{
		FavListItem = new CEGUI::ListboxTextItem(ProfileMgr.getCurrent()->FavoritesConstructions[i]);
		FavListItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
		FavLst->addItem(FavListItem);
	}
	
	// Liste les constuctions tout court
	CEGUI::Combobox* CstrLst = static_cast<CEGUI::Combobox*>(GuiSandboxMan.winMgr->getWindow("PlayerProfileEditFavCstrsWin/ConstructionSelector"));
	CstrLst->resetList();
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(&string("Players Profiles Datas\\"+ProfileMgr.getCurrent()->Name+"\\Constructions\\*.nmh")[0], &findFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		CEGUI::ListboxTextItem* CstrListItem = NULL;
		//FindNextFile(hFind, &findFileData);
		do
		{
			CstrListItem = new CEGUI::ListboxTextItem(findFileData.cFileName);
			CstrListItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
			CstrLst->addItem(CstrListItem);
		} 
		while(FindNextFile(hFind, &findFileData));
		
		FindClose(hFind);
	}
	else
		ldbg.send("(SandboxLoadFile Exeption) No file founded");

	
	// grise le boutton save (on ne peut sauvegarder sans chagements)
	static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileEditFavCstrsWin/SaveBttn"))->disable();

	// Suscribe new UI elements
	CEGUI::FrameWindow* SaveAsWin = static_cast<CEGUI::FrameWindow*>(winMgr->getWindow("PlayerProfileEditFavCstrsWin"));
	SaveAsWin->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionClose, this));

	CEGUI::PushButton* Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileEditFavCstrsWin/CancelBttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionClose, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileEditFavCstrsWin/AddFavBttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionAddBttn, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileEditFavCstrsWin/ReplaceFavBttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionEditBttn, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileEditFavCstrsWin/DelFavBttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionDelBttn, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileEditFavCstrsWin/SaveBttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionSave, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileEditFavCstrsWin/SaveAndCloseBttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionSaveAndClose, this));

	return true;
}

bool Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionAddBttn(const CEGUI::EventArgs&)
{

	CEGUI::Combobox* CstrLst = static_cast<CEGUI::Combobox*>(GuiSandboxMan.winMgr->getWindow("PlayerProfileEditFavCstrsWin/ConstructionSelector"));
	if(CstrLst->getSelectedItem() != NULL) // S'il y a bien une construction de sélectionnée
	{
		CEGUI::ListboxTextItem* FavListItem = new CEGUI::ListboxTextItem(CstrLst->getSelectedItem()->getText());
		FavListItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
		static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("PlayerProfileEditFavCstrsWin/FavoritedConstructionSelector"))->addItem(FavListItem);
	}
	else
		CreateMsgBox(BttnOk,"Please select a construction in the combo list before adding it.");		

	// dégrise le boutton (on peut sauvegarder les chagements)
	static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileEditFavCstrsWin/SaveBttn"))->enable();

	return true;
}

bool Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionEditBttn(const CEGUI::EventArgs&)
{
	CEGUI::Combobox* CstrLst = static_cast<CEGUI::Combobox*>(GuiSandboxMan.winMgr->getWindow("PlayerProfileEditFavCstrsWin/ConstructionSelector"));
	CEGUI::Listbox* FavLst = static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("PlayerProfileEditFavCstrsWin/FavoritedConstructionSelector"));

	if(CstrLst->getSelectedItem() != NULL) // S'il y a bien un favoris de sélectionné
	{
		if(FavLst->getFirstSelectedItem() != NULL) // S'il y a bien une construction avec laquelle interchanger de sélectionnée
			FavLst->getFirstSelectedItem()->setText(CstrLst->getSelectedItem()->getText());
		else
			CreateMsgBox(BttnOk,"Please select a favorite in the list to remplace the one you've selected.");
	}
	else
		CreateMsgBox(BttnOk,"Please select a construction in the combo list before editing it.");		

	
	// dégrise le boutton (on peut sauvegarder les chagements)
	static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileEditFavCstrsWin/SaveBttn"))->enable();

	return true;
}

bool Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionDelBttn(const CEGUI::EventArgs&)
{

	CEGUI::Listbox* FavLst = static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("PlayerProfileEditFavCstrsWin/FavoritedConstructionSelector"));
	if(FavLst->getFirstSelectedItem() != NULL) // S'il y a bien un favoris sélectionnée
	{
		FavLst->getFirstSelectedItem()->setAutoDeleted(true);
		FavLst->removeItem(FavLst->getFirstSelectedItem());
	}
	else
		CreateMsgBox(BttnOk,"Please select a construction in the combo list before deleting it.");		

	// dégrise le boutton (on peut sauvegarder les chagements)
	static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileEditFavCstrsWin/SaveBttn"))->enable();

	return true;
}

bool Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionClose(const CEGUI::EventArgs&)
{
	Current_Submenu = Submenu_PlayerProfile;

	// Ferme la fenètre
	sys->getGUISheet()->removeChildWindow("PlayerProfileEditFavCstrsWin");
	winMgr->destroyWindow("PlayerProfileEditFavCstrsWin");

	return true;
}


bool Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionSaveAndClose(const CEGUI::EventArgs&)
{
	Current_Submenu = Submenu_PlayerProfile;
	CEGUI::Listbox* FavLst = static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("PlayerProfileEditFavCstrsWin/FavoritedConstructionSelector"));

	//Réécris la liste des favoris
	ProfileMgr.getCurrent()->FavoritesConstructions.clear();
	for(Ogre::uint i = 0; i<FavLst->getItemCount(); ++i)
		ProfileMgr.getCurrent()->FavoritesConstructions.push_back(string(FavLst->getListboxItemFromIndex(i)->getText().c_str()));

	// Ferme la fenètre
	sys->getGUISheet()->removeChildWindow("PlayerProfileEditFavCstrsWin");
	winMgr->destroyWindow("PlayerProfileEditFavCstrsWin");

	return true;
}

bool Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstructionSave(const CEGUI::EventArgs&)
{
	CEGUI::Listbox* FavLst = static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("PlayerProfileEditFavCstrsWin/FavoritedConstructionSelector"));

	//Réécris la liste des favoris
	ProfileMgr.getCurrent()->FavoritesConstructions.clear();
	for(Ogre::uint i = 0; i<FavLst->getItemCount(); ++i)
		ProfileMgr.getCurrent()->FavoritesConstructions.push_back(string(FavLst->getListboxItemFromIndex(i)->getText().c_str()));

	// grise le boutton (on ne peut sauvegarder sans chagements)
	static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileEditFavCstrsWin/SaveBttn"))->disable();

	return true;
}















































































































































































































