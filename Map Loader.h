/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_MAP_LOADER// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_MAP_LOADER

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class Map_loader // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
	private:
		Ogre::uchar* PerlinNoise(Ogre::uint sizeXY, unsigned char octaves, float persistence = 0.45);
		Ogre::uchar* Derivation2D(Ogre::uint sizeXY, Ogre::uchar*);		

	public:
		TerrainGlobalOptions* terrainGlobals;
		Ogre::Terrain *terrain;
		Image img;
		Light* l;

		NamedDataPool MapDataPool;
		std::vector<ParserBalise*> MapSyntax;

		Map_loader(void);
		~Map_loader(void);
		void load(string);
		void unload(void);
};

// Déclaration de la map
extern Map_loader Map;


#endif // CLASS_PERSO

















































