/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PHYSXTRUNK // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PHYSXTRUNK

#include "Namuh_Main_H.h"
class TrunkDescription		// Parameters needed to make a trunk
{
	public:

		TrunkDescription();
		bool Cook();									// Verify params, and tells if correct

		string materialName;								
		string name;									// cute little name

		Ogre::Vector3 position;							// where is that trunk ?
		

		Construction* container;						// The trunk have to be bounded to a construction. Can we accept NULL ?
		Ogre::Quaternion orientation;					// how is orientated this trunk

		Critter::BodyDescription physXFields;			// Parameters that deals withs physics
		
		float	radius,
				length,
				volume;									// volume will be computed in respect with radius and length, so don't feed this field

		bool	isCooked;								// params aren't valid if the description isn't cooked

};


class Trunk : public ConstructionRigidElement
{
	public:
		Trunk(TrunkDescription* trunkDescription); 

		TrunkDescription descr;
		
		NamedDataPool* Serialize();

		static Trunk* MakeFromDataPool(NamedDataPool& pool, Construction* container);

};

#endif // CLASS_PHYSXHIDE

