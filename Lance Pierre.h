/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_LANCE_PIERRE	// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_LANCE_PIERRE

#include "Namuh_Main_H.h"

class Lance_Pierre : public Carried_Weapon // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{ // Cette classe ne décrit qu'une arme détenue par un joueru, pas un pickup
	public:
		Critter::BodyDescription PierreDescription;
		int NextProjectileID;

    Lance_Pierre(void);
    ~Lance_Pierre(void);
	void initialise(RakNet::RakNetGUID);
	void fire();
	//void SetAmmo(int);
};

#endif // CLASS_PERSO


