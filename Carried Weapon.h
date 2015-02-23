/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_CARRIED_WEAPON	// Pour �viter la redondance des d�clarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_CARRIED_WEAPON

#include "Namuh_Main_H.h"

enum Weapon
{
	LancePierre = 1,
	ArcEnBois = 2,
	Javelot_ = 3
};

class Carried_Weapon // Classe abstaite ne pouvant etre instanci�e
{ // Cette classe ne d�crit qu'une arme d�tenue par un joueru, pas un pickup
	public:
		Perso*	Owner;
		int		MaxAmmo, // FAire des �num�rations sur les valeurs n�gatives, gnres pour des munition infinies
				AmmoCount, // Nombre de Balles en r�serve
				Cooldown; // Temps mini en ms entre chaque tir
		ulong	LastfiredTimer;  // Timer du dernier tir

		Ogre::Entity *WeaponMesh; // Maillage de l'arme
		SceneNode *WeaponNode; // Neud du Maillage de l'arme

		std::vector<Projectile> ProjectileList;
		virtual void fire() =0; // Polymorphisme : Les pointeurs de type Carried_Weapon pointant par example sur un Lance pierre pourront appeller des red�finitions de fire (Alors que le type du pointeur ne le laissait pas supposer)
};

#endif // CLASS_PERSO


