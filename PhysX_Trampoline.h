/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PHYSXTRAMPOLINE // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PHYSXTRAMPOLINE

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class PhysX_Trampoline_mgr // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
public:
	
	std::vector<NxOgre::Cloth*> list;

    PhysX_Trampoline_mgr(void);
    ~PhysX_Trampoline_mgr(void);
    void add(NxOgre::Cloth* newWing);
	void remove(NxOgre::Cloth* wing);
	void flush(void);
	void update(float);
};

extern PhysX_Trampoline_mgr PhysX_Trampoline_man;

#endif // PhysX_Trampoline_mgr


