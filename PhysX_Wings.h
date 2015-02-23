/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PHYSXWING // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PHYSXWING

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class PhysX_Wings_mgr // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
public:

	std::vector<
				pair<	NxOgre::Cloth*,							// the wing itself
						pair<	float,							// its surface
								std::vector<Critter::Body*>		// linked bodies
							>	
					>
				>															list;

    PhysX_Wings_mgr(void);
    ~PhysX_Wings_mgr(void);

    void add(NxOgre::Cloth* newWing, float surface, std::vector<Critter::Body*>* attachedBodies);
	void remove(NxOgre::Cloth* wing);
	void flush(void);
	void update(float);
};

extern PhysX_Wings_mgr PhysX_Wings_man;

#endif // CLASS_WIng


