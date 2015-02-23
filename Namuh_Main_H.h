/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test main Header File -------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef MainH // Pour éviter la redondance des includes (ce fichier est a inclure dans las classes
#define MainH

/// Includes Système immuables
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <tchar.h>
	#include <stdio.h>
	#include <sstream>
	#include <string>
    #include <deque>
    #include <math.h>
	#include <algorithm>

    #include "Ogre\Ogre.h"       //Ogre
	#include "Ogre\OgreCompositorLogic.h"
	#include "Ogre\OgreCompositorInstance.h"
	#include "Ogre\Terrain\OgreTerrain.h" // La gestion des terrains
	#include "Ogre\Terrain\OgreTerrainMaterialGeneratorA.h" // La gestion des terrains v(bis)
	#include <Ogre\OgreTextAreaOverlayElement.h>

    #include <OIS\OIS.h>    //le systeme de gestion du clavier et de la souris

	#include "NxOgre\NxOgre.h" // Plugin PhysX
	#include "NxOgre\critter.h" // Plugin PhysX (bis)
	#include "NxOgre\CritterStable.h"	// Critter's internals

	#include <CEGUI/CEGUI.h> // Interface de fenètres
	#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h> // Interface de fenètres (bis)
	#include "CEGUI/ScriptingModules/LuaScriptModule/CEGUILua.h"

	#include "Hydrax/Hydrax.h"
	#include "Hydrax/Noise/FFT/FFT.h"
	#include "Hydrax/Noise/Perlin/Perlin.h"
	#include "Hydrax/Noise/Real/Real.h"
	#include "Hydrax/Modules/ProjectedGrid/ProjectedGrid.h"
	#include "Hydrax/Modules/RadialGrid/RadialGrid.h"
	#include "Hydrax/Modules/SimpleGrid/SimpleGrid.h"
	
	#include <SkyX/SkyX.h> // Plugin Ciel

	#include "PagedGeometry/PagedGeometry.h" // Plugin végétation
	#include "PagedGeometry/BatchPage.h" // Plugin végétation
	#include "PagedGeometry/WindBatchPage.h" // Plugin végétation
	#include "PagedGeometry/TreeLoader3D.h" // Plugin végétation
	#include "PagedGeometry/TreeLoader2D.h" // Plugin végétation
	#include "PagedGeometry/ImpostorPage.h" // Plugin végétation
	#include "PagedGeometry/GrassLoader.h" // Plugin végétation

	#include "RakNet/RakPeerInterface.h" // Réseau
	#include "RakNet/MessageIdentifiers.h" // Réseau
	#include "RakNet/BitStream.h" // Réseau
	#include "RakNet/RakNetTypes.h"  // MessageID  // Réseau
	#include "Raknet\Itoa.h"
	#include "Raknet\miniupnpc.h"
	#include "Raknet\upnpcommands.h"
	#include "Raknet\upnperrors.h"


	#include "SSAO\PFXSSAO.h"


    // Ca c'est pour activer les definitions d'Ogre et du C++ standard
    using namespace std;
    using namespace Ogre;
    using namespace NxOgre;
	using namespace Forests;
    class ExitListener:public FrameListener{OIS::Keyboard *mKeyboard;};// Ca c'est la classe qui permet le rendu (oui, je l'ai un peu résumé)
	
	#define toOgStr Ogre::StringConverter::toString

	// Enable selective render loop : some elements are capped in ther refresh rate so the overall game is more responsive.
	// Flickering have been detected on some system. While it is preferable to enable it, it is still possible to disable it.
	#define SelectiveRendering false

	/// Nos Includes a nous !
	// (on n'utilise pas de using namespace dans un vrais .h (c'est a dire pas un comme celui la qui ne fait qu'en regrouper d'autres))
	class Perso;
	extern Perso local_player;

	extern Ogre::String Debug_Text;
	#include "Raknet Server.h"
	#include "Raknet Client.h"
	#include "Raknet Manager.h"
	extern raknet_mgr raknet_man;
	extern std::map<RakNet::RakNetGUID, KeyState> KeyPlayer;
	extern std::map<RakNet::RakNetGUID, ushort> PlayerPing;
	#include "SmartMath.h"
	#include "GUI.h"
	#include "GraphAxis.h"
	#include "Graph.h"
	#include "Live Debugger.h"
	extern LiveDebugger ldbg;
	#include "Map Parser.h"
	extern MapParser Map_Parser;
	#include "Saveable.h"
	#include "ConstructionElement.h"
	#include "ConstructionRigidElement.h"
	#include "PhysX Manager.h"
	#include "Construction.h"
	#include "PhysX_raft.h"
	extern PhysX_Raft PhysXfloater;
	#include "PhysX_Wings.h"
	extern PhysX_Wings_mgr PhysXWingsMan;
	#include "PhysX_Trampoline.h"
//	extern PhysX_Trampoline_mgr PhysXTrampolineMan;
	#include "PhysX_Waterfall.h"
	#include "PhysX_Trunk.h"
	#include "PhysX_hide.h"
	#include "PhysX_Menhir.h"
	#include "PhysX_Disk.h"
	#include "Ogre initialisation.h"    //sys0
	extern Ogre_ini sys0;
	#include "Mesh Generator.h"

	#include "File_Serialiser.h"
	extern FileSerialiser File_Serialiser;
	#include "Map Loader.h"
	extern Map_loader Map;
	#include "Atmosphere manager.h"
	#include "Hydrax Manager.h"
	extern Hydrax_mgr Hydrax_man;
	extern PhysX_mgr PhysX_man;
	extern Atm_mgr Atm_man;

	#include "Menu circulaire.h"
	#include "Vie HUD.h"
	#include "Player Profile Manager.h"
	extern PlayerProfileManager ProfileMgr;
	#include "GUI_MainMenu.h"
	#include "GUI_FPS.h"
	#include "GUI_Sandbox.h"
	extern Gui_sandbox_manager GuiSandboxMan;
	#include "HDR.h"
	#include "System.h"                 //sys1
	#include "Projectile.h"
	#include "Carried Weapon.h"
	#include "Lance Pierre.h"
	#include "ArcEnBois.h"
	#include "Javelot.h"
	#include "Personnage.h"
	extern Perso local_player;
	#include "Faune Entity.h"
	#include "Amnesic Faune manager.h"
	#include "Faune manager.h"
	#include "Projectile CallBacks.h"
	extern std::vector<ProjCallback> ProjCallbackList;
	extern std::map<RakNet::RakNetGUID, Perso*> PlayerList;
	#include "DeathMatchRules.h"
	extern DeathMatchRules Ruler; 
	#include "Construction Manager.h"
	extern CstrMan CstrMgr;
	#include "Sandbox_Constructeur.h"
	#include "Paged geometry Manager.h"
	#include "Partie.h"



	float terrain_height(float,float, void *);
	void SkyXupdate(Ogre::Real);
	NxOgre::SceneGeometry* loadTerrainGeometry(const Ogre::String&, float* , Ogre::uint16, Ogre::Real, Ogre::Real , Ogre::Real , const Ogre::Vector3&);
	uint FakeDistance2D (int,int,int,int);
	BOOL DeleteDirectory(const TCHAR* sPath);


#endif




















































































































