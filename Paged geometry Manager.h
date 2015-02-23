/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PAGED_GEOMETRY_MANAGER// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PAGED_GEOMETRY_MANAGER

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class Paged_geometry_mgr // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
	std::vector<PagedGeometry*> Geom_Pager;

	Ogre::Radian yaw;
	Ogre::Real scale;
	GrassLoader *Vegetation_image_loader;
	TreeLoader2D *Vegetation_object_loader;
	std::vector<GrassLayer*> imageLayerlst;
	Vector3 position;
	std::vector<NxOgre::SceneGeometry*> physXCreatedGeometry;

	std::vector<std::vector<Ogre::Entity*>> Entity_list;
	std::vector<std::vector<NxOgre::Mesh*>> PhysX_Mesh_list;

	public:

		bool initialised;


		Paged_geometry_mgr(void);
		~Paged_geometry_mgr(void);
		void initialise(bool PhysX = true);
		void update(void);
		void destroy(void);
};

// Déclaration du joueur local
extern Paged_geometry_mgr Paged_man;

#endif // CLASS_PERSO


