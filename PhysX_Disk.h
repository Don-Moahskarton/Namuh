/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PHYSXDISK// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PHYSXDISK

#include "Namuh_Main_H.h"
class DiskDescription									// Parameters needed to make a disk
{
	public:

		DiskDescription();
		bool Cook();									// Verify params, and tells if correct

		string materialName;							// material used. Blank field will be modified to defaut disk material
		string name;									// cute little name	

		Construction*	container;						// The disk have to be bounded to a construction. Can we accept NULL ?
		Ogre::Quaternion orientation;					// how is orientated this disk
		
		float	radius,
				thickness,
				volume;									// volume will be computed in respect with height, so don't feed this field


		Critter::BodyDescription physXFields;			// Parameters that deals withs physics
		Ogre::Vector3 position;							// where is that disk ?	

		bool	isCooked;								// params aren't valid if the description isn't cooked
};


class Disk : public ConstructionRigidElement // wrapper for physX hide so we can add some attributes.
{
	public:
		Disk(DiskDescription *); 

		DiskDescription descr;

		NamedDataPool* Serialize();

		static Disk* MakeFromDataPool(NamedDataPool&);

};

#endif // CLASS_PHYSXHIDE

