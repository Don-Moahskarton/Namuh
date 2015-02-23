/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_SAVEABLE // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_SAVEABLE

#include "Namuh_Main_H.h"
class Saveable		// Contains necessary interface to save inherited instances through the parser.
{
	public:

		ParserBalise syntax;

		Saveable()
		{
			syntax.Name = "SaveableDefaultName";
		}
		
		static Saveable MakeFromDataPool(NamedDataPool&)
		{
			ldbg.send ("(Saveable Interface) Called MakeFromDataPool without overriding it ! )");
		}
			

		// virtual NamedDataPool* Serialize();


};

#endif // CLASS_SAVEABLE

