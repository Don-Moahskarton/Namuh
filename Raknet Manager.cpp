#include "Namuh_Main_H.h"

using namespace Ogre;


bool OpenUPNP(RakNet::RakPeerInterface *rakPeer, RakNet::SystemAddress serverAddress);


raknet_mgr::raknet_mgr(void)
{
	map = "";
	AdPlayer.push_back(RakNet::UNASSIGNED_SYSTEM_ADDRESS); //Le player 0 (ie local_player du serveur) n'a pas d'adresse
	peer = RakNet::RakPeerInterface::GetInstance();
	local_GUID = peer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS); // mon id local
	ldbg.send("initialising Raknet_mgr with ID_"+this->GUIDtoString(local_GUID));
	// pour les bots; on pourra leur donner un id avec Get64BitUniqueRandomNumber
}

void raknet_mgr::InitialiseAsServer(long double* ptr_gametime, int port, int MaxClients)
{	
	isServer = true;
	mServer.Initialise(port,MaxClients);
	initialised = true;
	gametime = ptr_gametime;
}

void raknet_mgr::InitialiseAsClient(long double* ptr_gametime, string IP, int port)
{	
	isServer = false;
	mClient.Initialise(IP,port);
	initialised = true;
	gametime = ptr_gametime;
}

void raknet_mgr::update(float inv_FPS)
{	
	if(isServer) mServer.update();
	else mClient.update();
}

void raknet_mgr::destroy(void)
{	

}
raknet_mgr::~raknet_mgr(void)
{
		RakNet::RakPeerInterface::DestroyInstance(peer);
}

string raknet_mgr::StringIDfromSystemAddress(RakNet::SystemAddress Addr)
{
	char* newstring = new char[64];
	(peer->GetGuidFromSystemAddress(Addr)).ToString(newstring);
	string ID = newstring;
	return ID;
}

string raknet_mgr::GUIDtoString(RakNet::RakNetGUID ID)
{
	char* newstring = new char[64];
	ID.ToString(newstring);
	std::stringstream outstream;
	string out = "";
	outstream << *newstring;
	outstream >> out;
	return out;
}

void raknet_mgr::ProcessPacket_RemoteDebuggerMsg(RakNet::Packet* Datas)
{
	RakNet::RakString rs;
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bsIn.Read(rs);
	ldbg.send(rs.C_String());
}

bool OpenUPNP(RakNet::RakPeerInterface *rakPeer, RakNet::SystemAddress serverAddress)
{	
	using namespace RakNet;
	struct UPNPDev * devlist = 0;
	devlist = upnpDiscover(1000, 0, 0, 0); // was 2000
	if (devlist)
	{
		char lanaddr[64];	/* my ip address on the LAN */
		struct UPNPUrls urls;
		struct IGDdatas data;
		if (UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr))==1)
		{
			DataStructures::List< RakNetSmartPtr< RakNetSocket> > sockets;
			rakPeer->GetSockets(sockets);

			char iport[32];
			Itoa(sockets[0]->boundAddress.GetPort(),iport,10);
			char eport[32];
			Itoa(rakPeer->GetExternalID(serverAddress).GetPort(),eport,10);

			int r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype, eport, iport, lanaddr, 0, "UDP", 0);
			if(r!=UPNPCOMMAND_SUCCESS)
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}















































































































