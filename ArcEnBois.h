/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_ARCENBOIS	// Pour �viter la redondance des d�clarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_ARCENBOIS

#include "Namuh_Main_H.h"

class Arc_En_Bois : public Carried_Weapon // Majuscule obligatoire pour �viter les conflits avec les biblio standards
{ // Cette classe ne d�crit qu'une arme d�tenue par un joueru, pas un pickup
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


