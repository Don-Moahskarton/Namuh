#include "Namuh_Main_H.h"

using namespace Ogre;
bool OpenUPNP(RakNet::RakPeerInterface *rakPeer, RakNet::SystemAddress serverAddress);

std::map<RakNet::RakNetGUID, ushort> PlayerPing;
extern std::map<RakNet::RakNetGUID, KeyState> KeyPlayer;

raknet_Server::raknet_Server(void)
{
	peer = raknet_man.peer;
	pingflag = 1;
}

void raknet_Server::Initialise(int port, int MaxClients)
{	
	peer = raknet_man.peer;
	int ConnecResult;
	ConnecResult = int(peer->Startup(MaxClients, &RakNet::SocketDescriptor(port,0), 1));
	if(ConnecResult != 1)
		ldbg.send("Cannot Start server, error code : "+StringConverter::toString(ConnecResult));
	else
		ldbg.send("Initialised server on port : "+StringConverter::toString(port)+", Maximum client allowed : "+StringConverter::toString(MaxClients)+".");
	peer->SetMaximumIncomingConnections(MaxClients);
	initialised = true;
}

void raknet_Server::update(void)
{	
	// Ping les clients tout les 3sec
	if(*raknet_man.gametime > pingflag*3000)
	{
		pingflag++;
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)StC_PING);
		bsOut.Write((long double)*raknet_man.gametime);
		peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
	}
	
	// Traitement de paquets en entrée
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
		switch (packet->data[0])
		{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				ldbg.send("(NetI/O Processor) Remote client ID_"+raknet_man.GUIDtoString(packet->guid)+" left");
			break;
			case ID_REMOTE_CONNECTION_LOST:
				ldbg.send("(NetI/O Processor) Remote client ID_"+raknet_man.GUIDtoString(packet->guid)+" lost the connection.");
			break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				ldbg.send("(NetI/O Processor) Remote client ID_"+raknet_man.GUIDtoString(packet->guid)+" connected.");
			break;
			case ID_NEW_INCOMING_CONNECTION:
				this->ProcessPacket_NewClient(packet);
			break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				ldbg.send("(NetI/O Processor) The server is full.");
			break;
			case ID_DISCONNECTION_NOTIFICATION:
				ldbg.send("(NetI/O Processor) Client ID_"+raknet_man.GUIDtoString(packet->guid)+" left");
			break;
			case ID_CONNECTION_LOST:
				ldbg.send("(NetI/O Processor) Client ID_"+raknet_man.GUIDtoString(packet->guid)+" lost the connection.");
			break;
			case ID_REMOTE_DEBUG_MESSAGE:
				raknet_man.ProcessPacket_RemoteDebuggerMsg(packet);
			break;
			case CtS_SYNC_TIMESTAMP:
				this->ProcessPacket_SyncClock(packet);
			break;
			case CtS_UPDATE_REQUEST:
				this->SendUpdate(packet->guid);
			break;
			case CtS_KEYSTATE:
				this->ProcessPacket_NewKeystate(packet);
			break;
			case CtS_ORIENTATION:
				this->ProcessPacket_NewOrientation(packet);
			break;
			case CtS_WEAPONSWITCH:
				this->ProcessPacket_ChangeWeapon(packet);
			break;
			case CtS_PING:
				this->ProcessPacket_Ping(packet);
			break;
			default:
				ldbg.send("(NetI/O Processor) Unknown packet with identifier "+StringConverter::toString(packet->data[0])+" recived from Client ID_"+raknet_man.GUIDtoString(packet->guid)+" : Skipping.\n" );
			break;
		}
}

void raknet_Server::ProcessPacket_NewClient(RakNet::Packet* Datas)
{		
	// Envoi au client les autres IDs présents
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)StC_SET_ID);
	for (std::map<RakNet::RakNetGUID, Perso*>::iterator IDIterator = PlayerList.begin();
		IDIterator !=PlayerList.end();
		IDIterator++)
		if(IDIterator->second->id != raknet_man.local_GUID)
			bsOut.Write(IDIterator->second->id); // recopie tout les GUID dans le paquet (sauf celui du srv)
	bsOut.Write(RakNet::UNASSIGNED_RAKNET_GUID); // Marqueur de fin
	peer->Send(&bsOut,MEDIUM_PRIORITY,RELIABLE,0,Datas->guid,false);

	// Envoi du nom de la map
	ldbg.send("(NetI/O Processor) Sending Map_name  (" + raknet_man.map + ") to this client");	
	bsOut.Reset();
	bsOut.Write((RakNet::MessageID)StC_GAME_MAP);
	RakNet::RakString rs;
	rs.Set(raknet_man.map.c_str());
	bsOut.Write(rs);
	peer->Send(&bsOut,MEDIUM_PRIORITY,RELIABLE,0,packet->guid,false);

	// Envoi du Timestamp
	bsOut.Reset();
	bsOut.Write((RakNet::MessageID)StC_SYNC_SET_TIME);
	bsOut.Write(long double(*raknet_man.gametime));
	bsOut.Write(long double(0));
	peer->Send(&bsOut,IMMEDIATE_PRIORITY,RELIABLE,0,packet->guid,false);

	// Spawn un nouveau perso
	Perso* NewPerso = new Perso(Datas->guid);
	NewPerso->initialise();
	NewPerso->spawn(Vector3FromName(Map.MapDataPool,"Spawn_Position")+Ogre::Vector3(Ogre::Math::RangeRandom(-5,5),0,Ogre::Math::RangeRandom(-5,5))); // Messy spawn point TODO : make team spawn area
	ldbg.send("(NetI/O Processor) Client ID_" +raknet_man.GUIDtoString(NewPerso->id) + " joined");


	// TODO : Make a system to ensure that this info is correctly transmited
	// Préviens les autres clients du nouveau joueur
	
	bsOut.Reset();
	bsOut.Write((RakNet::MessageID)StC_SET_ID);
	bsOut.Write(Datas->guid);
	bsOut.Write(RakNet::UNASSIGNED_RAKNET_GUID); // Marqueur de fin
	peer->Send(&bsOut,MEDIUM_PRIORITY,RELIABLE,0,Datas->guid,true);
	

}

void raknet_Server::SendUpdate(RakNet::RakNetGUID ClientID)
{
	RakNet::BitStream bsOut;
	float x,y,z;
	for (	std::map<RakNet::RakNetGUID, Perso*>::iterator PlayerIterator = PlayerList.begin();
		PlayerIterator != PlayerList.end();
		PlayerIterator++)
	{
		if(PlayerIterator->second->id != raknet_man.local_GUID)
		{
			// Envoi de la position des joueurs
			bsOut.Reset();
			x = float(PlayerIterator->second->pos.x);
			y = float(PlayerIterator->second->pos.y);
			z = float(PlayerIterator->second->pos.z);
			bsOut.Write((RakNet::MessageID)StC_PLAYER_POS);
			bsOut.Write((RakNet::RakNetGUID)PlayerIterator->second->id);
			bsOut.Write(x);
			bsOut.Write(y);
			bsOut.Write(z);
			peer->Send(&bsOut,IMMEDIATE_PRIORITY,RELIABLE_ORDERED,0,ClientID,false);

			// envoit l'orientation des joueurs , sauf l'orientation du joueur qui demande l'update
			// Par contre ce joueur doit recevoir sa nouvelle valeur de vie
			if(ClientID != PlayerIterator->first)
			{
				bsOut.Reset();
				bsOut.Write((RakNet::MessageID)StC_PLAYER_ROT);
				bsOut.Write(PlayerIterator->first);
				bsOut.Write(float(PlayerIterator->second->Node->getOrientation().getYaw().valueDegrees()));
				bsOut.Write(float(PlayerIterator->second->rot.getPitch().valueDegrees()));
				peer->Send(&bsOut,IMMEDIATE_PRIORITY,RELIABLE_ORDERED,0,ClientID,false);
			}
			else
			{
				// TODO : send only on change
				bsOut.Reset();
				bsOut.Write((RakNet::MessageID)StC_HEALTH);
				bsOut.Write(char(PlayerIterator->second->vie));
				peer->Send(&bsOut,IMMEDIATE_PRIORITY,RELIABLE_ORDERED,0,ClientID,false);
			}
		}
	}
}


void raknet_Server::ProcessPacket_NewKeystate(RakNet::Packet* Datas)
{
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	
	RakNet::RakNetGUID ID = Datas->guid;

	std::map<RakNet::RakNetGUID, Perso*>::iterator PlayerIterator = PlayerList.find(ID);
	if(PlayerIterator != PlayerList.end())
	{
		// Met a jour le clavier virtuel du joueur
		bsIn.Read(KeyPlayer[PlayerIterator->first].timestamp);
		bsIn.Read(KeyPlayer[PlayerIterator->first].fire);
		bsIn.Read(KeyPlayer[PlayerIterator->first].up);
		bsIn.Read(KeyPlayer[PlayerIterator->first].down);
		bsIn.Read(KeyPlayer[PlayerIterator->first].left);
		bsIn.Read(KeyPlayer[PlayerIterator->first].right);
		bsIn.Read(KeyPlayer[PlayerIterator->first].jump);
	}
	else
		ldbg.send("(NetI/O Processor) Recived new keystates from an unknown player : packet skipped (ID_" + raknet_man.GUIDtoString(ID) + ")");
}

void raknet_Server::ProcessPacket_NewOrientation(RakNet::Packet* Datas)
{
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakNetGUID ID = Datas->guid;

	float rot,rot_vert,rotative_velocity_x,rotative_velocity_y;
	bsIn.Read(rot); // angle horisontal
	bsIn.Read(rot_vert); // angle vertical
	bsIn.Read(rotative_velocity_x); // vitesse de rotation horisontale
	bsIn.Read(rotative_velocity_y); // vitesse de rotation verticale
	std::map<RakNet::RakNetGUID, Perso*>::iterator PlayerIterator = PlayerList.find(ID);
	if(PlayerIterator != PlayerList.end())
	{
		PlayerList[PlayerIterator->first]->OrientationInput = Ogre::Vector3(rot			-PlayerList[PlayerIterator->first]->Node->getOrientation().getYaw().valueDegrees(),
																			rot_vert	-PlayerList[PlayerIterator->first]->rot.getPitch().valueDegrees(),
																			0);
		PlayerList[PlayerIterator->first]->rotative_velocity = Ogre::Vector3(rotative_velocity_x,rotative_velocity_y,0);
	}
	else
		ldbg.send("(NetI/O Processor) Recived orientation from an unknown player : packet skipped (ID_" + raknet_man.GUIDtoString(ID) + ")");
}

void raknet_Server::ProcessPacket_ChangeWeapon(RakNet::Packet* Datas)
{
	// Récupère les infos du paquet
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakNetGUID ID = Datas->guid;
	long double timestamp;
	bsIn.Read(timestamp);

	char NewWeapon_c;
	bsIn.Read(NewWeapon_c);
	std::map<RakNet::RakNetGUID, Perso*>::iterator PlayerIterator = PlayerList.find(ID);
	if(PlayerIterator != PlayerList.end())
	{
		// Change d'arme
		PlayerIterator->second->Current_Weapon->WeaponNode->setVisible(false);
		PlayerIterator->second->Current_Weapon = PlayerIterator->second->Inventory[NewWeapon_c];
		PlayerIterator->second->Current_Weapon->WeaponNode->setVisible(true);

		// Préviens les autres clients que ClientPlayerIndex a changé d'arme
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)StC_WEAPONSWITCH);
		bsOut.Write(PlayerIterator->first);
		bsOut.Write(char(NewWeapon_c));
		peer->Send(&bsOut,LOW_PRIORITY,RELIABLE_ORDERED,0,ID,true);
		bsOut.Reset();
	}
	else
		ldbg.send("(NetI/O Processor) Recived a notification to change a weapon from an unknown player : packet skipped (ID_" + raknet_man.GUIDtoString(ID) + ")");
}

void raknet_Server::ProcessPacket_Ping(RakNet::Packet* Datas)
{
	long double pingdate;
	RakNet::RakNetGUID ID = Datas->guid;
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bsIn.Read(pingdate);
	PlayerPing[ID] = (ushort)(*raknet_man.gametime - pingdate);
	ldbg.send("(ID_" + raknet_man.GUIDtoString(Datas->guid) + "). ping : " + StringConverter::toString(int(PlayerPing[ID])));
}

void raknet_Server::ProcessPacket_SyncClock(RakNet::Packet* Datas)
{
	// Recois l'état de l'horloge client
	long double ClientClock, ClientDelta, delta;
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bsIn.Read(ClientClock);
	bsIn.Read(ClientDelta);
	delta = *raknet_man.gametime - ClientClock;
	//ldbg.send("(ID_" + raknet_man.GUIDtoString(Datas->guid) + "). delta : " + StringConverter::toString(float(delta)));

	// renvois un ajustement si nécécaire
	if(delta < 1  || ClientDelta < 1 || ClientDelta <= delta)
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)StC_SYNC_SET_TIME);
		bsOut.Write(long double(*raknet_man.gametime));
		bsOut.Write(long double(delta));
		peer->Send(&bsOut,IMMEDIATE_PRIORITY,RELIABLE,0,packet->guid,false);
	}
}

void raknet_Server::destroy(void)
{	

}
raknet_Server::~raknet_Server(void)
{
}












































































































