/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PLAYER_PROFILE_MANAGER // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PLAYER_PROFILE_MANAGER

#include "Namuh_Main_H.h"

class PlayerProfile
{
	public:

		PlayerProfile(void);
		~PlayerProfile(void);

		string	Name;
		std::vector<string> FavoritesConstructions;
		string Default_IP;

		// Add all stats datas to be recorded here
};


class PlayerProfileManager
{
	public:

		std::vector<ParserBalise*> ProfileFileSyntax;
		string ProfilePath;
		bool isCurrentProfileValid;

		PlayerProfileManager(void);
		~PlayerProfileManager(void);

		std::vector<PlayerProfile*> LoadedProfiles;
		
		PlayerProfile* getCurrent(void);
		void setCurrent(PlayerProfile* NewProfile);
		void deleteCurrent(void);
		void CreateNew(string Name);
		PlayerProfile* Load(string ProfileFile);
		void SaveAs(string FileName, PlayerProfile* ProfileObject  = NULL);

	private:
		PlayerProfile* CurrentProfile;


};
extern PlayerProfileManager ProfileMgr;

#endif // CLASS_PERSO


