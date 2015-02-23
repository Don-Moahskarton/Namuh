#include "Namuh_Main_H.h"

using namespace Ogre;
extern const OIS::MouseState &ms = sys0.mMouse->getMouseState();

namuh_system sys1;

namuh_system::namuh_system(void) //Constructeur
{
    escQuit = false;

	//définition du fichier de configuration racine
	MainConfigFileSyntax.push_back(new ParserBalise);
	MainConfigFileSyntax.back()->Name = "General";
	MainConfigFileSyntax.back()->StringKeywords.push_back("ActiveProfile");

	
	ifstream file ("Namuh.cfg",ios::in);
	if(!file)
	{
		file.close();
		ldbg.send("(Sys1 Exeption) Unable to open main config file, will prompt for informations to create a new one");
		ProfileMgr.isCurrentProfileValid = false;

	}
	else
	{
		file.close();
		ProfileMgr.isCurrentProfileValid = true;
		std::vector<NamedDataPool> RawMainConfigFile = Map_Parser.ParseFile(MainConfigFileSyntax,"Namuh.cfg");
		
		// Retrouve l'index des pools de données
		Ogre::uint	GeneralPoolIndex = -1;
		for(Ogre::uint i = 0; i < RawMainConfigFile.size(); ++i)
			if(RawMainConfigFile[i].Name == "General")
				GeneralPoolIndex = i;
		if(GeneralPoolIndex == -1)
		{
			ldbg.send("(Sys1 Exeption) Unable to load main config file : file is corrupted (Cannot find 'General' Balise), will prompt to create a new one");
			ProfileMgr.isCurrentProfileValid = false;
		}
		else
			ProfileMgr.setCurrent(ProfileMgr.Load("Players Profiles Datas/"+StringFromName(RawMainConfigFile[GeneralPoolIndex],"ActiveProfile")+"/MainFile.cfg"));
	}
}

namuh_system::~namuh_system(void) //Destructeur
{
	// write the main config file

	// construit les Datapools qui contiendront toutes les données a enregister
	std::vector<NamedDataPool*>Config_data;
	// General
	Config_data.push_back(new NamedDataPool);
	Config_data.back()->Name = "General";
		Config_data.back()->String_Data.push_back(*(new NamedString("ActiveProfile", ProfileMgr.getCurrent()->Name)));


	// Write the file
	File_Serialiser.WriteFile(Config_data,"Namuh.cfg");

	// Write down the current user profile
	ProfileMgr.SaveAs(ProfileMgr.ProfilePath+"MainFile.cfg",ProfileMgr.getCurrent());;
}
void namuh_system::render (bool force_full_render)
{
	if (!force_full_render) // termine le rendu si demandé (comportement par défault)
	{
	
		/*
		sys0.mRoot->_fireFrameStarted();

	
		Ogre::SceneManagerEnumerator::SceneManagerIterator::iterator a = sys0.mRoot->getSceneManagerIterator().begin();
		Ogre::SceneManagerEnumerator::SceneManagerIterator::iterator b = sys0.mRoot->getSceneManagerIterator().end();
		b--;

		//Rendu de toutes les render tagets sauf la principale
		MapIterator<Ogre::RenderTargetMap> it = sys0.mRoot->getRenderSystem()->getRenderTargetIterator();
		MapIterator<Ogre::RenderTargetMap>::iterator itarg;

		for( itarg = it.begin(); itarg != it.end(); ++itarg )
		{
			if( itarg->second->isActive() && !itarg->second->isAutoUpdated() &&  itarg->second->getName() != sys0.windowName)
					itarg->second->update(false);
		}
		sys0.mRoot->getRenderTarget(sys0.windowName)->update(false);


		// give client app opportunity to use queued GPU time
		sys0.mRoot->_fireFrameRenderingQueued();
		// block for final swap
		sys0.mRoot->getRenderSystem()->_swapAllRenderTargetBuffers(sys0.mRoot->getRenderSystem()->getWaitForVerticalBlank());
		sys0.mRoot->getRenderTarget(sys0.windowName)->swapBuffers(sys0.mRoot->getRenderSystem()->getWaitForVerticalBlank());

		// This belongs here, as all render targets must be updated before events are
		// triggered, otherwise targets could be mismatched.  This could produce artifacts,
		// for instance, with shadows.
		for (SceneManagerEnumerator::SceneManagerIterator it = sys0.mRoot->getSceneManagerIterator(); it.hasMoreElements(); it.moveNext())
			it.peekNextValue()->_handleLodEvents();



		sys0.mRoot->_fireFrameEnded();*/

		sys0.mRoot->_fireFrameStarted();
	
		//Rendu de toutes les render tagets sauf la principale
		MapIterator<Ogre::RenderTargetMap> it = sys0.mRoot->getRenderSystem()->getRenderTargetIterator();
		MapIterator<Ogre::RenderTargetMap>::iterator itarg = it.begin();

		for( itarg = it.begin(); itarg != it.end(); ++itarg )
		{
			if( itarg->second->isActive() && !itarg->second->isAutoUpdated() &&  itarg->second->getName() != sys0.windowName)
					itarg->second->update(false);
		}

	 	if (force_full_render) // termine le rendu si demandé (comportement par défault)
	 	{
		
		
			if(Hydrax_man.initialised)
			{

				Hydrax_man.update(0.05f);	
				Hydrax_man.mHydrax->updateOnlyRTT(0.0f);
			}

			sys0.mRoot->getRenderTarget(sys0.windowName)->update(false);


			// give client app opportunity to use queued GPU time
			sys0.mRoot->_fireFrameRenderingQueued();
			// block for final swap
			sys0.mRoot->getRenderSystem()->_swapAllRenderTargetBuffers(sys0.mRoot->getRenderSystem()->getWaitForVerticalBlank());
			sys0.mRoot->getRenderTarget(sys0.windowName)->swapBuffers(sys0.mRoot->getRenderSystem()->getWaitForVerticalBlank());

			// This belongs here, as all render targets must be updated before events are
			// triggered, otherwise targets could be mismatched.  This could produce artifacts,
			// for instance, with shadows.
			for (SceneManagerEnumerator::SceneManagerIterator it = sys0.mRoot->getSceneManagerIterator(); it.hasMoreElements(); it.moveNext())
				it.peekNextValue()->_handleLodEvents();


			sys0.mRoot->_fireFrameEnded();
		}
			
	}
	else
		sys0.mRoot->renderOneFrame();
	//sys0.RefractionRenderTarget->update(); // Rafraichie la réfraction
}
void  namuh_system::update(float time, bool force_full_render)
{
	namuh_system::update_wo_render();
	namuh_system::render(force_full_render);
}
void  namuh_system::update_wo_render(void)
{
	// Update des Stats
	MoyFPS = sys0.win->getAverageFPS();
	FPS = sys0.win->getLastFPS();
	BestFPS = sys0.win->getBestFPS();
	WorstFPS = sys0.win->getWorstFPS();
	Polycount = sys0.win->getTriangleCount();
		
    Ogre::WindowEventUtilities::messagePump(); //guette des messages de windows (Type Verr MAJ ou Shift)
    sys0.mKeyboard->capture(); // Met a jour le clavier
	sys0.update_SmartMouse(); // Toujours a appeller avant le getMouseState
	sys0.update_SmartKeyboard(); // Update de l'historique des touches pour avoir de releasing et des pressing key
    sys0.mMouse->capture(); const OIS::MouseState &ms = sys0.mMouse->getMouseState(); // Met a jour la souris
    if(!sys0.mKeyboard->isKeyDown(OIS::KC_ESCAPE) && escQuit && last_esc) // Quitte si Echap
    {
        LogManager::getSingletonPtr()->logMessage("*** Quit command (Escape) ***");
        MustQuit = true;
    }
	last_esc = sys0.mKeyboard->isKeyDown(OIS::KC_ESCAPE);
}
void namuh_system::logmsg(string msg)
{
    LogManager::getSingletonPtr()->logMessage(msg,LML_CRITICAL);
}
void namuh_system::resetMustQuit(void)
{
    MustQuit = last_esc = false;
}














BOOL IsDots(const TCHAR* str)
{
    if(_tcscmp(str,".") && _tcscmp(str,"..")) return FALSE;
    return TRUE;
}

BOOL DeleteDirectory(const TCHAR* sPath) 
{
	HANDLE hFind; // file handle
	WIN32_FIND_DATA FindFileData;
	 
	TCHAR DirPath[MAX_PATH];
	TCHAR FileName[MAX_PATH];
	 
	_tcscpy(DirPath,sPath);
	_tcscat(DirPath,"\\*"); // searching all files
	_tcscpy(FileName,sPath);
	_tcscat(FileName,"\\");
	 
	// find the first file
	hFind = FindFirstFile(DirPath,&FindFileData);
	if(hFind == INVALID_HANDLE_VALUE) return FALSE;
	_tcscpy(DirPath,FileName);
	 
	bool bSearch = true;
	while(bSearch) 
	{ // until we find an entry
		if(FindNextFile(hFind,&FindFileData)) 
		{
			if(IsDots(FindFileData.cFileName)) continue;
			_tcscat(FileName,FindFileData.cFileName);
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				// we have found a directory, recurse
				if(!DeleteDirectory(FileName)) 
				{
					FindClose(hFind);
					return FALSE; // directory couldn't be deleted
				}
			// remove the empty directory
			RemoveDirectory(FileName);
			_tcscpy(FileName,DirPath);
			}
			else
			{
				if(!DeleteFile(FileName))
				{ // delete the file
					FindClose(hFind);
					return FALSE;
				}
				_tcscpy(FileName,DirPath);
			}
		}
		else 
		{
			// no more files there
			if(GetLastError() == ERROR_NO_MORE_FILES)
			bSearch = false;
			else
			{
				// some error occurred; close the handle and return FALSE
				FindClose(hFind);
				return FALSE;
			}
		}
	}
	FindClose(hFind); // close the file handle
	return RemoveDirectory(sPath); // remove the empty directory
}






























