/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PHYSXRAFT // Pour �viter la redondance des d�clarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PHYSXRAFT

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class PhysX_Raft // Majuscule obligatoire pour �viter les conflits avec les biblio standards
{
public:

	int last_mesh,waterheight;

	Critter::Body* list[5000];

    PhysX_Raft(void);
    ~PhysX_Raft(void);
    void add(Critter::Body* new_raft);
	void remove(Critter::Body* raft);
	void flush(void);
	void update(float);
};

extern PhysX_Raft PhysXfloater;

#endif // CLASS_Raft


