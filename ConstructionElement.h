/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_CONSTRUCTION_ELEMENT // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_CONSTRUCTION_ELEMENT

#include "Namuh_Main_H.h"

class ConstructionElement	: public Saveable	// Abstraction of Log, rock, hide, etc...
{
	protected:
		bool rigidBodyBasedFlag;

	public:

		virtual NamedDataPool* Serialize(){ldbg.send("Error : called ConstructionElement::Serialize. Construction element name = " + name); return NULL;}
		bool isRigidBodyBased ();
		string name;

};

#endif // CLASS_CONSTRUCTION_ELEMENT

