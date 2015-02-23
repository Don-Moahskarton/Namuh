/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_CONSTRUCTION_MANAGER // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_CONSTRUCTION_MANAGER

#include "Namuh_Main_H.h"

class CstrMan
{

	private:
		friend void PhysX_mgr::initialise(void);
		std::vector<ParserBalise*> Syntax;

	public:

		NxOgre::Mesh* RocherMesh;
		std::vector<Construction*> LoadedConstructions;


		CstrMan(void);
		~CstrMan(void);
		
		Construction* CreateNew();
		Construction* Load(string File, Ogre::Vector3 Position, Ogre::Quaternion Orientation = Ogre::Quaternion::IDENTITY);
		void SaveAs(Construction* Cstr, string FileName);
		void Destroy(Construction*);

		Trunk* CreateTrunk( TrunkDescription*);


		Menhir* CreateMenhir( MenhirDescription*);

		
		void Create_triangle(	Construction* Container, 
								Ogre::Vector3 Position, 
								Ogre::Quaternion Orientation, 
								float diametre, 
								float longueur, 
								string name = "");

		
		Disk*  Create_disk( DiskDescription* param);
		

		NxOgre::Cloth*  Create_hide(	Construction* container, 
										Ogre::Vector3 firstCornerPos, 
										Critter::Body* firstCornerToLink, 
										Ogre::Vector3 secondCornerPos, 
										Critter::Body* secondCornerToLink, 
										Ogre::Vector3 thirdCornerPos, 
										Critter::Body* thirdCornerToLink, 
										Ogre::Vector3 fourthCornerPos, 
										Critter::Body* fourthCornerToLink,
										bool isWing = false,
										bool isBounce = false,
										string name = "");
};
extern CstrMan CstrMgr;

#endif // CLASS_PERSO


