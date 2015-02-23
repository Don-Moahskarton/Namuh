/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PHYSX_MANAGER// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PHYSX_MANAGER

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class PhysX_mgr // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
	public:

		bool	initialised,
				first_ini;
		
		NxOgre::Scene* mScene;
		Critter::RenderSystem* mRenderSystem;
		NxOgre::World* mWorld;
		NxOgre::SceneDescription scndesc;
		NxOgre::Mesh*	MenhirMesh;
		NxOgre::Mesh* Cailloux;

		NxOgre::SceneGeometry* MapHF;
		AnimationState* mLightAnimState;

		
		PhysX_mgr(void);
		~PhysX_mgr(void);
		void initialise(void);
		void update(float);
		void destroy(void);
};

// Déclaration du joueur local
extern PhysX_mgr PhysX_man;

#endif // CLASS_PERSO


