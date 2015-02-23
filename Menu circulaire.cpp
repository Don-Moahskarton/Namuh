#include "Namuh_Main_H.h"

using namespace Ogre;

int Menuci::next_id = 0;
bool Menuci::isthereOneVisible =false;

// Code des méthodes de la classe
Menuci::Menuci(void)
{
}
Menuci::~Menuci(void)
{
}

void Menuci::create(Menuci_Type requested_type,int requested_posX,int requested_posY, string ParentWindow)
{
	id = next_id;
	isVisible = false;
	type = requested_type;
	posX = requested_posX;
	posY = requested_posY;
	output = -1;
	CEGUI::Window *Rootwindow = GuiSandboxMan.winMgr->getWindow(ParentWindow);
	ParentWin = Rootwindow;

		highlight = GuiSandboxMan.winMgr->createWindow("TaharezLook/FrameWindow","Sandbox_interface/Menu_Circulaire"+StringConverter::toString(id)+"/Highlight");
		highlight->setSize(CEGUI::UVector2(CEGUI::UDim(0,sys0.vp->getActualHeight()*0.2+10),CEGUI::UDim(0,sys0.vp->getActualHeight()*0.2+2)));
		highlight->setPosition(CEGUI::UVector2(CEGUI::UDim(0,posX-5-sys0.vp->getActualHeight()*0.1),CEGUI::UDim(0,posY-sys0.vp->getActualHeight()*0.1-1)));
		highlight->setAlpha(0.0f);		
		highlight->setProperty("TitlebarEnabled","False");
		highlight->setVisible(false);
			CEGUI::Window* highlight2 = GuiSandboxMan.winMgr->createWindow("TaharezLook/StaticImage","Sandbox_interface/Menu_Circulaire"+StringConverter::toString(id)+"/Highlight/Image");
			highlight2->setInheritsAlpha(false);
			highlight2->setAlpha(1.0f); 
			if(type == Menuci_Type_MainMenu)highlight2->setProperty("Image","set:Menu-principal-higtlight image:menu");
			if(type == Menuci_Type_Four)highlight2->setProperty("Image","set:Menu-circulaire-hightlight image:quatre");
			highlight2->setProperty("FrameEnabled","False");
			highlight2->setProperty("BackgroundEnabled","False");
			highlight2->setArea(CEGUI::URect( CEGUI::UDim(0,0),CEGUI::UDim(0,0) ,CEGUI::UDim(1,0),CEGUI::UDim(1,0) ));
		highlight->addChildWindow(highlight2);

		Menu = GuiSandboxMan.winMgr->createWindow("TaharezLook/FrameWindow","Sandbox_interface/Menu_Circulaire"+StringConverter::toString(id)+"/Frame");
		Menu->setSize(CEGUI::UVector2(CEGUI::UDim(0,sys0.vp->getActualHeight()*0.2+10),CEGUI::UDim(0,sys0.vp->getActualHeight()*0.2+2)));
		Menu->setPosition(CEGUI::UVector2(CEGUI::UDim(0,posX-5-sys0.vp->getActualHeight()*0.1),CEGUI::UDim(0,posY-sys0.vp->getActualHeight()*0.1-1)));
		Menu->setAlpha(0.0f);	
		Menu->setProperty("TitlebarEnabled","False");
		Menu->setVisible(false);
			CEGUI::Window* Menu2 = GuiSandboxMan.winMgr->createWindow("TaharezLook/StaticImage","Sandbox_interface/Menu_Circulaire"+StringConverter::toString(id)+"/Frame/Image");
			Menu2->setInheritsAlpha(false);
			Menu2->setAlpha(1.0f); 
			if(type == Menuci_Type_MainMenu)Menu2->setProperty("Image","set:Menu-principal image:menu");
			if(type == Menuci_Type_Four)Menu2->setProperty("Image","set:Menu-circulaire image:quatre");
			Menu2->setProperty("FrameEnabled","False");
			Menu2->setProperty("BackgroundEnabled","False");
			Menu2->setArea(CEGUI::URect( CEGUI::UDim(0,0),CEGUI::UDim(0,0) ,CEGUI::UDim(1,0),CEGUI::UDim(1,0) ));
		Menu->addChildWindow(Menu2);

		curseur = GuiSandboxMan.winMgr->createWindow("TaharezLook/FrameWindow","Sandbox_interface/Menu_Circulaire"+StringConverter::toString(id)+"/Pointeur/Frame");
		curseur->setMinSize(CEGUI::UVector2(CEGUI::UDim(0,10),CEGUI::UDim(0,10)));
		curseur->setSize(CEGUI::UVector2(CEGUI::UDim(0,sys0.vp->getActualHeight()*0.06+10),CEGUI::UDim(0,sys0.vp->getActualHeight()*0.06+2)));
		curseur->setPosition(CEGUI::UVector2(cegui_absdim(posX-5-sys0.vp->getActualHeight()*0.03),cegui_absdim(posY-sys0.vp->getActualHeight()*0.03-1))); // Oo
		curseur->setAlpha(0.0f);		
		curseur->setProperty("TitlebarEnabled","False");
		curseur->setVisible(false);
		CEGUI::Window* curseur2 = GuiSandboxMan.winMgr->createWindow("TaharezLook/StaticImage","Sandbox_interface/Menu_Circulaire"+StringConverter::toString(id)+"/Pointeur/Image");
			curseur2->setInheritsAlpha(false);
			curseur2->setAlpha(1.0f);
			curseur2->setProperty("Image","set:curseur-circulaire image:curseur");
			curseur2->setProperty("FrameEnabled","False");
			curseur2->setProperty("BackgroundEnabled","False");
			curseur2->setArea(CEGUI::URect( CEGUI::UDim(0,0),CEGUI::UDim(0,0) ,CEGUI::UDim(1,0),CEGUI::UDim(1,0) )); // l'offset contre le léger écralsement horisontal des images pour garder une image bien circulaire
		curseur->addChildWindow(curseur2);

	Rootwindow->addChildWindow(Menu);
	Rootwindow->addChildWindow(curseur);
	Rootwindow->addChildWindow(highlight);

	CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(	posX - ParentWin->getXPosition().asAbsolute(sys0.vp->getActualWidth()),
																	posY - ParentWin->getYPosition().asAbsolute(sys0.vp->getActualHeight())));

	next_id++;
	ldbg.send("(Menu circulaire n°"+StringConverter::toString(id)+") Created");
}

void Menuci::update()
{

	float xpol = CEGUI::MouseCursor::getSingleton().getPosition().d_x - posX - ParentWin->getXPosition().asAbsolute(sys0.vp->getActualWidth()),
		  ypol = CEGUI::MouseCursor::getSingleton().getPosition().d_y - posY - ParentWin->getYPosition().asAbsolute(sys0.vp->getActualHeight());
	float angle = -360*(atan((float)(-ypol/(xpol+sqrt((float)(ypol*ypol+xpol*xpol)))))/3.141);

	if(type == Menuci_Type_Four)
	{
		// Gestion des Hightlights
		if(-135<angle && angle<-45)Current_highlighted=1;
		if(-45<angle && angle<45)Current_highlighted=2;	
		if(45<angle && angle<135)Current_highlighted=3;
		if(-135>angle || angle > 135)Current_highlighted=4;	

		if(!Menu->isVisible() && isVisible)CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(posX,posY));
		if(xpol != 0)curseur->setRotation(CEGUI::Vector3(curseur->getRotation().d_x,curseur->getRotation().d_y,angle));
		if(-135>angle || angle > 135)highlight->setRotation(CEGUI::Vector3(0,0,-90));
		if(-135<angle && angle<-45)highlight->setRotation(CEGUI::Vector3(0,0,0));
		if(-45<angle && angle<45)highlight->setRotation(CEGUI::Vector3(0,0,90));
		if(45<angle && angle<135)highlight->setRotation(CEGUI::Vector3(0,0,180));

		// Gestion des outputs
		if(Menu->isVisible() && !isVisible)
		{
			if(-135<angle && angle<-45)		output	=	1;
			if(-45<angle && angle<45)		output	=	2;	
			if(45<angle && angle<135)		output	=	3;
			if(-135>angle || angle > 135)	output	=	4;	
		}
		else								output =	-1;

		if ( sys0.kb_pressing(OIS::KC_UP) )
		{
			Current_highlighted=1;
			highlight->setRotation(CEGUI::Vector3(0,0,0));
			output = 1;
		}
		else if ( sys0.kb_pressing(OIS::KC_DOWN) )
		{
			Current_highlighted=3;
			highlight->setRotation(CEGUI::Vector3(0,0,180));
			output = 3;
		}
		else if ( sys0.kb_pressing(OIS::KC_RIGHT) )
		{
			Current_highlighted=2;	
			highlight->setRotation(CEGUI::Vector3(0,0,90));
			output = 2;
		}
		else if ( sys0.kb_pressing(OIS::KC_LEFT) )
		{
			Current_highlighted=4;	
			highlight->setRotation(CEGUI::Vector3(0,0,-90));
			output = 4;
		}

		if (output != -1)
			ldbg.send("(Menu circulaire n°"+StringConverter::toString(id)+") Output sent : "+StringConverter::toString(output));

	}

	if(type == Menuci_Type_MainMenu)
	{
		// Gestion des Hightlights
		if(-90<angle && angle<90)Current_highlighted=1;
		if(-90>angle || angle > 90)Current_highlighted=2;

		if(!Menu->isVisible() && isVisible)CEGUI::MouseCursor::getSingleton().setPosition(CEGUI::Point(posX,posY)); // positionne le curseur au milieu de la roue a son appartion
		if(xpol != 0)curseur->setRotation(CEGUI::Vector3(0,0,angle));
		if(-90<angle && angle<90)highlight->setRotation(CEGUI::Vector3(0,0,0));
		if(-90>angle || angle > 90)highlight->setRotation(CEGUI::Vector3(0,0,180));

		
		//  Gestion des outputs
		if(Menu->isVisible() && !isVisible)
		{
			if(-90<angle && angle<90)output=1;
			if(-90>angle || angle > 90)output=2;	
		}
		else output=-1;
		
		if ( sys0.kb_pressing(OIS::KC_RIGHT) )
		{
			Current_highlighted=1;
			highlight->setRotation(CEGUI::Vector3(0,0,0));
			output = 1;
		}
		if ( sys0.kb_pressing(OIS::KC_LEFT) )
		{
			Current_highlighted=2;
			highlight->setRotation(CEGUI::Vector3(0,0,180));
			output = 2;
		}

		if (output != -1)
			ldbg.send("(Menu circulaire n°"+StringConverter::toString(id)+") Output sent : "+StringConverter::toString(output));

	}

	Menu->setVisible(isVisible);
	curseur->setVisible(isVisible);
	highlight->setVisible(isVisible);
	


	isthereOneVisible =false;
	for(int i = 0; i<next_id;i++)
	{
		if(GuiSandboxMan.winMgr->isWindowPresent("Sandbox_interface/Menu_Circulaire"+StringConverter::toString(i)+"/Frame"))
		{
			CEGUI::Window *AnotherMenu = GuiSandboxMan.winMgr->getWindow("Sandbox_interface/Menu_Circulaire"+StringConverter::toString(i)+"/Frame");
			isthereOneVisible = isthereOneVisible || AnotherMenu->isVisible();
		}
	}
	
	

}

void Menuci::setAllwaysOnTop(bool AllwaysOnTop)
{
	highlight->setAlwaysOnTop(AllwaysOnTop);
	Menu->setAlwaysOnTop(AllwaysOnTop);
	curseur->setAlwaysOnTop(AllwaysOnTop);
}
























































































































