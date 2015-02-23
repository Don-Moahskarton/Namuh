/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_AMNESIC_FAUNE_MANAGER// Pour �viter la redondance des d�clarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_AMNESIC_FAUNE_MANAGER

#include "Namuh_Main_H.h"
#include <OgreVector3.h>


class Amnesic_Faune_mgr // Majuscule obligatoire pour �viter les conflits avec les biblio standards
{
// l'algo part d'une bonne id�e, mais a �t� entour� de m�canismes assez cochons...
	public:

		bool initialised;

		std::vector<std::vector<Amnesic_Faune_Entity>> Amnesic_Entities;
		uint	DisplayFakeDistance,
				MaxEntities,
				ActualEntityCount;
	
    Amnesic_Faune_mgr(void);
    ~Amnesic_Faune_mgr(void);
    void initialise(void);
	void update(float, Ogre::Vector2);
    void destroy(void);
	void GenerateNewAnimals(Ogre::Vector2);
};

extern Amnesic_Faune_mgr Amnesic_Faune_man;

#endif // CLASS_PERSO


