/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_JAVELOT	// Pour �viter la redondance des d�clarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_JAVELOT

#include "Namuh_Main_H.h"

class Javelot : public Carried_Weapon // Majuscule obligatoire pour �viter les conflits avec les biblio standards
{ // Cette classe ne d�crit qu'une arme d�tenue par un joueru, pas un pickup
	public:
		Critter::BodyDescription JavelotDescription;
		int NextProjectileID;

    Javelot(void);
    ~Javelot(void);
	void initialise(RakNet::RakNetGUID);
	void fire();
	//void SetAmmo(int);
};

#endif // CLASS_PERSO


