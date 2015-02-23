/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_FAUNE_ENTITY// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_FAUNE_ENTITY

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class Faune_Page;

class Faune_Entity // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{	
	public:
		Ogre::Vector3	position,
						linearVelocity;
		std::list<Ogre::Vector3> linearVelocityHistory;
		Ogre::Entity* AnimalEntity;
		Ogre::SceneNode* AnimalNode;
		Faune_Page* Container;

		



		Faune_Entity(void);
		~Faune_Entity(void);
		void initialise(Faune_Page* Container, Ogre::Vector3 position);
		void update(float);
		void destroy(void);
};

class Amnesic_Faune_Entity // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
	public:
		Ogre::Vector3 position;
		Ogre::Entity* AnimalEntity;
		Ogre::SceneNode* AnimalNode;

		Amnesic_Faune_Entity(void);
		~Amnesic_Faune_Entity(void);
		void initialise(Ogre::Vector2);
		void update(float);
		void destroy(void);
};
#endif // CLASS_PERSO


