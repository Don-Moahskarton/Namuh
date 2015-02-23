#include "Namuh_Main_H.h"
using namespace Ogre;

/* 
=====================================================================================================================================================================================================================================================================================
============================================================================ Inscription des  Mains handlers ======================================================================================================================================================
===========================================================================================================================================================================================================================================================================
*/
void Gui_sandbox_manager::Suscribe(void)
{
	CEGUI::MenuItem* MenuItem = static_cast<CEGUI::MenuItem*>(winMgr->getWindow("Sandbox_interface/Menubar/FileButton/FileMenu/Load"));
	MenuItem->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleLoadConstruction, this));
	MenuItem = static_cast<CEGUI::MenuItem*>(winMgr->getWindow("Sandbox_interface/Menubar/FileButton/FileMenu/Save"));
	MenuItem->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSaveConstruction, this));
	MenuItem = static_cast<CEGUI::MenuItem*>(winMgr->getWindow("Sandbox_interface/Menubar/FileButton/FileMenu/SaveAs"));
	MenuItem->subscribeEvent(CEGUI::MenuItem::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSaveConstructionAs, this));


	CEGUI::Checkbox* checkBox = static_cast<CEGUI::Checkbox*>(winMgr->getWindow("Sandbox_interface/System_info/Camera_volante"));
	checkBox->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&Gui_sandbox_manager::Handlecameravolanteswitch, this));
	checkBox = static_cast<CEGUI::Checkbox*>(winMgr->getWindow("Sandbox_interface/Sandbox/Hide/CheckWing"));
	checkBox->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSandboxHideWing, this));
	checkBox = static_cast<CEGUI::Checkbox*>(winMgr->getWindow("Sandbox_interface/Sandbox/Hide/CheckBounce"));
	checkBox->subscribeEvent(CEGUI::Checkbox::EventCheckStateChanged, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSandboxHideBounce, this));

	CEGUI::PushButton* Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("Sandbox_interface/Sandbox/Log/Bttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSandboxRondin, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("Sandbox_interface/Sandbox/Rocher/Bttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSandboxRocher, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("Sandbox_interface/Sandbox/Disk/Bttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSandboxDisk, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("Sandbox_interface/Sandbox/Triangle/Bttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSandboxTriangle, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("Sandbox_interface/Sandbox/Hide/Bttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSandboxHide, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("Sandbox_interface/Sandbox/DelObjBttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleDelObj, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("Sandbox_interface/Sandbox/NewLnkBttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleNewLnk, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("Sandbox_interface/Sandbox/DelLnkBttn"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleDelLnk, this));

	
	CEGUI::Listbox* cstrList = static_cast<CEGUI::Listbox*>(winMgr->getWindow("Sandbox_interface/Sandbox/Liste_constructions"));
	cstrList->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSwitchConstruction, this));
};
/*
=====================================================================================================================================================================================================================================================================================
===================================================================== Définition des handlers ===============================================================================================================================================================
===========================================================================================================================================================================================================================================================================
*/
bool Gui_sandbox_manager::Handlecameravolanteswitch(const CEGUI::EventArgs &e)
{
	cameravolanteswitch = !cameravolanteswitch;
	return true;
};

/*
 ============================================ Handlers du Menu File ====================================================
*/
bool Gui_sandbox_manager::HandleLoadConstruction(const CEGUI::EventArgs &e)
{

	//Charge le sélecteur de fichiers
	sys->getGUISheet()->addChildWindow(CEGUI::WindowManager::getSingleton().loadWindowLayout("FileSelector.layout"));
	
	// Liste les fichiers présents
	CEGUI::Listbox* FileList = static_cast<CEGUI::Listbox*>(winMgr->getWindow("FileSelector/Filelist"));
	WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(&string("Players Profiles Datas\\"+ProfileMgr.getCurrent()->Name+"\\Constructions\\*.nmh")[0], &findFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		CEGUI::ListboxTextItem* FileItem = NULL;
		do
		{
			FileItem = new CEGUI::ListboxTextItem(string(findFileData.cFileName));
			FileItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
			FileList->addItem(FileItem);
		} 
		while(FindNextFile(hFind, &findFileData));
		
		FindClose(hFind);
	}
	else
		ldbg.send("(SandboxLoadFile Exeption) No file founded");


	// Suscribe new UI elements
	CEGUI::FrameWindow* SaveAsWin = static_cast<CEGUI::FrameWindow*>(winMgr->getWindow("FileSelector"));
	SaveAsWin->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleLoadConstructionCancel, this));

	CEGUI::PushButton* Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("FileSelector/CancelButton"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleLoadConstructionCancel, this));
	Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("FileSelector/OkButton"));
	Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleLoadConstructionOk, this));
	
	return true;
}
bool Gui_sandbox_manager::HandleSaveConstruction(const CEGUI::EventArgs &e)
{

	// Sécurité sur le nom de fichier
	if(lastFileNameSaved == "")
	{
		ldbg.send("(SandBoxSaveFile Exeption) Fired Save Construction with blank file name, using a rendom one.");
		lastFileNameSaved = "Untitled" + StringConverter::toString(sys0.timer.getMicroseconds());
	}

	// Sauvegarde le construction
	CstrMgr.SaveAs(Constructeur.BuiltCstr[Constructeur.getConstruction_num()],lastFileNameSaved);

	return true;
}


bool Gui_sandbox_manager::HandleSaveConstructionAs(const CEGUI::EventArgs &e)
{
	if (Constructeur.getConstruction_num() != -1)
	{
		sys->getGUISheet()->addChildWindow(CEGUI::WindowManager::getSingleton().loadWindowLayout("SaveAS.layout"));
		winMgr->getWindow("SaveAsFrameWin/EditBox")->setText(Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->name);

		// Suscribe new UI elements
		CEGUI::FrameWindow* SaveAsWin = static_cast<CEGUI::FrameWindow*>(winMgr->getWindow("SaveAsFrameWin"));
		SaveAsWin->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSaveConstructionAsCancel, this));

		CEGUI::PushButton* Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("SaveAsFrameWin/CancelButton"));
		Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSaveConstructionAsCancel, this));
		Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("SaveAsFrameWin/OkButton"));
		Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleSaveConstructionAsOk, this));
	}
	else
		CreateMsgBox(BttnOk,"There is nothing to save.");
	
	return true;
}


/*
 ============================================ Handlers des curseurs ====================================================
*/


	// ==============  Log cursors :

bool Gui_sandbox_manager::HandleSandboxRondinOrientation(const CEGUI::EventArgs &e)
{	
	CEGUI::WindowManager * winMgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Scrollbar* scroller = static_cast<CEGUI::Scrollbar*>(winMgr->getWindow("Sandbox_interface/Sandbox/Log/Orientation/Scroller"));
	Constructeur.orientation_rondin = scroller->getScrollPosition();
	winMgr->getWindow("Sandbox_interface/Sandbox/Log/Orientation/Label")->setText("Orientation : " + StringConverter::toString(Constructeur.orientation_rondin));
	return true;
};

bool Gui_sandbox_manager::HandleSandboxRondinLength(const CEGUI::EventArgs &e)
{	
	CEGUI::WindowManager * winMgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Scrollbar* scroller = static_cast<CEGUI::Scrollbar*>(winMgr->getWindow("Sandbox_interface/Sandbox/Log/Length/Scroller"));
	Constructeur.longeur_rondin = scroller->getScrollPosition()+5; // la longueur s'ajuste de 5 a 50m
	winMgr->getWindow("Sandbox_interface/Sandbox/Log/Length/Label")->setText("Length : " + StringConverter::toString(Constructeur.longeur_rondin)+"m");
	return true;
};


bool Gui_sandbox_manager::HandleSandboxRondinRadius(const CEGUI::EventArgs &e)
{	
	CEGUI::WindowManager * winMgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Scrollbar* scroller = static_cast<CEGUI::Scrollbar*>(winMgr->getWindow("Sandbox_interface/Sandbox/Log/Radius/Scroller"));
	Constructeur.diametre_rondin = scroller->getScrollPosition();
	winMgr->getWindow("Sandbox_interface/Sandbox/Log/Radius/Label")->setText("Radius : " + StringConverter::toString(Constructeur.diametre_rondin));
	return true;
};

	// ==============  Disk cursors :

bool Gui_sandbox_manager::HandleSandboxDiskOrientation(const CEGUI::EventArgs &e)
{	
	CEGUI::WindowManager * winMgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Scrollbar* scroller = static_cast<CEGUI::Scrollbar*>(winMgr->getWindow("Sandbox_interface/Sandbox/Disk/Orientation/Scroller"));
	Constructeur.diskOrientation = scroller->getScrollPosition();
	winMgr->getWindow("Sandbox_interface/Sandbox/Disk/Orientation/Label")->setText("Orientation : " + StringConverter::toString(Constructeur.diskOrientation));
	return true;
};

bool Gui_sandbox_manager::HandleSandboxDiskThichness(const CEGUI::EventArgs &e)
{	
	CEGUI::WindowManager * winMgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Scrollbar* scroller = static_cast<CEGUI::Scrollbar*>(winMgr->getWindow("Sandbox_interface/Sandbox/Disk/Thickness/Scroller"));
	Constructeur.diskLength = scroller->getScrollPosition(); 
	winMgr->getWindow("Sandbox_interface/Sandbox/Disk/Thickness/Label")->setText("Thickness : " + StringConverter::toString(Constructeur.diskLength)+"m");
	return true;
};


bool Gui_sandbox_manager::HandleSandboxDiskRadius(const CEGUI::EventArgs &e)
{	
	CEGUI::WindowManager * winMgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Scrollbar* scroller = static_cast<CEGUI::Scrollbar*>(winMgr->getWindow("Sandbox_interface/Sandbox/Disk/Radius/Scroller"));
	Constructeur.diskRadius = scroller->getScrollPosition();
	winMgr->getWindow("Sandbox_interface/Sandbox/Disk/Radius/Label")->setText("Radius : " + StringConverter::toString(Constructeur.diskRadius));
	return true;
};


	// ==============  Triangle cursors :

bool Gui_sandbox_manager::HandleSandboxTriangleOrientation(const CEGUI::EventArgs &e)
{	
	CEGUI::WindowManager * winMgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Scrollbar* scroller = static_cast<CEGUI::Scrollbar*>(winMgr->getWindow("Sandbox_interface/Sandbox/Triangle/Orientation/Scroller"));
	Constructeur.triangleOrientation = scroller->getScrollPosition();
	winMgr->getWindow("Sandbox_interface/Sandbox/Triangle/Orientation/Label")->setText("Orientation : " + StringConverter::toString(Constructeur.triangleOrientation));
	return true;
};

bool Gui_sandbox_manager::HandleSandboxTriangleDirection(const CEGUI::EventArgs &e)
{	
	CEGUI::WindowManager * winMgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Scrollbar* scroller = static_cast<CEGUI::Scrollbar*>(winMgr->getWindow("Sandbox_interface/Sandbox/Triangle/Direction/Scroller"));
	Constructeur.triangleDirection = scroller->getScrollPosition();
	winMgr->getWindow("Sandbox_interface/Sandbox/Triangle/Direction/Label")->setText("Direction : " + StringConverter::toString(Constructeur.triangleDirection));
	return true;
};

bool Gui_sandbox_manager::HandleSandboxTriangleLength(const CEGUI::EventArgs &e)
{	
	CEGUI::WindowManager * winMgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Scrollbar* scroller = static_cast<CEGUI::Scrollbar*>(winMgr->getWindow("Sandbox_interface/Sandbox/Triangle/Length/Scroller"));
	Constructeur.triangleLength = scroller->getScrollPosition()+0.001;
	winMgr->getWindow("Sandbox_interface/Sandbox/Triangle/Length/Label")->setText("Length : " + StringConverter::toString(Constructeur.triangleLength)+"m");
	return true;
};


bool Gui_sandbox_manager::HandleSandboxTriangleRadius(const CEGUI::EventArgs &e)
{	
	CEGUI::WindowManager * winMgr = CEGUI::WindowManager::getSingletonPtr();
	CEGUI::Scrollbar* scroller = static_cast<CEGUI::Scrollbar*>(winMgr->getWindow("Sandbox_interface/Sandbox/Triangle/Radius/Scroller"));
	Constructeur.triangleRadius = scroller->getScrollPosition();
	winMgr->getWindow("Sandbox_interface/Sandbox/Triangle/Radius/Label")->setText("Radius : " + StringConverter::toString(Constructeur.triangleRadius));
	return true;
};

/*
 ============================================ Handlers des Outils ====================================================
*/

bool Gui_sandbox_manager::HandleSandboxRondin(const CEGUI::EventArgs &e)
{
	if(Constructeur.getConstructionMode() == RONDIN)
		Constructeur.setConstructionMode(NONE);
	else
	{
		if(Constructeur.getConstructionMode()  == NONE)
			if(!Constructeur.BuiltCstr.size())
				Constructeur.NewConstruction();
			else
			{
				if(Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.size())// Ne crée pas de nouvelle construction si la précédente est vide
					Constructeur.NewConstruction();
				else
					CreateMsgBox(BttnOk,"Current construction empty, will not create a new one");
			}
		Constructeur.setConstructionMode(RONDIN);

	}
	return true;
};

bool Gui_sandbox_manager::HandleSandboxRocher(const CEGUI::EventArgs &e)
{
	if(Constructeur.getConstructionMode()  == ROCHER)
		Constructeur.setConstructionMode(NONE);
	else
	{
		if(Constructeur.getConstructionMode()  == NONE)
			if(!Constructeur.BuiltCstr.size())
				Constructeur.NewConstruction();
			else
			{
				if(Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.size())// Ne crée pas de nouvelle construction si la précédente est vide
					Constructeur.NewConstruction();
				else
					CreateMsgBox(BttnOk,"Current construction empty, will not create a new one");
			}

		Constructeur.setConstructionMode(ROCHER);
	}
	return true;
};


bool Gui_sandbox_manager::HandleSandboxDisk(const CEGUI::EventArgs &e)
{
	if(Constructeur.getConstructionMode() == DISK)
		Constructeur.setConstructionMode(NONE);
	else
	{
		if(Constructeur.getConstructionMode()  == NONE)
			if(!Constructeur.BuiltCstr.size())
				Constructeur.NewConstruction();
			else
			{
				if(Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.size())// Ne crée pas de nouvelle construction si la précédente est vide
					Constructeur.NewConstruction();
				else
					CreateMsgBox(BttnOk,"Current construction empty, will not create a new one");
			}
		Constructeur.setConstructionMode(DISK);
	}
	return true;
};


bool Gui_sandbox_manager::HandleSandboxTriangle(const CEGUI::EventArgs &e)
{
	if(Constructeur.getConstructionMode() == TRIANGLE)
		Constructeur.setConstructionMode(NONE);
	else
	{
		if(Constructeur.getConstructionMode()  == NONE)
			if(!Constructeur.BuiltCstr.size())
				Constructeur.NewConstruction();
			else
			{
				if(Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.size())// Ne crée pas de nouvelle construction si la précédente est vide
					Constructeur.NewConstruction();
				else
					CreateMsgBox(BttnOk,"Current construction empty, will not create a new one");
			}
		Constructeur.setConstructionMode(TRIANGLE);
	}
	return true;
};


bool Gui_sandbox_manager::HandleSandboxHide(const CEGUI::EventArgs &e)
{
	if(Constructeur.getConstructionMode() == ANIMAL_HIDE)
		Constructeur.setConstructionMode(NONE);
	else
	{
		if(Constructeur.getConstructionMode()  == NONE)
			if(!Constructeur.BuiltCstr.size())
				Constructeur.NewConstruction();
			else
			{
				if(Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.size())// Ne crée pas de nouvelle construction si la précédente est vide
					Constructeur.NewConstruction();
				else
					CreateMsgBox(BttnOk,"Current construction empty, will not create a new one");
			}
		Constructeur.setConstructionMode(ANIMAL_HIDE);
		Constructeur.sethideCornerNum(1);
	}
	return true;
};

bool Gui_sandbox_manager::HandleSandboxHideWing(const CEGUI::EventArgs &e)
{
	isHideWing = !isHideWing;
	return true;
};

bool Gui_sandbox_manager::HandleSandboxHideBounce(const CEGUI::EventArgs &e)
{
	isHideBounce = !isHideBounce;
	return true;
};

bool Gui_sandbox_manager::HandleSwitchConstruction(const CEGUI::EventArgs &e)
{
	CEGUI::Listbox* Cstrlist = static_cast<CEGUI::Listbox*>(winMgr->getWindow("Sandbox_interface/Sandbox/Liste_constructions"));

	if(Cstrlist->getFirstSelectedItem() != NULL) // S'il y a bien une construction sélectionnée
	{
		string requestedCstr = Cstrlist->getFirstSelectedItem()->getText().c_str();
		for (Ogre::uint i = 0; i<Constructeur.BuiltCstr.size(); ++i)
			if(Constructeur.BuiltCstr[i]->name == requestedCstr)
				Constructeur.setConstruction_num(i);
	}
	else
	{
	//	ldbg.send("(SandBox HandleSwitchConstruction Exeption) Unable to swich construction : no construction selected ! Swiching to last made one.");
//		Constructeur.setConstruction_num(Construction.size()-1);
		Constructeur.setConstruction_num(Constructeur.getConstruction_num());
	}

	return true;
};

bool Gui_sandbox_manager::HandleDelObj(const CEGUI::EventArgs &e)
{
	if(Constructeur.getConstructionMode()  != DELOBJ)
		Constructeur.setConstructionMode(DELOBJ);
	else
		Constructeur.setConstructionMode(NONE);

	return true;
};


bool Gui_sandbox_manager::HandleNewLnk(const CEGUI::EventArgs &e)
{
	if(!GuiSandboxMan.winMgr->isWindowPresent("SanboxLinkCreator"))
	{
		//Charge le sélecteur de fichiers
		sys->getGUISheet()->addChildWindow(CEGUI::WindowManager::getSingleton().loadWindowLayout("SandboxLnkCreator.layout"));
		if(Constructeur.BuiltCstr.size() && Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.size())
		{
			// Remplit la première liste d'objets
			CEGUI::Listbox* ObjLst = static_cast<CEGUI::Listbox*>(winMgr->getWindow("SanboxLinkCreator/FirstObjLst"));
			CEGUI::ListboxTextItem* FileItem = NULL;
			for (	std::vector<ConstructionElement*>::iterator itt = Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.begin();
					itt != Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.end();
					++itt)
				if ((*itt)->isRigidBodyBased())

				{
					FileItem = new CEGUI::ListboxTextItem(static_cast<ConstructionRigidElement*>(*itt)->physXBody->getName());
					FileItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
					ObjLst->addItem(FileItem);
				}

			// Remplit la seconde liste d'objets
			ObjLst = static_cast<CEGUI::Listbox*>(winMgr->getWindow("SanboxLinkCreator/SecondObjLst"));
			FileItem = NULL;
			for (	std::vector<ConstructionElement*>::iterator itt = Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.begin();
					itt != Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.end();
					++itt)
				if ((*itt)->isRigidBodyBased())
				{
					FileItem = new CEGUI::ListboxTextItem(static_cast<ConstructionRigidElement*>(*itt)->physXBody->getName());
					FileItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
					ObjLst->addItem(FileItem);
				}

		}

		// Suscribe new UI elements
		CEGUI::FrameWindow* SaveAsWin = static_cast<CEGUI::FrameWindow*>(winMgr->getWindow("SanboxLinkCreator"));
		SaveAsWin->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleNewLnkCancel, this));

		CEGUI::PushButton* Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("SanboxLinkCreator/CancelBttn"));
		Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleNewLnkCancel, this));
		Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("SanboxLinkCreator/OkBttn"));
		Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleNewLnkOk, this));
		Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("SanboxLinkCreator/PickFrstObjBttn"));
		Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleNewLnkPickFrstObjBttn, this));
		Boutton = static_cast<CEGUI::PushButton*>(winMgr->getWindow("SanboxLinkCreator/PickSecondObjBttn"));
		Boutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Gui_sandbox_manager::HandleNewLnkPickSecondObjBttn, this));
	}
	else
		CreateMsgBox(BttnOk,"Link creator is already open");

	return true;
};

bool Gui_sandbox_manager::HandleDelLnk(const CEGUI::EventArgs &e)
{
	CEGUI::Listbox* LnkLst = static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("Sandbox_interface/Sandbox/Liste_liaisons"));
	string lnkName = "";
	if(LnkLst->getFirstSelectedItem() != NULL) // S'il y a bien un 1er obj sélectionnée
		lnkName = LnkLst->getFirstSelectedItem()->getText().c_str();
	else
	{
		ldbg.send("(SandBoxHandleDelLnk Exeption) Unable to delete link : no one selected");
		CreateMsgBox(BttnOk,"You must select a link before deleting it.");
		return true;
	}

	for (Ogre::uint i = 0; i<Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->joint.size(); ++i)
		if(Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->joint[i].name == lnkName)
		{
			Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->DestroyJoint(Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->joint[i].joint); // Don't seems to work. Can't explain why...
			Constructeur.refreshLinkLst();
		} 
	

	return true;
};




























































