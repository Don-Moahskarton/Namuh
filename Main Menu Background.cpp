#include "Namuh_Main_H.h"

using namespace Ogre;

void Gui_MainMenu_manager::SetupBackground(void)
{
	if(!IsBackGroundInitialised)
	{
		Perso::cam_sync = raknet_man.peer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS); // On met la camera dans le joueur local

		Map.load("Maps/Main Menu background/Main Menu background.map");
		//Map.load("Maps/Caribean Lost Island/Caribean Lost Island.map");
		ldbg.send("Terrain loaded sucessfully");

	//	Hydrax_man.initialise();
		Atm_man.initialise();
		ldbg.send("Atmophere manager loaded sucessfully");

		PhysX_man.initialise();
		ldbg.send("PhysX loaded sucessfully");

		Map.terrain->dirtyLightmap(); //a faire au moins une fois quand meme
		if(sys0.RefractionRenderTarget)
			sys0.RefractionRenderTarget->update(); // Pareil
		Map.terrain->update();

		local_player.initialise();
		local_player.spawn(Vector3FromName(Map.MapDataPool,"Spawn_Position"),true);
		local_player.update(1/60,Mvt_Fly,false);
		local_player.OrientationInput =  Ogre::Vector3(	150,//				-local_player.Node->getOrientation().getYaw().valueDegrees(),
														-10				-local_player.rot.getPitch().valueDegrees(),0);
			
		ldbg.send("loading Paged geometry ");
		Paged_man.initialise();
		ldbg.send("Paged geometry loaded sucessfully");
	
		// testing faune destroyers
		ldbg.send("Loading Amnesic Faune "); 
		Amnesic_Faune_man.initialise(); 
		ldbg.send("Amnesic Faune Loaded sucessfully");

		ldbg.send("Loading Faune "); 
		Faune_man.initialise();
		ldbg.send("Faune Loaded sucessfully");
	
		ldbg.send("Loading static geometry"); 
		Entity* Grotte = sys0.mgr->createEntity("Grotte","Grotte.mesh");
		SceneNode* Grotte_n = sys0.mgr->getRootSceneNode()->createChildSceneNode();
		Grotte_n->attachObject(Grotte);
		Grotte_n->setPosition(100,45,100);
		Construction* estrade = CstrMgr.Load("Estrade Menu principal.nmh",Ogre::Vector3(115,45,115),Ogre::Quaternion(Ogre::Radian(0),Ogre::Vector3::UNIT_Y));

		estrade->Rotate(Ogre::Quaternion(Ogre::Radian(90),Ogre::Vector3::UNIT_Z));

		
		IsBackGroundInitialised = true;
	}
}




void Gui_MainMenu_manager::UpdateBackground(void)
{
	if(IsBackGroundInitialised)
	{
		float inv_FPS = 1/60;	// Otherwise PhysX freeze Oo   (yup, because inv_FPS should be passed from the main loop, otherwise inv_FPS = 0)
		PhysX_man.update(inv_FPS);
		local_player.update(inv_FPS,Mvt_Fly,false);
			
		//Faune_man.update(inv_FPS,local_player.pos);
		//Amnesic_Faune_man.update(inv_FPS,Ogre::Vector2(local_player.pos.x,local_player.pos.z)); 
		Atm_man.update(inv_FPS);
	//	Hydrax_man.update(inv_FPS);
		Map.terrain->update();
		Paged_man.update();
	}
}


void Gui_MainMenu_manager::DestroyBackground(void)
{
	if(IsBackGroundInitialised)
	{
		sys0.mgr->destroyEntity("Grotte");
		//Hydrax_man.destroy();
		Atm_man.destroy();

		// Flush constructions
		while (CstrMgr.LoadedConstructions.size())
		{
			CstrMgr.Destroy(CstrMgr.LoadedConstructions.back());
			CstrMgr.LoadedConstructions.pop_back();
		}

		PhysX_man.destroy();
		local_player.destroy();
		Paged_man.destroy();
		Faune_man.destroy();
		Amnesic_Faune_man.destroy();

		Map.unload();
		ldbg.disable();

		IsBackGroundInitialised = false;
	}
}









































































































