#include "Namuh_Main_H.h"

using namespace Ogre;

PlayerProfileManager ProfileMgr;

// Code des méthodes de la classe
PlayerProfileManager::PlayerProfileManager(void)
{
	// Definition de la syntaxe des fichiers de profil

	ProfileFileSyntax.push_back(new ParserBalise);
	ProfileFileSyntax.back()->Name = "General Info";
	ProfileFileSyntax.back()->StringKeywords.push_back("ProfileName");

	ProfileFileSyntax.push_back(new ParserBalise);
	ProfileFileSyntax.back()->Name = "Game preferences";

		ParserBalise FavoriteCstrLst = {"Favorite Construction List"};
		ProfileFileSyntax.back()->SousBalises.push_back(FavoriteCstrLst);

			ParserBalise FavoriteCstr = {"Favorite Construction"};
			ProfileFileSyntax.back()->SousBalises.back().SousBalises.push_back(FavoriteCstr);
		
			ProfileFileSyntax.back()->SousBalises.back().SousBalises.back().StringKeywords.push_back("FileName");

	ProfileFileSyntax.push_back(new ParserBalise);
	ProfileFileSyntax.back()->Name = "Misc";
	ProfileFileSyntax.back()->StringKeywords.push_back("Default_IP");
}
PlayerProfileManager::~PlayerProfileManager(void)
{
}
 

PlayerProfile* PlayerProfileManager::Load(string ProfileFile)
{
	std::vector<NamedDataPool> RawConfigFile = Map_Parser.ParseFile(ProfileFileSyntax, ProfileFile);
	LoadedProfiles.push_back(new PlayerProfile);

	// Retrouve l'index des pools de données
	Ogre::uint	GeneralInfoPoolIndex = -1,
				MiscPoolIndex = -1,
				GamePreferencePoolIndex = -1;
	for(Ogre::uint i = 0; i < RawConfigFile.size(); ++i)
	{
		if(RawConfigFile[i].Name == "General Info")
			GeneralInfoPoolIndex = i;
		else if (RawConfigFile[i].Name == "Game preferences")
			GamePreferencePoolIndex = i;
		else if (RawConfigFile[i].Name == "Misc")
			MiscPoolIndex = i;
	}
	if(GeneralInfoPoolIndex == -1)
	{
		ldbg.send("(PlayerProfileManager Exeption) Unable to load profile file " + ProfileFile + ". 'General Info' pool is missing. NULL will be returned.");
		return NULL;
	}
	if(GamePreferencePoolIndex == -1)
	{
		ldbg.send("(PlayerProfileManager Exeption) Unable to load profile file " + ProfileFile + ". 'Game preferences' pool is missing. NULL will be returned.");
		return NULL;
	}
	if(MiscPoolIndex == -1)
	{
		ldbg.send("(PlayerProfileManager Exeption) Unable to load profile file " + ProfileFile + ". 'Misc' pool is missing. NULL will be returned.");
		return NULL;
	}


	// Remplit le nouvel objet PlayerProfile

	LoadedProfiles.back()->Name = StringFromName(RawConfigFile[GeneralInfoPoolIndex],"ProfileName");
	LoadedProfiles.back()->Default_IP = StringFromName(RawConfigFile[MiscPoolIndex],"Default_IP");
	
	Ogre::uint FavCstrLst_Indexe = SubPoolsIndexesFromName(RawConfigFile[GamePreferencePoolIndex],"Favorite Construction List").back();
	for(Ogre::uint i = 0; i < RawConfigFile[GamePreferencePoolIndex].SubPool[FavCstrLst_Indexe].SubPool.size(); ++i)
		LoadedProfiles.back()->FavoritesConstructions.push_back(StringFromName(RawConfigFile[GamePreferencePoolIndex].SubPool[FavCstrLst_Indexe].SubPool[i],"FileName"));

	return LoadedProfiles.back();
}


void PlayerProfileManager::SaveAs(string ProfileFile, PlayerProfile* ProfileObject )
{	
	if(ProfileObject == NULL) ProfileObject = this->getCurrent();
	// construit les Datapools qui contiendront toutes les données a enregister
	std::vector<NamedDataPool*>Profile_data;

	// General Info
	Profile_data.push_back(new NamedDataPool);
	Profile_data.back()->Name = "General Info";
		Profile_data.back()->String_Data.push_back(*(new NamedString("ProfileName", ProfileObject->Name)));

	// Game Preferences
	Profile_data.push_back(new NamedDataPool);
	Profile_data.back()->Name = "Game preferences";
		Profile_data.back()->SubPool.push_back(*(new NamedDataPool));
		Profile_data.back()->SubPool.back().Name = "Favorite Construction List";

		for(Ogre::uint i = 0; i < ProfileObject->FavoritesConstructions.size(); ++i)
		{
			Profile_data.back()->SubPool.back().SubPool.push_back(*(new NamedDataPool));
			Profile_data.back()->SubPool.back().SubPool.back().Name = "Favorite Construction";

			Profile_data.back()->SubPool.back().SubPool.back().String_Data.push_back(*(new NamedString("FileName", ProfileObject->FavoritesConstructions[i])));
		}
	
	// Misc
	Profile_data.push_back(new NamedDataPool);
	Profile_data.back()->Name = "Misc";
		Profile_data.back()->String_Data.push_back(*(new NamedString("Default_IP", ProfileObject->Default_IP)));

		

	// Write the file
	File_Serialiser.WriteFile(Profile_data,ProfileFile);
}


void PlayerProfileManager::setCurrent(PlayerProfile* NewProfile)
{
	ProfilePath = "Players Profiles Datas/" + NewProfile->Name+"/";
	CurrentProfile = NewProfile;
}


PlayerProfile* PlayerProfileManager::getCurrent(void)
{
	return CurrentProfile;
}


void PlayerProfileManager::deleteCurrent(void)
{
	//TOFIX
	DeleteDirectory(&string("Players Profiles Datas/"+getCurrent()->Name)[0]);
}

void PlayerProfileManager::CreateNew(string Name)
{
	// Crée les dossiers du profil
	CreateDirectory (&string("Players Profiles Datas/"+Name)[0], NULL);
	CreateDirectory (&string("Players Profiles Datas/"+Name+"/Constructions")[0], NULL);
	
	// Crée un fichier de configuration vierge
	PlayerProfile NewProfile;
	NewProfile.Name = Name;
	SaveAs("Players Profiles Datas/"+Name+"/MainFile.cfg", &NewProfile);
}



PlayerProfile::PlayerProfile(void)
{
	Name = "Unamed Profile " + StringConverter::toString(sys0.timer.getMicroseconds());
	Default_IP = "127.0.0.1";
	// TODO : Mettre des constuction en favoris par défault (des construction préfaites)

}
PlayerProfile::~PlayerProfile(void)
{
}




























































































































