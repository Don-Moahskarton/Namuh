/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PHYSXMENHIR // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PHYSXMENHIR

#include "Namuh_Main_H.h"
class MenhirDescription		// Parameters needed to make a hide
{
	public:

		MenhirDescription();
		bool Cook();		// Verify params, and tells if correct

		string name;
		string materialName;

		Critter::BodyDescription PhysX_descr;

		Construction*	container;

		Vector3 position;
		Ogre::Quaternion orientation;

		float	height,
				volume;		// volume will be computed in respect with height, so don't feed this field

		bool	isCooked;	// params aren't valid if the description isn't cooked
};


class Menhir : public ConstructionRigidElement // wrapper for physX hide so we can add some attributes.
{
	public:
		Menhir(MenhirDescription* description); 

		MenhirDescription descr;

		NamedDataPool* Serialize();

		static Menhir* MakeFromDataPool(NamedDataPool& pool, Construction* container);

};

#endif // CLASS_PHYSXHIDE

