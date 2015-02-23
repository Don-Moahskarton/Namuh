/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_DEATHMATCH_RULES	// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_DEATHMATCH_RULES

#include "Namuh_Main_H.h"

class DeathMatchRules // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
	public:
		std::map<RakNet::RakNetGUID, float> PointList;
		bool EndOfGame;

	void initialise(void);
	void destroy(void);
	void update(void);
	void NoticeFrag(RakNet::RakNetGUID Victim,RakNet::RakNetGUID Criminal);
	void NoticeScuicide(RakNet::RakNetGUID Noob);
	DeathMatchRules(void);
    ~DeathMatchRules(void);
};

extern DeathMatchRules Ruler;
#endif // CLASS_PERSO


