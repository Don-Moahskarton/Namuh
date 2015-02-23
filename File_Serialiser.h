/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_FILE_SERIALISER // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_FILE_SERIALISER

#include "Namuh_Main_H.h"

class FileSerialiser
{
	public:

		FileSerialiser(void);
		~FileSerialiser(void);

		void WriteFile(std::vector<NamedDataPool*> Data, string FileName);

	private:
		inline std::string getOpenBaliseStr(string BaliseName);
		inline std::string getCloseBaliseStr(string BaliseName);
		inline std::string getAffectStr(string DataName);
		inline std::string SerialisePool(NamedDataPool* Pool, string lineprefix = "");

};
extern FileSerialiser File_Serialiser;

#endif // CLASS_PERSO


