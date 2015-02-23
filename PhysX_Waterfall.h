/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PHYSXWATERFALL // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PHYSXWATERFALL

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class PhysX_Waterfall // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
public:

	//int last_mesh,waterheight;

	//Critter::Body* list[5000];

    PhysX_Waterfall(void);
    ~PhysX_Waterfall(void);
    void make(void);  //(Critter::Body* new_raft);
	//void remove(Critter::Body* raft);
	//void update(int deltatime,Hydrax::Hydrax* mHydrax);
};
/*
class PhysX_Waterfall_Desc // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
public:

	//int last_mesh,waterheight;

	//Critter::Body* list[5000];
	NxOgre::FluidDescription Fluid_desc;
	NxOgre::FluidEmitterDescription Emiter_desc;

	PhysX_Waterfall_Desc(void);
    ~PhysX_Waterfall_Desc(void);
    void make(void);  //(Critter::Body* new_raft);
	//void remove(Critter::Body* raft);
	//void update(int deltatime,Hydrax::Hydrax* mHydrax);
};
*/
// Déclaration du joueur local
extern PhysX_Waterfall PhysX_Waterfall_man;

#endif // CLASS_PHYSXWATERFALL


