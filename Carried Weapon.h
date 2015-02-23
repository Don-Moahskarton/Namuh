/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_CARRIED_WEAPON	// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_CARRIED_WEAPON

#include "Namuh_Main_H.h"

enum Weapon
{
	LancePierre = 1,
	ArcEnBois = 2,
	Javelot_ = 3
};

class Carried_Weapon // Classe abstaite ne pouvant etre instanciée
{ // Cette classe ne décrit qu'une arme détenue par un joueru, pas un pickup
	public:
		Perso*	Owner;
		int		MaxAmmo, // FAire des énumérations sur les valeurs négatives, gnres pour des munition infinies
				AmmoCount, // Nombre de Balles en réserve
				Cooldown; // Temps mini en ms entre chaque tir
		ulong	LastfiredTimer;  // Timer du dernier tir

		Ogre::Entity *WeaponMesh; // Maillage de l'arme
		SceneNode *WeaponNode; // Neud du Maillage de l'arme

		std::vector<Projectile> ProjectileList;
		virtual void fire() =0; // Polymorphisme : Les pointeurs de type Carried_Weapon pointant par example sur un Lance pierre pourront appeller des redéfinitions de fire (Alors que le type du pointeur ne le laissait pas supposer)
};

#endif // CLASS_PERSO


