#include "Namuh_Main_H.h"
using namespace Ogre;

Gui_MainMenu_manager GuiMainMenuMan;

Gui_MainMenu_manager::Gui_MainMenu_manager(void)
{
	IsBackGroundInitialised = false;
}

void Gui_MainMenu_manager::Initialise(void)
{	
    this->initialise_CEGUI();

	// Construct loading screen
	this->ConstructLoadingScreen();
	sys1.update(1/60);
	this->Update(1);
	this->winMgr->destroyAllWindows();

	layout = winMgr->loadWindowLayout("Menu Principal.layout"); 
	sys->setGUISheet(layout);
	Main.create(Menuci_Type_MainMenu);
	sys0.mKeyboard->setEventCallback( this );
	ldbg.enable(); // A partir de la, le Live Debugger est opérationnel

	//TODO : retrouver ou est chargé le fichier Namuh.cfg

	// Si aucun profil n'est diponible, on en crée un et sinon, on l'applique
	if(!ProfileMgr.isCurrentProfileValid)
	{
		Current_Submenu = Submenu_Other;
		//Charge la fenètre qui demande le nom du profil
		sys->getGUISheet()->addChildWindow(CEGUI::WindowManager::getSingleton().loadWindowLayout("FirstProfileNamePrompt.layout"));
		
		CEGUI::PushButton* Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("FirstProfileNameInput/OkBttn"));
		Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandleFirstProfileNamePromtOk, this));

		CreateMsgBox(BttnOk,"TODO : when Namuh.cfg is deleted, try first to pick one of existing profiles rather than lauching the first-lauch assistant");
		CreateMsgBox(BttnOk,"Hey, I guess you're new here ! \n Before anything serious between us, you have to create yourself a profile first. Don't worry, it won't be long. \n Just do it !");
	}
	else
	{
		Data.Network.Defaut_IP = ProfileMgr.getCurrent()->Default_IP;
		Current_Submenu = Submenu_Main;
	}

	Window.EntryNameBox = GuiMainMenuMan.winMgr->createWindow("TaharezLook/StaticText","MainMenu/EntryName");
		Window.EntryNameBox->setArea(CEGUI::URect( CEGUI::UDim(0.4f,0.0f),CEGUI::UDim(0.15f,0.0f) ,CEGUI::UDim(0.6f,0),CEGUI::UDim(0.2f,0.0f) ));
		Window.EntryNameBox->setText("Menu Principal");
		//Window.EntryNameBox->setHorizontalAlignment(CEGUI::HorizontalAlignment::HA_CENTRE);
	GuiMainMenuMan.winMgr->getWindow("root")->addChildWindow(Window.EntryNameBox);

	lastEscKey = false;

	// Load background
	//this->SetupBackground();
}

MainMenuUpdaterOutput Gui_MainMenu_manager::UpdateMainMenu(void)
{

	
	// Load background
	this->UpdateBackground();
	// TODO : implement keyboard navigation
	MenuOutData.LancerPartie = false;

	
	if(sys0.kb_releasing(OIS::KC_ESCAPE) && Current_Submenu != Submenu_EscMenu)
	{
		this->ConstructEscmenu();
		lastEscKey = false;
	}

	switch (Current_Submenu)
	{
		case(Submenu_Main):
		{
			Main.isVisible = !sys0.ms_releasing(OIS::MB_Left);

			if(Main.Current_highlighted==1)Window.EntryNameBox->setText("Sandbox");
			if(Main.Current_highlighted==2)Window.EntryNameBox->setText("Partie Réseau");

			Main.update();

			if(Main.output == 1) // Goes to Sandbox map selection window
			{

				// Old things 
				/*
				MenuOutData.LancerPartie = true;
				MenuOutData.Param.Net_Status = Partie_Locale;
				MenuOutData.Param.isSandbox = true;
				
				//MenuOutData.Param.Mapfile = "Maps/Chutes Bresiliennes/Chutes Bresiliennes.map";
				MenuOutData.Param.Mapfile = "Maps/Caribean Lost Island/Caribean Lost Island.map";
				//MenuOutData.Param.Mapfile = "Maps/Flat/Flat.map";
				//MenuOutData.Param.Mapfile = "Maps/Main Menu background/Main Menu background.map";

				// Destroy background
				this->DestroyBackground();*/


				// Construct map selection window
				CEGUI::MouseCursor::getSingleton().setVisible(!CEGUI::MouseCursor::getSingleton().isVisible()); // show mouse
				Current_Submenu = Submenu_Other;	// disable other menus reactions

				sys->getGUISheet()->addChildWindow(CEGUI::WindowManager::getSingleton().loadWindowLayout("SandBoxMapSelector.layout"));	// loads window layout
				
				// connect the loaded layout to the c++ code
				CEGUI::Listbox* mapList = static_cast<CEGUI::Listbox*>(winMgr->getWindow("SandboxMapSelector/MapLst"));
				mapList->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandleSandboxMapSelectorMapLst, this));	
				CEGUI::PushButton* Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("SandboxMapSelector/CancelBttn"));
				Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandleSandboxMapSelectorCancelBttn, this));
				Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("SandboxMapSelector/LaunchBttn"));
				Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandleSandboxMapSelectorLaunchBttn, this));


				// loads maps into list   --    Create list element
				CEGUI::ListboxTextItem* LinkListItem;
				LinkListItem = new CEGUI::ListboxTextItem("Chutes Bresiliennes");
				LinkListItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
				mapList->addItem(LinkListItem);
				LinkListItem = new CEGUI::ListboxTextItem("Caribean Lost Island");
				LinkListItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
				mapList->addItem(LinkListItem);
				LinkListItem = new CEGUI::ListboxTextItem("Flat");
				LinkListItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
				mapList->addItem(LinkListItem);
				mapList->getListboxItemFromIndex(1)->setSelected(true);	// select caribean island

				// fills in description for caribean map
				winMgr->getWindow("SandboxMapSelector/MapName")->setText("Caribbean Lost Island");
				winMgr->getWindow("SandboxMapSelector/HeaderWater")->setText("Has water : yes");
				winMgr->getWindow("SandboxMapSelector/HeaderSize")->setText("Map Size : 450m x 450m");
				winMgr->getWindow("SandboxMapSelector/DescriptionText")->setText("Small island, far from others in the Atlantic \n ocean. Very sunny, with large beaches and many cliffs");
				winMgr->getWindow("SandboxMapSelector/WildLifeText")->setText("The waters are very fishy arround there. TODO :\n birds and no mamoths");
				winMgr->getWindow("SandboxMapSelector/PlantLifeText")->setText("Some palm trees, no grass, mostly sand and\n rock.");


				return MenuOutData;

			}
			if(Main.output == 2)	// Network Submenu
			{
				Main.output = -1; // pour ne pas retourner directement dans le sous menu d'ou l'on vient
				Current_Submenu = Submenu_Network;
				ConstructSubmenu(Submenu_Network);

				Main.isVisible = true;
				Main.update(); // garder le main a l'écran
			}
		}
		break;

		case(Submenu_Network):
		{
			Network.isVisible = !sys0.ms_releasing(OIS::MB_Left);

			if(Network.Current_highlighted==1)Window.EntryNameBox->setText("IP a connecter");
			if(Network.Current_highlighted==2)Window.EntryNameBox->setText("Retour");
			if(Network.Current_highlighted==3)Window.EntryNameBox->setText("Créer un serveur");
			if(Network.Current_highlighted==4)Window.EntryNameBox->setText("Connexion au serveur");

			Network.update();

			if(Network.output == 4)
			{
				MenuOutData.LancerPartie = true;
				MenuOutData.Param.Net_Status = Partie_Client;
				MenuOutData.Param.isSandbox = false;
				MenuOutData.Param.IPofSrv = Window.Network.IPEditBox->getText().c_str();
				MenuOutData.Param.Mapfile = "Maps/Chutes Bresiliennes/Chutes Bresiliennes.map";
				//MenuOutData.Param.Mapfile = "Maps/Caribean Lost Island/Caribean Lost Island.map";

				
				// Destroy background
				this->DestroyBackground();

			}
			if(Network.output == 3)
			{
				MenuOutData.LancerPartie = true;
				MenuOutData.Param.Net_Status = Partie_Server;
				MenuOutData.Param.isSandbox = false;
				//MenuOutData.Param.Mapfile = "Maps/Chutes Bresiliennes/Chutes Bresiliennes.map";
				MenuOutData.Param.Mapfile = "Maps/Caribean Lost Island/Caribean Lost Island.map";
		
				// Destroy background
				this->DestroyBackground();

			}
			if(Network.output != -1 && Network.output != 1)
			{
				ofstream ocfg_file ("Namuh.conf",ios::out|ios::trunc);
				ocfg_file << Window.Network.IPEditBox->getText();
				Data.Network.Defaut_IP = Window.Network.IPEditBox->getText().c_str();
			}
			if(Network.output == 2)
			{
				DestructSubmenu(Submenu_Network);
				Current_Submenu = Submenu_Main;
				ConstructSubmenu(Submenu_Main);
			}
			if(Network.output == 1)
			{
				if(!Window.Network.IPEditBox->isDisabled())
				{
					Window.Network.IPEditBox->deactivate();
					Window.Network.IPEditBox->disable();
				}
				else
				{
					Window.Network.IPEditBox->enable();
					Window.Network.IPEditBox->activate();
				}
			}
			if(!Window.Network.IPEditBox->isDisabled() && (sys0.mKeyboard->isKeyDown(OIS::KC_RETURN) || sys0.mKeyboard->isKeyDown(OIS::KC_NUMPADENTER))) // Sortie de la config d'ip via touche entrée
			{
				Window.Network.IPEditBox->deactivate();
				Window.Network.IPEditBox->disable();
			}

		}
		break;
		case(Submenu_EscMenu):
		{
			EscMenu.isVisible = !sys0.ms_releasing(OIS::MB_Left) && !lastEscKey;
			EscMenu.update();

			if(EscMenu.Current_highlighted==1)GuiMainMenuMan.winMgr->getWindow("EscMenuWin/Infotext")->setText("Unused");
			if(EscMenu.Current_highlighted==2)GuiMainMenuMan.winMgr->getWindow("EscMenuWin/Infotext")->setText("Profil joueur");
			if(EscMenu.Current_highlighted==3)GuiMainMenuMan.winMgr->getWindow("EscMenuWin/Infotext")->setText("Quitter Namuh");
			if(EscMenu.Current_highlighted==4)GuiMainMenuMan.winMgr->getWindow("EscMenuWin/Infotext")->setText("Options");


			// Profil du joueur
			if(EscMenu.output == 2)
			{
				ConstructPlayerProfilemenu();
				Current_Submenu = Submenu_PlayerProfile;
			}

			// Quitte le jeu
			if(EscMenu.output == 3)
			{
				ldbg.send("(MainMenuEscMenu info) recieved quit command, sending MusQuit signal.");
				sys1.MustQuit = true;
				
				// Destroy background
				this->DestroyBackground();

			}

			// Quitte le menu
			if(!sys0.mKeyboard->isKeyDown(OIS::KC_ESCAPE) && lastEscKey)
			{
				lastEscKey = false;
				this->DestructEscmenu();
			}
		}
		break;
	}

	lastEscKey = sys0.mKeyboard->isKeyDown(OIS::KC_ESCAPE);
	
	sys1.update(1/60);
	Update(1/sys1.FPS);
	return MenuOutData;
}



void Gui_MainMenu_manager::ConstructSubmenu(Submenu type)
{
	switch (Current_Submenu)
	{
		case(Submenu_Main):
		{
			CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(sys0.vp->getActualWidth()/2,sys0.vp->getActualHeight()/2));
		}
		break;
		case(Submenu_Network):
		{
			Network.create(Menuci_Type_Four,(sys0.vp->getActualWidth()/2)-sys0.vp->getActualHeight()*0.2,(sys0.vp->getActualHeight()/2));

			Window.Network.IPEditBox = GuiMainMenuMan.winMgr->createWindow("TaharezLook/Editbox","Submenu_Network/IPEditbox");
				Window.Network.IPEditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0,250),CEGUI::UDim(0,30)));
				Window.Network.IPEditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0,(sys0.vp->getActualWidth()/2)-sys0.vp->getActualHeight()*0.2-125),CEGUI::UDim(0,(sys0.vp->getActualHeight()/2)-sys0.vp->getActualHeight()*0.10-30)));
				Window.Network.IPEditBox->setInheritsAlpha(false);
				Window.Network.IPEditBox->setAlpha(1.0f); 
				Window.Network.IPEditBox->setText(Data.Network.Defaut_IP);
				Window.Network.IPEditBox->deactivate();
				Window.Network.IPEditBox->disable();
			GuiMainMenuMan.winMgr->getWindow("root")->addChildWindow(Window.Network.IPEditBox);
		}
	}
};

void Gui_MainMenu_manager::DestructSubmenu(Submenu type)
{
	switch (Current_Submenu)
	{
		case(Submenu_Network):
		{
			//Network.destroy ?
			CEGUI::WindowManager::getSingleton().destroyWindow(Window.Network.IPEditBox);
		}
	}
}

void Gui_MainMenu_manager::ConstructEscmenu(void)
{
	if(!CEGUI::WindowManager::getSingleton().isWindowPresent("EscMenuWin"))
	{
		// TODO : Makes the circular menu a child of the window
		EscMenu_Previous_Submenu = Current_Submenu;
		Current_Submenu = Submenu_EscMenu;

		sys->getGUISheet()->addChildWindow(CEGUI::WindowManager::getSingleton().loadWindowLayout("MainMenuEscMenu.layout"));
		EscMenu.create(	Menuci_Type_Four,
						sys0.vp->getActualWidth()/2,
						sys0.vp->getActualHeight()/2);
		EscMenu.setAllwaysOnTop(true);
					//	GuiMainMenuMan.winMgr->getWindow("EscMenuWin")->getArea().getWidth().asAbsolute(sys0.vp->getActualWidth())/2,
					//	GuiMainMenuMan.winMgr->getWindow("EscMenuWin")->getArea().getHeight().asAbsolute(sys0.vp->getActualHeight())/2);
						//"EscMenuWin");
	}
	else
		ldbg.send("(ConstructEscmenu Exeption) Called ConstructEscmenu while the EscMenu is already present. Command ignored.");
}


bool Gui_MainMenu_manager::DestructEscmenu(const CEGUI::EventArgs&)
{
	//EscMenu.destroy ?
	Current_Submenu = EscMenu_Previous_Submenu;

	// Ferme la fenètre
	sys->getGUISheet()->removeChildWindow("EscMenuWin");
	winMgr->destroyWindow("EscMenuWin");

	return true;
}


void Gui_MainMenu_manager::ConstructPlayerProfilemenu(void)
{
	CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(sys0.vp->getActualWidth()/2,sys0.vp->getActualHeight()/2));

	//Charge la layout
	sys->getGUISheet()->addChildWindow(CEGUI::WindowManager::getSingleton().loadWindowLayout("PlayerProfileMenu.layout"));
	
	// Suscribe new UI elements
	CEGUI::FrameWindow* SaveAsWin = static_cast<CEGUI::FrameWindow*>(winMgr->getWindow("PlayerProfileWin"));
	SaveAsWin->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandlePlayerProfileClose, this));

	CEGUI::PushButton* Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileWin/CancelBttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandlePlayerProfileClose, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("PlayerProfileWin/EditConstructionsFavorites"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_MainMenu_manager::HandlePlayerProfileEditFavoriteConstruction, this));
}
   
void Gui_MainMenu_manager::ConstructLoadingScreen(void) // a mettre dans ConstructSubmenu ?
{
	CEGUI::Window* Rootwindow;
	if(winMgr->isWindowPresent("root")) // Root n'existe pas forcément
		Rootwindow = GuiMainMenuMan.winMgr->getWindow("root");
	else
	{
		Rootwindow = GuiMainMenuMan.winMgr->createWindow( "DefaultWindow", "root" );
		CEGUI::System::getSingleton().setGUISheet(Rootwindow);
	}

	CEGUI::Window *LoadingScreen = GuiMainMenuMan.winMgr->createWindow("TaharezLook/StaticImage","MainMenu/Loading Screen");
	LoadingScreen->setProperty("Image","set:Loading_Screen image:LoadingScreen");
		Rootwindow->addChildWindow(LoadingScreen);
};


bool Gui_MainMenu_manager::HandleFirstProfileNamePromtOk(const CEGUI::EventArgs&)
{
	//Crée le profil et le définit comme le profil courant
	ProfileMgr.CreateNew(string(winMgr->getWindow("FirstProfileNameInput/ProfileName")->getText().c_str()));
	ProfileMgr.setCurrent(ProfileMgr.Load("Players Profiles Datas/"+string(winMgr->getWindow("FirstProfileNameInput/ProfileName")->getText().c_str())+"/MainFile.cfg"));

	Data.Network.Defaut_IP = ProfileMgr.getCurrent()->Default_IP;
	//Active le menu principal
	Current_Submenu = Submenu_Main;

	// Ferme la fenètre
	sys->getGUISheet()->removeChildWindow("FirstProfileNameInput");
	winMgr->getWindow("FirstProfileNameInput")->destroy();


	return true;
}






bool Gui_MainMenu_manager::HandleSandboxMapSelectorMapLst(const CEGUI::EventArgs&)
{


	CEGUI::Listbox* mapList = static_cast<CEGUI::Listbox*>(winMgr->getWindow("SandboxMapSelector/MapLst"));

	if(mapList->getFirstSelectedItem() != NULL) // If a map has been selected
	{
		// TODO : do not hardcode thoses
		string requestedMap = mapList->getFirstSelectedItem()->getText().c_str();
		if(requestedMap == "Flat")
		{
			// fills in description for flat
			winMgr->getWindow("SandboxMapSelector/MapName")->setText("Flat");
			winMgr->getWindow("SandboxMapSelector/HeaderWater")->setText("Has water : no");
			winMgr->getWindow("SandboxMapSelector/HeaderSize")->setText("Map Size : 2 Km x 2 Km");
			winMgr->getWindow("SandboxMapSelector/DescriptionText")->setText("Large Flat area. Totally horisontal, no water.");
			winMgr->getWindow("SandboxMapSelector/WildLifeText")->setText("None");
			winMgr->getWindow("SandboxMapSelector/PlantLifeText")->setText("Some grass and that's it.");
		}
		else if(requestedMap == "Caribean Lost Island")
		{
			// fills in description for caribean map
			winMgr->getWindow("SandboxMapSelector/MapName")->setText("Caribbean Lost Island");
			winMgr->getWindow("SandboxMapSelector/HeaderWater")->setText("Has water : yes");
			winMgr->getWindow("SandboxMapSelector/HeaderSize")->setText("Map Size : 450m x 450m");
			winMgr->getWindow("SandboxMapSelector/DescriptionText")->setText("Small island, far from others in the Atlantic\n ocean. Very sunny, with large beaches\n and many cliffs");
			winMgr->getWindow("SandboxMapSelector/WildLifeText")->setText("The waters are very fishy arround there. TODO :\n birds and no mamoths");
			winMgr->getWindow("SandboxMapSelector/PlantLifeText")->setText("Some palm trees, no grass, mostly sand and\n rock.");
		}
		else if(requestedMap == "Chutes Bresiliennes")
		{
			// fills in description for Chutes Bresiliennes
			winMgr->getWindow("SandboxMapSelector/MapName")->setText("Chutes Bresiliennes");
			winMgr->getWindow("SandboxMapSelector/HeaderWater")->setText("Has water : yes");
			winMgr->getWindow("SandboxMapSelector/HeaderSize")->setText("Map Size : 2Km x 2Km");
			winMgr->getWindow("SandboxMapSelector/DescriptionText")->setText("Very large map, with a dense jungle. A river\n pass trough the area, from a grand waterfall\n to ocean.");
			winMgr->getWindow("SandboxMapSelector/WildLifeText")->setText("Mammoths can be fairly easily founded, and some fish are in the river. TODO : more wild animals. And animate those mammoths");
			winMgr->getWindow("SandboxMapSelector/PlantLifeText")->setText("Dense vegetation : trees are all over the\n place, and sometimes big bushes. The grass is\n tall.");
		}
	}


	return true;
}


bool Gui_MainMenu_manager::HandleSandboxMapSelectorLaunchBttn(const CEGUI::EventArgs&)
{

	// launch Sandbox besd on selected map

	MenuOutData.LancerPartie = true;
	MenuOutData.Param.Net_Status = Partie_Locale;
	MenuOutData.Param.isSandbox = true;

	// TODO : do not hardcode thoses
	CEGUI::Listbox* mapList = static_cast<CEGUI::Listbox*>(winMgr->getWindow("SandboxMapSelector/MapLst"));
	string requestedMap = mapList->getFirstSelectedItem()->getText().c_str();
	if(requestedMap == "Flat")
		MenuOutData.Param.Mapfile = "Maps/Flat/Flat.map";
	else if(requestedMap == "Caribean Lost Island")
		MenuOutData.Param.Mapfile = "Maps/Caribean Lost Island/Caribean Lost Island.map";
	else if(requestedMap == "Chutes Bresiliennes")
		MenuOutData.Param.Mapfile = "Maps/Chutes Bresiliennes/Chutes Bresiliennes.map";
	//MenuOutData.Param.Mapfile = "Maps/Main Menu background/Main Menu background.map";

	return true;
}

bool Gui_MainMenu_manager::HandleSandboxMapSelectorCancelBttn(const CEGUI::EventArgs&)
{
	Current_Submenu =  Submenu_Main;

	// Ferme la fenètre
	sys->getGUISheet()->removeChildWindow("SandboxMapSelector");
	winMgr->getWindow("SandboxMapSelector")->destroy();

	CEGUI::MouseCursor::getSingleton().setVisible(!CEGUI::MouseCursor::getSingleton().isVisible()); // hide mouse
	return true;
}

























