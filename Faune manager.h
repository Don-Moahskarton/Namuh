/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_FAUNE_MANAGER// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_FAUNE_MANAGER

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class Faune_Page // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{	
	public:

		bool isVisible;
		Ogre::Vector3 Center;
		std::vector<Faune_Page*> SubPages;
		std::vector<Faune_Entity*> ContainedEntities;
		int		MinXBorder, 
				MaxXBorder,
				MinZBorder,
				MaxZBorder,
				size;
		//Parent Page
		Faune_Page*		Parent;

		// Chainage octuple
		Faune_Page		*Right,
						*TopRight,
						*Top,
						*TopLeft,
						*Left,
						*BottomLeft,
						*Bottom,
						*BottomRight;
	
		
		Ogre::Entity* FlagEntity;
		Ogre::SceneNode* FlagNode;

		Faune_Page(void);
		~Faune_Page(void);
		void initialise(int MinXBorder,int MaxXBorder ,int MinZBorder ,int MaxZBorder);
		void update(float);
		void setVisible(bool);
		void destroy(void);
};

class Faune_mgr // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
	public:

		Ogre::uint debugInfoTimestamp; // Timestamp when last debug info was thrown


		float DisplayDistance;
		Faune_Page RootPage;
		Faune_Page *GuessedPlayerPage;
		std::vector<Faune_Page*> UpdateList;
		char posID;
		bool initialised;
	
    Faune_mgr(void);
    ~Faune_mgr(void);
    void initialise(void);
	void update(float TimeEllapsed, Ogre::Vector3 CenterOfUpdate);
	Faune_Page* LocatePlayer(Ogre::Vector3 PositionToLocalise);
    void ChainageOctupleFirstPass(Faune_Page*);
    void ChainageOctupleSecondPass(Faune_Page*);
	int getIndexInParentSubPages(Faune_Page*);
    void destroy(void);
};

extern Faune_mgr Faune_man;


#endif // CLASS_PERSO


