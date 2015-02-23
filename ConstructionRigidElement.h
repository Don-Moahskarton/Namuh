/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_CONSTRUCTION_RIGID_ELEMENT // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_CONSTRUCTION_RIGID_ELEMENT

#include "Namuh_Main_H.h"

class ConstructionRigidElement	: public ConstructionElement	// Abstraction of Log, rock, etc... They have to inherit of a PhysX rigidbody
{
	public:

		Critter::Body* physXBody;

		std::vector<Critter::Body*> linkedBodies;		// list of bodies that are attached to this element
		
};

#endif // CLASS_CONSTRUCTION_ELEMENT

