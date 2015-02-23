/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_RAKNET_SERVER	// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_RAKNET_SERVER

#include "Namuh_Main_H.h"

class Perso;
class raknet_mgr;
enum raknet_msg_identifier;
class KeyState;
extern std::map<RakNet::RakNetGUID, KeyState> KeyPlayer;
extern std::map<RakNet::RakNetGUID, ushort> PlayerPing;

// CtS : Client to Server
// StC : Server to Client

class raknet_Server 
{
	void ProcessPacket_NewClient(RakNet::Packet* Datas);
	void SendUpdate(RakNet::RakNetGUID ClientID);

	void ProcessPacket_NewKeystate(RakNet::Packet* Datas);
	void ProcessPacket_NewOrientation(RakNet::Packet* Datas);
	void ProcessPacket_ChangeWeapon(RakNet::Packet* Datas);
	void ProcessPacket_Ping(RakNet::Packet* Datas);
	void ProcessPacket_SyncClock(RakNet::Packet* Datas);

	uint pingflag;

	public:
		bool initialised;
		RakNet::RakPeerInterface *peer;
		RakNet::Packet* packet;

	void Initialise(int = 1993, int = 128);
	void destroy(void);
	void update(void);
	
	raknet_Server(void);
    ~raknet_Server(void);
};
#endif // CLASS_PERSO


