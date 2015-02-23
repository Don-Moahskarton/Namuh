/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_ARCENBOIS	// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_ARCENBOIS

#include "Namuh_Main_H.h"

class Arc_En_Bois : public Carried_Weapon // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{ // Cette classe ne décrit qu'une arme détenue par un joueru, pas un pickup
	public:
		Critter::BodyDescription FlecheDescription;
		int NextProjectileID;

    Arc_En_Bois(void);
    ~Arc_En_Bois(void);
	void initialise(RakNet::RakNetGUID);
	void fire();
	//void SetAmmo(int);
};

#endif // CLASS_PERSO


