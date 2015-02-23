/*

ldbg.send("File "+ string(__FILE__) +" passed line n°"+ StringConverter::toString(__LINE__)); // Debug =================================

*/
#include "Namuh_Main_H.h"

using namespace Ogre;
Gui_sandbox_manager GuiSandboxMan; // Un jour faudra le mettre en tant que membre de partie

// Code des méthodes de la classe
Partie::Partie(PartieArg args)
{
	Net_Partie_Status = args.Net_Status;
	isSandBox = args.isSandbox;
	Perso::cam_sync = raknet_man.peer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS); // On met la camera dans le joueur local
	bouclecount = 0;
	lastDebugTextUpdateTimeStamp = 0;
	lastHydraxUpdateTimeStamp = 0;
	lastHydraxRTTUpdateTimeStamp = 0;

	gametime = 0;
	inv_FPS = -1;
	string MapName = args.Mapfile;
	if(ldbg.initialised) // enlève le ldbg si déja initalisé ailleurs.
		ldbg.disable();


	if(isSandBox)
		GuiSandboxMan.Initialise();
	else
		GuiFPSMan.Initialise();
	if(Net_Partie_Status == Partie_Server)
	{
		raknet_man.InitialiseAsServer(&gametime);
		raknet_man.map = MapName;
	}
	if(Net_Partie_Status == Partie_Client)
	{
		ldbg.send("Connecting to " + args.IPofSrv);
		raknet_man.InitialiseAsClient(&gametime, args.IPofSrv);
		ldbg.send("Connexion initialised, waiting for server datas.");
		// Attente de réponse du serveur, pour savoir quelle map charger
		float timestamp = sys0.timer.getMilliseconds();
		while(raknet_man.map == "")
		{
			raknet_man.update(1/60);
			if(timestamp + 30000 < sys0.timer.getMilliseconds())
			{
				
				ldbg.send("Connexion failed, will load default map.");
				raknet_man.map = "Maps/Chutes Bresiliennes/Chutes Bresiliennes.map";
			}
		}
		if(timestamp + 30000 < sys0.timer.getMilliseconds())
				ldbg.send("Connexion sucessful, map recieved.");
		MapName = raknet_man.map;
		ldbg.send("Loading map : " + MapName);
	}

	Map.load(MapName);
	ldbg.send("Terrain loaded sucessfully");
	Hydrax_man.initialise();
	if(Net_Partie_Status != Partie_Server)
	{
		Atm_man.initialise();
		ldbg.send("Atmophere manager loaded sucessfully");
	}
	PhysX_man.initialise();
	
	// Perso ini
	for (	std::map<RakNet::RakNetGUID, Perso*>::iterator PlayerIterator = PlayerList.begin();
			PlayerIterator != PlayerList.end();
			PlayerIterator++)
			PlayerIterator->second->initialise(); 
	ldbg.send("PhysX loaded sucessfully");

	if(isSandBox)
	{
		Constructeur.intitalise();
		ldbg.send("Sandbox_constructeur initialised sucessfully");
	}
	ldbg.send("First part loaded sucessfully");
	if(Net_Partie_Status != Partie_Server)
	{
		Map.terrain->dirtyLightmap(); //a faire au moins une fois quand meme
		//sys0.RefractionRenderTarget->update(); // Pareil
	}
	Map.terrain->update();
	for (	std::map<RakNet::RakNetGUID, Perso*>::iterator PlayerIterator = PlayerList.begin();
			PlayerIterator != PlayerList.end();
			PlayerIterator++)
	{
		ldbg.send("Initialising/spawning player ID_"+raknet_man.GUIDtoString(PlayerIterator->first));

		PlayerIterator->second->initialise();
		PlayerIterator->second->spawn(Vector3FromName(Map.MapDataPool,"Spawn_Position")+Ogre::Vector3(Ogre::Math::RangeRandom(-5,5),0,Ogre::Math::RangeRandom(-5,5)),true); // Messy spawn point TODO : make team spawn area
	}
	
	if(Net_Partie_Status == Partie_Server)
		local_player.OrientationInput = Ogre::Vector3(0,90-local_player.rot.getPitch().valueDegrees(),0); // Le serveur regarde en l'air, gain énorme de FPS
	ldbg.send("loading Paged geometry ");
	//Paged_man.initialise(Net_Partie_Status != Partie_Client); // normalement c'est ca (authoritative srv)
	Paged_man.initialise(false); // pour test
	ldbg.send("Paged geometry loaded sucessfully");
	if(Net_Partie_Status != Partie_Server)
	{
		ldbg.send("Loading Amnesic Faune "); 
		Amnesic_Faune_man.initialise(); 
		ldbg.send("Amnesic Faune Loaded sucessfully");
	}
	ldbg.send("Loading Faune "); Faune_man.initialise(); ldbg.send("Faune Loaded sucessfully");
	if(!isSandBox)Ruler.initialise();

	ldbg.Flows["FPS"] = &inv_FPS;
	ldbg.Graphs["FPS"].YDatas["FPS"].setRange_CenterLastValue(true,0.05f);
	ldbg.Graphs["FPS"].YDatas["FPS"].AutoKeepOrigin = false;
	ldbg.graph("FPS","FPS");

	ldbg.Flows["posX"] = &(PlayerList.begin()->second->pos.x);
	ldbg.Flows["posZ"] = &(PlayerList.begin()->second->pos.z);
	ldbg.Graphs["miniMap"].XFlowConnexions["posZ"] = "posZ";
	ldbg.Graphs["miniMap"].YFlowConnexions["posX"] = "posX";
	ldbg.Graphs["miniMap"].posY = 0.025f + (ldbg.Graphs["miniMap"].sizeY +0.005f) * (ldbg.Graphs.size()-1);

	ldbg.Flows["LightDir"] = &(Atm_man.lightDir.y);
	ldbg.Flows["PosY"] = &(PlayerList.begin()->second->pos.y);
	ldbg.graph("PosY","PosY");
	ldbg.send("Loading static geometry"); 
	/*

	TODO : Implement contruction rotation

	Entity* Grotte = sys0.mgr->createEntity("Grotte","Grotte.mesh");
	SceneNode* Grotte_n = sys0.mgr->getRootSceneNode()->createChildSceneNode();
	Grotte_n->attachObject(Grotte);
	Grotte_n->setPosition(100,44,100);

	
	CstrMgr.Load("Estrade Menu principal.nmh",Ogre::Vector3(115,45,115)); ////////////===============
	
	CstrMgr.LoadedConstructions.back()->Rotate(Ogre::Quaternion(Ogre::Radian(3.14/2),Ogre::Vector3(0,1,0)));  ////////////===============
	*/

}


void Partie::update(void)
{
	sys1.Watch_LoopStart = sys0.timer.getMilliseconds();
	//ldbg.UpdateGrantedGraphes();											// Slow tool, use it only when required
	inv_FPS = 1/sys1.FPS;	
	if(isSandBox)
	{
		if(lastDebugTextUpdateTimeStamp + 500 < sys1.Watch_LoopStart)
		{
			GuiSandboxMan.UpdateDebugtext("Constructeur.Cursor3D_Type : " + toOgStr(Constructeur.Cursor3D_Type));
			lastDebugTextUpdateTimeStamp = sys1.Watch_LoopStart;
		}
		GuiSandboxMan.Update(inv_FPS);
		GuiSandboxMan.UpdateSandbox(inv_FPS);
		Constructeur.update();
	}
	else
	{
		GuiFPSMan.UpdateStats();
		GuiFPSMan.Update(inv_FPS);
		GuiFPSMan.UpdateFPS_UI(inv_FPS);
		Ruler.update();
	}
	
	if(Net_Partie_Status != Partie_Locale) raknet_man.update(inv_FPS);

	// Perso updation : movement
	for (	std::map<RakNet::RakNetGUID, Perso*>::iterator PlayerIterator = PlayerList.begin();
			PlayerIterator != PlayerList.end();
			PlayerIterator++)
		if(isSandBox && GuiSandboxMan.cameravolanteswitch) // Fly si sandbox
			PlayerIterator->second->update(inv_FPS,Mvt_Fly,Net_Partie_Status == Partie_Client); // Manual Fire only for client
		else
			PlayerIterator->second->update(inv_FPS,Mvt_Normal,Net_Partie_Status == Partie_Client); // Manual Fire only for client
	
	PhysX_man.update(inv_FPS/2);

	// Perso updation : orienation
	for (	std::map<RakNet::RakNetGUID, Perso*>::iterator PlayerIterator = PlayerList.begin();
			PlayerIterator != PlayerList.end();
			PlayerIterator++)
		if(isSandBox && GuiSandboxMan.cameravolanteswitch) // Fly si sandbox
			PlayerIterator->second->update(inv_FPS,Mvt_Fly,Net_Partie_Status == Partie_Client); // Manual Fire only for client
		else
			PlayerIterator->second->update(inv_FPS,Mvt_Normal,Net_Partie_Status == Partie_Client); // Manual Fire only for client
	PhysX_man.update(inv_FPS/2);

	Faune_man.update(inv_FPS,local_player.pos);

	if(Net_Partie_Status != Partie_Server)
	{
		Amnesic_Faune_man.update(inv_FPS,Ogre::Vector2(local_player.pos.x,local_player.pos.z));
		Atm_man.update(inv_FPS);
		Map.terrain->update();
		//Map.terrain->dirtyLightmap(); // a déplacer pour n'etre éffectué que quand
		Paged_man.update();
	}

		
	if(sys0.kb_releasing(OIS::KC_M))
		CstrMgr.LoadedConstructions.back()->Rotate(Ogre::Quaternion(Ogre::Radian(3.141/2),Ogre::Vector3::UNIT_Y));
	

	// ============================================================================================================================

	// TODO Merge the selective Render

	/// Render routine (detached to enable separate control of render targets, like the hydrax one)
	if (SelectiveRendering)
	{
		sys1.update(inv_FPS, false);// update system
	
		if(Net_Partie_Status != Partie_Server && lastHydraxUpdateTimeStamp + 50 < sys1.Watch_LoopStart) // Hydrax update caped to 10 FPS
		{
			Hydrax_man.update(0.05f);			
			lastHydraxUpdateTimeStamp = sys0.timer.getMilliseconds();
		}
		if(Net_Partie_Status != Partie_Server && lastHydraxRTTUpdateTimeStamp + 500 < sys1.Watch_LoopStart) // Hydrax update caped to 10 FPS
		{
			if(Hydrax_man.initialised)
				Hydrax_man.mHydrax->updateOnlyRTT(0.0f);
			lastHydraxRTTUpdateTimeStamp = sys0.timer.getMilliseconds();
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
	else
	{
		Hydrax_man.update(0.05f);
		Hydrax_man.mHydrax->updateOnlyRTT(0.0f);	
		sys1.update(inv_FPS, true);// update system

	}
	/// End of render routine
	// ============================================================================================================================
	

		

	sys1.Watch_LoopEnd = sys0.timer.getMilliseconds();
	gametime += sys1.Watch_LoopEnd - sys1.Watch_LoopStart;
}


Partie::~Partie(void)
{
	ldbg.send("Destroying Ruler");
	Ruler.destroy();
	ldbg.send("Destroying Partie");
	GuiSandboxMan.Destroy();
//	Hydrax_man.destroy();
	Atm_man.destroy();
	// Flush constructions
	while (CstrMgr.LoadedConstructions.size())
	{
		CstrMgr.Destroy(CstrMgr.LoadedConstructions.back());
		CstrMgr.LoadedConstructions.pop_back();
	}

	PhysX_man.destroy();
	//if(isSandBox)Constructeur.destroy(); // TODO : implement it
	local_player.destroy();
	Paged_man.destroy();
	Faune_man.destroy();
	Amnesic_Faune_man.destroy();

	Map.unload();
	ldbg.send("Partie Destroyed");
	ldbg.disable();
}









































































