/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PROJECTILE	// Pour �viter la redondance des d�clarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PROJECTILE

#include "Namuh_Main_H.h"


class Perso; // D�claration des perso en avant garde

class Projectile // Majuscule obligatoire pour �viter les conflits avec les biblio standards
{
	public:
		Perso *Owner;
		Critter::Body *body;
		bool CanDamage;
		unsigned long	Date_Fired; 

    Projectile(Perso*); // Constructeur additionel pour avoir directement l'owner d'attribu�
	Projectile(void);
    ~Projectile(void);
};

#endif // CLASS_PERSO


