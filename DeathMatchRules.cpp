#include "Namuh_Main_H.h"

using namespace Ogre;

DeathMatchRules Ruler;

DeathMatchRules::DeathMatchRules(void)
{
}


void DeathMatchRules::initialise(void)
{	
	EndOfGame =false;
}

void DeathMatchRules::update(void)
{	
	if(raknet_man.isServer)
		for (	std::map<RakNet::RakNetGUID, Perso*>::iterator PlayerIterator = PlayerList.begin();
				PlayerIterator != PlayerList.end();
				PlayerIterator++)
			if(PointList[PlayerIterator->first] >= 20)
			{
				EndOfGame =true;
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)StC_GAME_END);
				raknet_man.peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
			}
}

void DeathMatchRules::NoticeFrag(RakNet::RakNetGUID id_Victim, RakNet::RakNetGUID id_Criminal)
{	
	PointList[id_Victim] -= 1;
	PointList[id_Criminal] += 2;
	
	if(raknet_man.initialised && raknet_man.isServer)
	{
		// Prévient les clients du changemrnt points
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)StC_PLAYER_PTS);
		bsOut.Write(id_Victim);
		bsOut.Write(int(PointList[id_Victim]*100));
		raknet_man.peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
		bsOut.Reset();
		bsOut.Write((RakNet::MessageID)StC_PLAYER_PTS);
		bsOut.Write(id_Criminal);
		bsOut.Write(int(PointList[id_Criminal]*100));
		raknet_man.peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
	}
}

void DeathMatchRules::NoticeScuicide(RakNet::RakNetGUID id_Noob)
{	
	PointList[id_Noob] -= 0.5;
	if(raknet_man.initialised && raknet_man.isServer)
	{
		// Prévient les clients du changemrnt points
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)StC_PLAYER_PTS);
		bsOut.Write(id_Noob);
		bsOut.Write(int(PointList[id_Noob]*100));
		raknet_man.peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
	}
}

void DeathMatchRules::destroy(void)
{	
	EndOfGame =false;
	PointList.erase(PointList.begin(),PointList.end());
}
DeathMatchRules::~DeathMatchRules(void)
{
}


























































































































