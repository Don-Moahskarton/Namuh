#include "Namuh_Main_H.h"
using namespace Ogre;



bool Gui_sandbox_manager::HandleNewLnkOk(const CEGUI::EventArgs&)
{
	
	CEGUI::Listbox* Objlist = static_cast<CEGUI::Listbox*>(winMgr->getWindow("SanboxLinkCreator/FirstObjLst"));
	string objName = "";
	NxOgre::RigidBody	*FirstObj = NULL,
						*SecondObj = NULL;
	// Récupère le premier objet sélectionné
	if(Objlist->getFirstSelectedItem() != NULL) // S'il y a bien un 1er obj sélectionnée
		objName = Objlist->getFirstSelectedItem()->getText().c_str();
	for (	std::vector<ConstructionElement*>::iterator itt = Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.begin();
			itt != Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.end();
			++itt )
			if((*itt)->isRigidBodyBased())
				if(static_cast<ConstructionRigidElement*>(*itt)->physXBody->getName() == objName)
					FirstObj = static_cast<ConstructionRigidElement*>(*itt)->physXBody;


	// Récupère le second objet séléctionné
	Objlist = static_cast<CEGUI::Listbox*>(winMgr->getWindow("SanboxLinkCreator/SecondObjLst"));
	if(Objlist->getFirstSelectedItem() != NULL) // S'il y a bien un 1er obj sélectionnée
		objName = Objlist->getFirstSelectedItem()->getText().c_str();
	for (	std::vector<ConstructionElement*>::iterator itt = Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.begin();
			itt != Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->element.end();
			++itt)
			if((*itt)->isRigidBodyBased())
				if(static_cast<ConstructionRigidElement*>(*itt)->physXBody->getName() == objName)
					SecondObj = static_cast<ConstructionRigidElement*>(*itt)->physXBody;

	if(SecondObj == FirstObj)
	{
		ldbg.send("(SanboxHandleNewLnkOk Exeption) Cannot create link : selected twice the same object");
		CreateMsgBox(BttnOk,"Cannot create link : You selected twice the same object !");
		return true;
	}
	else
	{
		// Fais le lien
		Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->CreateFixedLink(	static_cast<ConstructionRigidElement*>(Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->getElementFromBody(FirstObj)),
																						static_cast<ConstructionRigidElement*>(Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->getElementFromBody(SecondObj)));
			// test :
			//Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->CreateRevoluteLink(FirstObj, SecondObj);
				
		// Crée un élément dans la liste
		CEGUI::ListboxTextItem* LinkListItem = new CEGUI::ListboxTextItem(Constructeur.BuiltCstr[Constructeur.getConstruction_num()]->joint.back().name);
		LinkListItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
		static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("Sandbox_interface/Sandbox/Liste_liaisons"))->addItem(LinkListItem);

		// Ferme la fenètre
		sys->getGUISheet()->removeChildWindow("SanboxLinkCreator");
		winMgr->getWindow("SanboxLinkCreator")->destroy();
	}
	return true;
}


bool Gui_sandbox_manager::HandleNewLnkPickFrstObjBttn(const CEGUI::EventArgs&)
{
	winMgr->getWindow("SanboxLinkCreator")->disable();
	winMgr->getWindow("SanboxLinkCreator")->hide();

	Constructeur.setConstructionMode(CREATE_LINK_PICK_FIRST);
	return true;
}


bool Gui_sandbox_manager::HandleNewLnkPickSecondObjBttn(const CEGUI::EventArgs&)
{
	// Cache la fenètre
	winMgr->getWindow("SanboxLinkCreator")->disable();
	winMgr->getWindow("SanboxLinkCreator")->hide();


	Constructeur.setConstructionMode(CREATE_LINK_PICK_SECOND);
	return true;
}



bool Gui_sandbox_manager::HandleNewLnkCancel(const CEGUI::EventArgs&)
{
	// Ferme la fenètre
	sys->getGUISheet()->removeChildWindow("SanboxLinkCreator");
	winMgr->getWindow("SanboxLinkCreator")->destroy();
	return true;
}














































































