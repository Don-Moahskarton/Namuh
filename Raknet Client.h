/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_RAKNET_CLIENT	// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_RAKNET_CLIENT

#include "Namuh_Main_H.h"

class Perso;
class raknet_mgr;
enum raknet_msg_identifier;
class KeyState;
extern std::map<RakNet::RakNetGUID, KeyState> KeyPlayer;

// CtS : Client to Server
// StC : Server to Client

class raknet_Client // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
	void SendDatas(void);
	void ProcessPacket_MapName(RakNet::Packet* Datas);
	void ProcessPacket_SetID(RakNet::Packet* Datas);
	void ProcessPacket_PlayerPosition(RakNet::Packet* Datas);
	void ProcessPacket_PlayerOrientation(RakNet::Packet* Datas);
	void ProcessPacket_PlayerHealth(RakNet::Packet* Datas);
	void ProcessPacket_Tir(RakNet::Packet* Datas);
	void ProcessPacket_WeaponSwitch(RakNet::Packet* Datas);
	void ProcessPacket_PlayerPts(RakNet::Packet* Datas);
	void ProcessPacket_Ping(RakNet::Packet* Datas);
	void ProcessPacket_SyncClock(RakNet::Packet* Datas);



	public:
		RakNet::RakPeerInterface *peer;
		bool initialised,connected,needupdate;
		RakNet::Packet *packet;

		void Initialise(string IpToConnect,int port = 1993);
		void destroy(void);
		void update(void);
		
		raknet_Client(void);
		~raknet_Client(void);
};
#endif // CLASS_PERSO


