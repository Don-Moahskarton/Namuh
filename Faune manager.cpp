#include "Namuh_Main_H.h"

using namespace Ogre;

Faune_Page::Faune_Page(void)
{	
}
Faune_Page::~Faune_Page(void)
{
}

void Faune_Page::initialise(int rqMinXBorder,int rqMaxXBorder ,int rqMinZBorder ,int rqMaxZBorder)
{  
	MinXBorder = rqMinXBorder;
	MaxXBorder = rqMaxXBorder;
	MinZBorder = rqMinZBorder;
	MaxZBorder = rqMaxZBorder;

	Center = Ogre::Vector3((MinXBorder+MaxXBorder)/2,0,(MinZBorder+MaxZBorder)/2);
	Center = Ogre::Vector3(Center.x,terrain_height(Center.x, Center.z,0),Center.z);
	size = MaxXBorder - MinXBorder;

	/*
	// Also uses cubes to show updated pages
	FlagEntity = sys0.mgr->createEntity("Flag" + StringConverter::toString(sys0.timer.getMicroseconds()),"Cube.mesh");
	//FlagEntity->setMaterial(MaterialManager::getSingleton().getByName("blink"));
	FlagNode = sys0.mgr->getRootSceneNode()->createChildSceneNode();
	FlagNode->setScale(5*size/15,0.5,5*size/15);
	FlagNode->attachObject(FlagEntity);
	FlagNode->setPosition(	MinXBorder + size/2,
							250 + size / 2,
							MinZBorder + size/2);
	FlagNode->setVisible(false); // Also uncomment the FlagNode->setVisible(NewVisibility); in  Faune_Page::setVisible
	*/
}

void Faune_Page::update(float TimeLapse)
{
	if(SubPages.size() != 0) // Si la page contient des sous pages on update les sous pages
		for(int t = 0;t != SubPages.size();t++)
			SubPages[t]->update(TimeLapse);
	else
		for(Ogre::uint t = 0;t < ContainedEntities.size();t++)
		{
			ContainedEntities[t]->update(TimeLapse);
		
			// Si une entitée sors de son enclot, on doit la réattibuer a la bonne Page
			char posID =		char(bool(MaxXBorder < ContainedEntities[t]->position.x)*1) +
								char(bool(ContainedEntities[t]->position.x < MinXBorder)*2) + 
								char(bool(MaxZBorder < ContainedEntities[t]->position.z)*4) + 
								char(bool(ContainedEntities[t]->position.z < MinZBorder)*8) ;

			if(posID != 0)
			{
				Faune_Page* NewPage = NULL;
				if(posID ==  1) NewPage = Right;
				if(posID ==  2) NewPage = Left;
				if(posID ==  4) NewPage = Top;
				if(posID ==  5) NewPage = TopRight;
				if(posID ==  6) NewPage = TopLeft;
				if(posID ==  8) NewPage = Bottom;
				if(posID ==  9) NewPage = BottomRight;
				if(posID == 10) NewPage = BottomLeft;


				// Si on sors de la map, on est attribué a la dernière page qui nous as hébergé
				if(NewPage != NULL) 
				{
					ContainedEntities[t]->Container = NewPage;
	
					NewPage->ContainedEntities.push_back(ContainedEntities[t]);
		
					ContainedEntities[t] = ContainedEntities.back();
					ContainedEntities.pop_back();

					// applique la visibilité de la nouvelle page
					NewPage->ContainedEntities.back()->AnimalNode->setVisible(NewPage->isVisible);
				}
			
			}
		}
}

void Faune_Page::destroy(void)
{
	// if this page is not a leaf, destroy childs
	if (!SubPages.size())
		for (int t = 0;t != SubPages.size();t++)
		{
			SubPages[t]->destroy();
			delete SubPages[t];
		}
	else // else we have to destroy it
	{		
		// destroy all entities of this pages
		for(int t = 0;t != ContainedEntities.size();t++)
			ContainedEntities[t]->destroy();
		// free their memories
		for(int t = 0;t != ContainedEntities.size();t++)
			ContainedEntities.pop_back();

	}
}

void Faune_Page::setVisible(bool NewVisibility)
{
	if(NewVisibility != isVisible)
	{
		isVisible = NewVisibility;
		if(SubPages.size() != 0) // Si la page contient des sous pages on update les sous pages
			for(int t = 0;t != SubPages.size();t++)
				SubPages[t]->setVisible(NewVisibility);
		else // Sinon on cache les maillages de la feuille de l'arbre
		{
			for(int t = 0;t != ContainedEntities.size();t++)
				ContainedEntities[t]->AnimalNode->setVisible(NewVisibility);
			//FlagNode->setVisible(NewVisibility);
		}
	}
}



Faune_mgr Faune_man;
// Code des méthodes de la classe
Faune_mgr::Faune_mgr(void)
{	
	initialised = false;
	debugInfoTimestamp = 0;
}
Faune_mgr::~Faune_mgr(void)
{
}

void Faune_mgr::initialise(void)
{  
	if(!initialised)
	{
		initialised = true;

		float MinimalFeuilleDimention = FloatFromName(Map.MapDataPool,"Faune_Page_Min_Size");
		DisplayDistance = FloatFromName(Map.MapDataPool,"Faune_Display_Distance");
		std::vector<Faune_Page*> FeuillesShortCut,NextFeuillesShortCut;
		Faune_Page WorldPage;
		RootPage = WorldPage;
		NextFeuillesShortCut.clear();
		NextFeuillesShortCut.push_back(&RootPage);
		NextFeuillesShortCut.back()->initialise(	Vector3FromName(Map.MapDataPool,"World_Position").x - FloatFromName(Map.MapDataPool,"World_Horisontal_Size")/2,
													Vector3FromName(Map.MapDataPool,"World_Position").x + FloatFromName(Map.MapDataPool,"World_Horisontal_Size")/2,
													Vector3FromName(Map.MapDataPool,"World_Position").z - FloatFromName(Map.MapDataPool,"World_Horisontal_Size")/2,
													Vector3FromName(Map.MapDataPool,"World_Position").z + FloatFromName(Map.MapDataPool,"World_Horisontal_Size")/2);
		NextFeuillesShortCut.back()->Parent = NULL;

		float FeuilleDimention = NextFeuillesShortCut.back()->size;

		while(FeuilleDimention/2 > MinimalFeuilleDimention)
		{
			FeuillesShortCut = NextFeuillesShortCut;
			NextFeuillesShortCut.clear();
			for(Ogre::uint t =0; t < FeuillesShortCut.size(); t++ )
			{
				Faune_Page* ParentPage = FeuillesShortCut[t];
				// Découpe la page en 4

				Faune_Page* ChildFaunePage1 = new Faune_Page();
				ParentPage->SubPages.push_back(ChildFaunePage1);
				ParentPage->SubPages.back()->Parent = ParentPage;
				ParentPage->SubPages.back()->initialise(ParentPage->MinXBorder, 
														ParentPage->MinXBorder + ParentPage->size/2, 
														ParentPage->MinZBorder, 
														ParentPage->MinZBorder + ParentPage->size/2);
				NextFeuillesShortCut.push_back(ParentPage->SubPages.back());
				Faune_Page* ChildFaunePage2 = new Faune_Page();
				ParentPage->SubPages.push_back(ChildFaunePage2);
				ParentPage->SubPages.back()->Parent = ParentPage;
				ParentPage->SubPages.back()->initialise(ParentPage->MinXBorder + ParentPage->size/2, 
														ParentPage->MaxXBorder, 
														ParentPage->MinZBorder, 
														ParentPage->MinZBorder + ParentPage->size/2);
				NextFeuillesShortCut.push_back(ParentPage->SubPages.back());
				Faune_Page* ChildFaunePage3 = new Faune_Page();
				ParentPage->SubPages.push_back(ChildFaunePage3);
				ParentPage->SubPages.back()->Parent = ParentPage;
				ParentPage->SubPages.back()->initialise(ParentPage->MinXBorder, 
														ParentPage->MinXBorder + ParentPage->size/2, 
														ParentPage->MinZBorder+ ParentPage->size/2, 
														ParentPage->MaxZBorder);
				NextFeuillesShortCut.push_back(ParentPage->SubPages.back());
				Faune_Page* ChildFaunePage4 = new Faune_Page();
				ParentPage->SubPages.push_back(ChildFaunePage4);
				ParentPage->SubPages.back()->Parent = ParentPage;
				ParentPage->SubPages.back()->initialise(ParentPage->MinXBorder + ParentPage->size/2, 
														ParentPage->MaxXBorder, 
														ParentPage->MinZBorder + ParentPage->size/2, 
														ParentPage->MaxZBorder);
				NextFeuillesShortCut.push_back(ParentPage->SubPages.back());
			}
			FeuilleDimention = FeuillesShortCut[0]->SubPages.back()->size;
		}
		FeuillesShortCut = NextFeuillesShortCut;


		this->ChainageOctupleFirstPass(&RootPage);
		this->ChainageOctupleSecondPass(&RootPage);

		Faune_Entity* NewAnimal;
		Ogre::Vector3 AnimalPos;
		//on peuple les pages

		// Essai : on place les poissons uniquement sous l'eau


		int atemptsCount = 10; // 10 essais pour lacer correctement l'animal, sinon, on le zappe
		int totalNanimoCount = 0;
		for(Ogre::uint t =0; t < FeuillesShortCut.size(); t++ )
		{
			Faune_Page* CurrentPage = FeuillesShortCut[t];
			std::vector<Faune_Entity*>* WorkELst = &FeuillesShortCut[t]->ContainedEntities;

			Ogre::uint nanimoCount = int(20*pow(Ogre::Math::RangeRandom(0,5)/4,8)); //  jusqu'a 120 animaux par pages, mais moyenne a 11 anomaux
			for(Ogre::uint i = 0; i < nanimoCount; i++) // 1 nanimo par page
			{

				atemptsCount = 10;
				AnimalPos = Ogre::Vector3(	Ogre::Math::RangeRandom(CurrentPage->MinXBorder,CurrentPage->MaxXBorder),
											0,
											Ogre::Math::RangeRandom(CurrentPage->MinZBorder,CurrentPage->MaxZBorder));
				while (atemptsCount > 0 && terrain_height(AnimalPos.x, AnimalPos.z,0) + 1 > Hydrax_man.sea_level)  // essaye de re-placer l'animal si l'emplacement ne va pas
				{
					
					AnimalPos = Ogre::Vector3(	Ogre::Math::RangeRandom(CurrentPage->MinXBorder,CurrentPage->MaxXBorder),
												0,
												Ogre::Math::RangeRandom(CurrentPage->MinZBorder,CurrentPage->MaxZBorder));
					atemptsCount--;
				}

				if (terrain_height(AnimalPos.x, AnimalPos.z,0) + 1 < Hydrax_man.sea_level)  // ne pose un nanimo que si la place est correcte
				{
					totalNanimoCount++;
					NewAnimal = new Faune_Entity;
					WorkELst->push_back(NewAnimal);
					AnimalPos.y = terrain_height(AnimalPos.x, AnimalPos.z,0);
					WorkELst->back()->initialise(CurrentPage,AnimalPos);
				}
			}
			FeuillesShortCut[t]->setVisible(false);
		}
		GuessedPlayerPage = this->LocatePlayer(Vector3FromName(Map.MapDataPool,"Spawn_Position"));
		ldbg.send("Just popped " + toOgStr(totalNanimoCount) + " cutes animals !");
		/*
		//Uses cubes to visually delimit bottom layer pages
		Ogre::Entity* BaliseEntity = NULL;
		Ogre::SceneNode* BaliseNode = NULL;
		for(Ogre::uint t =0; t < FeuillesShortCut.size(); t++ )
		{
			BaliseEntity = sys0.mgr->createEntity("Balise" + StringConverter::toString(sys0.timer.getMicroseconds()),"Cube.mesh");
			BaliseEntity->setMaterial(MaterialManager::getSingleton().getByName("fur"));
			BaliseNode = sys0.mgr->getRootSceneNode()->createChildSceneNode();
			BaliseNode->setScale(0.5,1000,0.5);
			BaliseNode->attachObject(BaliseEntity);
			BaliseNode->setPosition(	FeuillesShortCut[t]->MinXBorder,
										terrain_height(FeuillesShortCut[t]->MinXBorder,FeuillesShortCut[t]->MinZBorder,0),
										FeuillesShortCut[t]->MinZBorder);
		}
		*/

	}
}

void Faune_mgr::destroy(void)
{
	if (!initialised)
		ldbg.send("Faune mgr : Attempted to destroy a non-initailised manager : nothing to do");
	else
	{
		ldbg.send("Faune mgr : destroying...");
		RootPage.destroy();
		ldbg.send("Faune mgr : destruction completed");
		initialised = false;
	}
}

void Faune_mgr::ChainageOctupleFirstPass(Faune_Page* CurrentPage) // Ne lie que quadruplement par top, bot, left et right
{
	Faune_Page		*SearchCursor = NULL,
					*SearchResult = NULL;
	std::vector<int> CurrentPagePath;
	int index = 0;
	bool founded = false;


	// Recherche de la Page Bottom
	SearchCursor = CurrentPage;
	CurrentPagePath.clear();
	founded = false;
	index = 0;
	while(SearchCursor->Parent != NULL && !founded)
	{
		index = getIndexInParentSubPages(SearchCursor);
		CurrentPagePath.push_back(index);
		if(index == 0 || index == 1) founded = true;
		else SearchCursor = SearchCursor->Parent;
	}
	if(founded)
	{
			if(CurrentPagePath.back() == 0) SearchResult = SearchCursor->Parent->SubPages[2];
			if(CurrentPagePath.back() == 1) SearchResult = SearchCursor->Parent->SubPages[3];

			for(int t = CurrentPagePath.size()-2;t >= 0;t--)
			{
				if(CurrentPagePath[t] == 2) SearchResult = SearchResult->SubPages[0];
				if(CurrentPagePath[t] == 3) SearchResult = SearchResult->SubPages[1];
			}
	}
	else
	{
		SearchResult = NULL;
		SearchCursor = NULL;
	}
	CurrentPage->Bottom = SearchResult;
	//if(founded)
	//ldbg.send("Page centered in " + Ogre::StringConverter::toString(CurrentPage->Center) + " with a size of "  + Ogre::StringConverter::toString(CurrentPage->size) + " have bottom link with the page centered in " + Ogre::StringConverter::toString(CurrentPage->Bottom->Center) + "with a size of "  + Ogre::StringConverter::toString(CurrentPage->Bottom->size));

	
	// Recherche de la Page Left
	SearchCursor = CurrentPage;
	CurrentPagePath.clear();
	founded = false;
	index = 0;
	while(SearchCursor->Parent != NULL && !founded)
	{
		index = getIndexInParentSubPages(SearchCursor);
		CurrentPagePath.push_back(index);
		if(index == 1 || index == 3) founded = true;
		else SearchCursor = SearchCursor->Parent;
	}
	if(founded)
	{
		if(CurrentPagePath.back() == 1) SearchResult = SearchCursor->Parent->SubPages[0];
		if(CurrentPagePath.back() == 3) SearchResult = SearchCursor->Parent->SubPages[2];

		for(int t = CurrentPagePath.size()-2;t >= 0;t--)
		{
			if(CurrentPagePath[t] == 0) SearchResult = SearchResult->SubPages[1];
			if(CurrentPagePath[t] == 2) SearchResult = SearchResult->SubPages[3];
		}
	}
	else
	{
		SearchResult = NULL;
		SearchCursor = NULL;
	}
	CurrentPage->Left = SearchResult;
	
	//if(founded)
	//ldbg.send("Page centered in " + Ogre::StringConverter::toString(CurrentPage->Center) + " with a size of "  + Ogre::StringConverter::toString(CurrentPage->size) + " have Left link with the page centered in " + Ogre::StringConverter::toString(CurrentPage->Left->Center) + " with a size of "  + Ogre::StringConverter::toString(CurrentPage->Left->size));


	// Recherche de la Page Right
	SearchCursor = CurrentPage;
	CurrentPagePath.clear();
	founded = false;
	index = 0;
	while(SearchCursor->Parent != NULL && !founded)
	{
		index = getIndexInParentSubPages(SearchCursor);
		CurrentPagePath.push_back(index);
		if(index == 0 || index == 2) founded = true;
		else SearchCursor = SearchCursor->Parent;
	}
	if(founded)
	{
		if(CurrentPagePath.back() == 0) SearchResult = SearchCursor->Parent->SubPages[1];
		if(CurrentPagePath.back() == 2) SearchResult = SearchCursor->Parent->SubPages[3];

		for(int t = CurrentPagePath.size()-2;t >= 0;t--)
		{
			if(CurrentPagePath[t] == 1) SearchResult = SearchResult->SubPages[0];
			if(CurrentPagePath[t] == 3) SearchResult = SearchResult->SubPages[2];
		}
	}
	else
	{
		SearchResult = NULL;
		SearchCursor = NULL;
	}
	CurrentPage->Right = SearchResult;
	
	//if(founded)
	//ldbg.send("Page centered in " + Ogre::StringConverter::toString(CurrentPage->Center) + " with a size of "  + Ogre::StringConverter::toString(CurrentPage->size) + " have right link with the page centered in " + Ogre::StringConverter::toString(CurrentPage->Right->Center) + " with a size of "  + Ogre::StringConverter::toString(CurrentPage->Right->size));

	// Recherche de la Page Top
	SearchCursor = CurrentPage;
	CurrentPagePath.clear();
	founded = false;
	index = 0;
	while(SearchCursor->Parent != NULL && !founded)
	{
		index = getIndexInParentSubPages(SearchCursor);
		CurrentPagePath.push_back(index);
		if(index == 2 || index == 3) founded = true;
		else SearchCursor = SearchCursor->Parent;
	}
	if(founded)
	{
		if(CurrentPagePath.back() == 2) SearchResult = SearchCursor->Parent->SubPages[0];
		if(CurrentPagePath.back() == 3) SearchResult = SearchCursor->Parent->SubPages[1];

		for(int t = CurrentPagePath.size()-2;t >= 0;t--)
		{
			if(CurrentPagePath[t] == 0) SearchResult = SearchResult->SubPages[2];
			if(CurrentPagePath[t] == 1) SearchResult = SearchResult->SubPages[3];
		}
	}
	else
	{
		SearchResult = NULL;
		SearchCursor = NULL;
	}
	CurrentPage->Top = SearchResult;
	
	//if(founded)
	//ldbg.send("Page centered in " + Ogre::StringConverter::toString(CurrentPage->Center) + " with a size of "  + Ogre::StringConverter::toString(CurrentPage->size) + " have top link with the page centered in " + Ogre::StringConverter::toString(CurrentPage->Top->Center) + " with a size of "  + Ogre::StringConverter::toString(CurrentPage->Top->size));
	
	if(CurrentPage->SubPages.size() != 0)
	{
		ChainageOctupleFirstPass(CurrentPage->SubPages[0]);
		ChainageOctupleFirstPass(CurrentPage->SubPages[1]);
		ChainageOctupleFirstPass(CurrentPage->SubPages[2]);
		ChainageOctupleFirstPass(CurrentPage->SubPages[3]);

	}
}

void Faune_mgr::ChainageOctupleSecondPass(Faune_Page* CurrentPage) // Lie les 4 fois manquantes
{
	if(CurrentPage->Left != NULL)	CurrentPage->BottomLeft = CurrentPage->Left->Bottom;
	else							CurrentPage->BottomLeft = NULL;
	if(CurrentPage->Left != NULL)	CurrentPage->TopLeft = CurrentPage->Left->Top;
	else							CurrentPage->TopLeft = NULL;
	if(CurrentPage->Right != NULL)	CurrentPage->BottomRight = CurrentPage->Right->Bottom;
	else							CurrentPage->BottomRight = NULL;
	if(CurrentPage->Right != NULL)	CurrentPage->TopRight = CurrentPage->Right->Top;
	else							CurrentPage->TopRight = NULL;

	if(CurrentPage->SubPages.size() != 0) //Tant que la page a des Subpages, on les lies récusivement
	{
		ChainageOctupleSecondPass(CurrentPage->SubPages[0]);
		ChainageOctupleSecondPass(CurrentPage->SubPages[1]);
		ChainageOctupleSecondPass(CurrentPage->SubPages[2]);
		ChainageOctupleSecondPass(CurrentPage->SubPages[3]);
	}
}

int Faune_mgr::getIndexInParentSubPages(Faune_Page* CurrentPage)
{
	if(CurrentPage->Parent != NULL)
	{
		if(CurrentPage == CurrentPage->Parent->SubPages[0]) return 0;
		if(CurrentPage == CurrentPage->Parent->SubPages[1]) return 1;
		if(CurrentPage == CurrentPage->Parent->SubPages[2]) return 2;
		if(CurrentPage == CurrentPage->Parent->SubPages[3]) return 3;
	}
	else
	{
		// On peut metttre un message de debug, ici typiquement la RootPage arrivera
		return -1;
	}
	return -1;
}



Faune_Page* Faune_mgr::LocatePlayer(Ogre::Vector3 playerpos)
{
	Faune_Page* SearchScope = &RootPage;
	Ogre::uint t = 0;
	while(t < SearchScope->SubPages.size())
	{
		if(	SearchScope->SubPages[t]->MinXBorder <= playerpos.x && 
			SearchScope->SubPages[t]->MaxXBorder >= playerpos.x && 
			SearchScope->SubPages[t]->MinZBorder <= playerpos.z && 
			SearchScope->SubPages[t]->MaxZBorder >= playerpos.z )
		{
			if(SearchScope->SubPages[t]->SubPages.size() == 0)
				return SearchScope->SubPages[t];
			else
			{
				SearchScope = SearchScope->SubPages[t];
				t = -1; // reset le "for" avec un nouveau champ de recherche
			}
		}
		t++;
	}
	if (!t)
		ldbg.send("Faune mgr : warning : no subPage founded in root page");
	ldbg.send("Faune_mgr : Falied to locate player : No player founded in Paged World. Will return rootPage", false);
	return &RootPage;
}

void Faune_mgr::update(float ellapsed_time, Ogre::Vector3 playerpos)
{
	Faune_Page* LastAdded;
	if(GuessedPlayerPage == NULL)
	{
		GuessedPlayerPage = this->LocatePlayer(playerpos);
		UpdateList.clear();
		UpdateList.push_back(GuessedPlayerPage);
	}

	if(GuessedPlayerPage->SubPages.size() != 0) // Si on guess pas une feuille, on essaye de locate dans cette page
	{
		GuessedPlayerPage = this->LocatePlayer(playerpos);
		UpdateList.clear();
		UpdateList.push_back(GuessedPlayerPage);
	}
	// Si le joueur est pas la ou on l'attend, on fait une synthèse de la ou il peut être
	posID =		char(GuessedPlayerPage->MaxXBorder < playerpos.x)*1 +
				char(playerpos.x < GuessedPlayerPage->MinXBorder)*2 + 
				char(GuessedPlayerPage->MaxZBorder < playerpos.z)*4 + 
				char(playerpos.z < GuessedPlayerPage->MinZBorder)*8 ;

	if(posID != 0)
	{
		if(posID ==  1)GuessedPlayerPage = GuessedPlayerPage->Right;
		if(posID ==  2)GuessedPlayerPage = GuessedPlayerPage->Left;
		if(posID ==  4)GuessedPlayerPage = GuessedPlayerPage->Top;
		if(posID ==  5)GuessedPlayerPage = GuessedPlayerPage->TopRight;
		if(posID ==  6)GuessedPlayerPage = GuessedPlayerPage->TopLeft;
		if(posID ==  8)GuessedPlayerPage = GuessedPlayerPage->Bottom;
		if(posID ==  9)GuessedPlayerPage = GuessedPlayerPage->BottomRight;
		if(posID == 10)GuessedPlayerPage = GuessedPlayerPage->BottomLeft;
		if(GuessedPlayerPage != NULL)
		{
			posID =		char(GuessedPlayerPage->MaxXBorder < playerpos.x)*1 +
						char(playerpos.x < GuessedPlayerPage->MinXBorder)*2 + 
						char(GuessedPlayerPage->MaxZBorder < playerpos.z)*4 + 
						char(playerpos.z < GuessedPlayerPage->MinZBorder)*8 ;
		}
		else
			posID = -1; // -1 est une valeur non nulle, pour faire faire le LoactePlayer
		if(posID != 0) GuessedPlayerPage = this->LocatePlayer(playerpos);

		// Destruction de l'ancienne updatelist
		for(Ogre::uint t = 0; t < UpdateList.size(); t++) // Top bullshit
			UpdateList[t]->setVisible(false);
		UpdateList.clear();
		UpdateList.push_back(GuessedPlayerPage);
		
		// Generation de la nouvelle updatelist
		// Va tout a droite
		LastAdded = GuessedPlayerPage;
		while(LastAdded->Right != NULL && abs(LastAdded->Right->Center.x-playerpos.x) < DisplayDistance)
		{
			UpdateList.push_back(LastAdded->Right);
			LastAdded = UpdateList.back();
		}
		// Tout a gauche
		LastAdded = GuessedPlayerPage;
		while(LastAdded->Left != NULL && abs(LastAdded->Left->Center.x-playerpos.x) < DisplayDistance)
		{
			UpdateList.push_back(LastAdded->Left);
			LastAdded = UpdateList.back();
		}
		
		// Extrude verticalement
		int initialSize = UpdateList.size(); // Hum, allocation mémoire real time ?
		for(int t = 0; t < initialSize; t++)
		{
			LastAdded = UpdateList[t];
			while(LastAdded->Top != NULL && FakeDistance2D(LastAdded->Top->Center.x,LastAdded->Top->Center.z,GuessedPlayerPage->Center.x,GuessedPlayerPage->Center.z) < DisplayDistance)
			{
				UpdateList.push_back(LastAdded->Top);
				LastAdded = UpdateList.back();
			}
			LastAdded = UpdateList[t];
			while(LastAdded->Bottom != NULL && FakeDistance2D(LastAdded->Bottom->Center.x,LastAdded->Bottom->Center.z,GuessedPlayerPage->Center.x,GuessedPlayerPage->Center.z) < DisplayDistance)
			{
				UpdateList.push_back(LastAdded->Bottom);
				LastAdded = UpdateList.back();
			}
		}

		for(Ogre::uint t = 0; t < UpdateList.size(); t++) // The return Of Top bullshit
			UpdateList[t]->setVisible(true);
	}
	if(!GuessedPlayerPage->isVisible)
		for(Ogre::uint t = 0; t < UpdateList.size(); t++) 
			UpdateList[t]->setVisible(true);

	// Update tout ce joli monde :
	for(Ogre::uint t = 0; t < UpdateList.size(); t++)
		UpdateList[t]->update(ellapsed_time);
}






























































































































