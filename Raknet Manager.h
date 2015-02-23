/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_RAKNET_MANAGER	// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_RAKNET_MANAGER

#include "Namuh_Main_H.h"

class Perso;
class raknet_Server ;
class raknet_Client;

// CtS : Client to Server
// StC : Server to Client

enum raknet_msg_identifier
{
	ID_REMOTE_DEBUG_MESSAGE				= ID_USER_PACKET_ENUM+1,

	// Server to CLient Packet ID :
	StC_GAME_MAP						= ID_USER_PACKET_ENUM+2,
	StC_SET_ID							= ID_USER_PACKET_ENUM+3,
	StC_PLAYER_POS						= ID_USER_PACKET_ENUM+4,
	StC_PLAYER_ROT						= ID_USER_PACKET_ENUM+5,
	StC_HEALTH							= ID_USER_PACKET_ENUM+6,
	StC_TIR								= ID_USER_PACKET_ENUM+7,
	StC_WEAPONSWITCH					= ID_USER_PACKET_ENUM+8,
	StC_PLAYER_PTS						= ID_USER_PACKET_ENUM+9,
	StC_GAME_END						= ID_USER_PACKET_ENUM+10,
	StC_SYNC_SET_TIME					= ID_USER_PACKET_ENUM+11,
	StC_PING							= ID_USER_PACKET_ENUM+12,

	// Client to Server Packet ID :
	CtS_UPDATE_REQUEST					= ID_USER_PACKET_ENUM+13,
	CtS_KEYSTATE						= ID_USER_PACKET_ENUM+14,
	CtS_ORIENTATION						= ID_USER_PACKET_ENUM+15,
	CtS_WEAPONSWITCH					= ID_USER_PACKET_ENUM+16,
	CtS_SYNC_TIMESTAMP					= ID_USER_PACKET_ENUM+17,
	CtS_PING							= ID_USER_PACKET_ENUM+18

};

// Classe représentante d'un clavier
class KeyState
{
	public :
		bool	up,
				down,
				left,
				right,
				jump,
				fire;
		long double timestamp;
		KeyState(void)
		{
			up = down = left = right = jump = fire = false;
			timestamp = 0;
		};
};

extern std::map<RakNet::RakNetGUID, KeyState> KeyPlayer;

class raknet_mgr // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
	public:
		long double* gametime;
		RakNet::RakPeerInterface *peer;
		bool isServer,initialised;
		RakNet::Packet *packet;
		std::vector<RakNet::SystemAddress> AdPlayer;
		raknet_Server mServer;
		raknet_Client mClient;
		string map;
		RakNet::RakNetGUID local_GUID;

	void InitialiseAsServer(long double* gametime, int port = 1993, int Maxplayer = 128);
	void InitialiseAsClient(long double* gametime, string  IpToConnect, int port = 1993);
	void destroy(void);
	void update(float);
	void ProcessPacket_RemoteDebuggerMsg(RakNet::Packet* Datas);
	string StringIDfromSystemAddress(RakNet::SystemAddress);
	string GUIDtoString(RakNet::RakNetGUID);
	raknet_mgr(void);
    ~raknet_mgr(void);
};

extern raknet_mgr raknet_man;
#endif // CLASS_PERSO


