#include "Namuh_Main_H.h"

using namespace Ogre;
bool OpenUPNP(RakNet::RakPeerInterface *rakPeer, RakNet::SystemAddress serverAddress);


raknet_Client::raknet_Client(void)
{
	peer = raknet_man.peer;
	connected = false;
	initialised = false;
	needupdate = true;
}

void raknet_Client::Initialise(string IP,int port)
{	
	peer = raknet_man.peer;
	peer->Startup(1,&RakNet::SocketDescriptor(), 1);
	OpenUPNP(peer, RakNet::SystemAddress(&IP[0], port));
	ldbg.send("Initialising client on port : "+StringConverter::toString(port));
	ldbg.send("Trying to connect to : "+IP);
	peer->Connect(&IP[0], port, 0,0);
	initialised = true;
}

void raknet_Client::update(void)
{	
	// Ne demande d'update que quand la partie est prête
	if(local_player.initialised)
		this->SendDatas(); 

	// Traitement de paquets en entrée
	for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
		switch (packet->data[0])
		{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				ldbg.send("(NetI/O Processor) Remote client disconnected.");
			break;
			case ID_REMOTE_CONNECTION_LOST:
				ldbg.send("(NetI/O Processor) Remote client lost the connection.");
			break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				ldbg.send("(NetI/O Processor) Ana peut pas se connectationner au serveur :/");
			break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				ldbg.send("(NetI/O Processor) Remote client connected.");
			break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				connected = true;
				ldbg.send("(NetI/O Processor) Connected to server");
			}
			break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				ldbg.send("(NetI/O Processor) The server is full.");
			break;
			case ID_DISCONNECTION_NOTIFICATION:
			{
				connected = false;
				ldbg.send("(NetI/O Processor) Disconnected by server.");
			}
			break;
			case ID_CONNECTION_LOST:
			{
				connected = false;
				ldbg.send("(NetI/O Processor) Connection lost.");
			}
			break;
			case ID_REMOTE_DEBUG_MESSAGE:
				raknet_man.ProcessPacket_RemoteDebuggerMsg(packet);
			break;
			case StC_SYNC_SET_TIME:
				this->ProcessPacket_SyncClock(packet);
			break;
			case StC_GAME_MAP:
				this->ProcessPacket_MapName(packet);
			break;
			case StC_HEALTH:
				this->ProcessPacket_PlayerHealth(packet);
			break;
			case StC_PLAYER_POS:
				this->ProcessPacket_PlayerPosition(packet);
			break;
			case StC_PLAYER_ROT:
				this->ProcessPacket_PlayerOrientation(packet);
			break;
			case StC_SET_ID:
				this->ProcessPacket_SetID(packet);
			break;			
			case StC_TIR:
				this->ProcessPacket_Tir(packet);
			break;
			case StC_PLAYER_PTS:
				this->ProcessPacket_PlayerPts(packet);
			break;
			case StC_WEAPONSWITCH:
				this->ProcessPacket_WeaponSwitch(packet);
			break;
			case StC_GAME_END:
				Ruler.EndOfGame = true;
			break;
			case StC_PING:
				this->ProcessPacket_Ping(packet);
			break;
			default:
				ldbg.send("Message with identifier "+StringConverter::toString(packet->data[0])+" has arrived.\n" );
			break;
		}
}

void raknet_Client::ProcessPacket_MapName(RakNet::Packet* Datas)
{
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakString rs;
	bsIn.Read(rs);
	raknet_man.map = rs.C_String();
	ldbg.send("(NetI/O Processor) Recived map name : "+ raknet_man.map);
}

void raknet_Client::ProcessPacket_SetID(RakNet::Packet* Datas)
{
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::RakNetGUID newID;

	// construit autant de nouveau personnage qu'il y a d'autres clients dans la partie
	bsIn.Read(newID);
	while (newID != RakNet::UNASSIGNED_RAKNET_GUID) // Tant qu'on ne passe pas le marqueur de fin
	{
		ldbg.send("Recieved new player with ID_"+raknet_man.GUIDtoString(newID));
		Perso* newone = new Perso(newID);
		if (PhysX_man.initialised)	// partie can initalise them later if physX isn't avariable
			newone->initialise();		
		bsIn.Read(newID);
	}
}

void raknet_Client::ProcessPacket_PlayerPosition(RakNet::Packet* Datas)
{	
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	Vector3 pos;
	float x,y,z;
	RakNet::RakNetGUID ID;
	bsIn.Read(ID); 
	bsIn.Read(x);
	bsIn.Read(y);
	bsIn.Read(z);
	pos = Vector3(x,y,z);
	if(PlayerList.count(ID))
		PlayerList[ID]->setpos(pos);
	else
		ldbg.send("Recieved position (" + toOgStr(pos) + ") for unknown player ID_"+raknet_man.GUIDtoString(ID));
	needupdate = true;
}

void raknet_Client::ProcessPacket_PlayerOrientation(RakNet::Packet* Datas)
{
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	float angle,angle_vert;
	RakNet::RakNetGUID ID;
	bsIn.Read(ID);
	bsIn.Read(angle);
	bsIn.Read(angle_vert);
	if(PlayerList.count(ID))
		PlayerList[ID]->OrientationInput = Ogre::Vector3(angle			-PlayerList[ID]->Node->getOrientation().getYaw().valueDegrees(),
														 angle_vert		-PlayerList[ID]->rot.getPitch().valueDegrees(),
														 0);
	else
		ldbg.send("Recieved orientation  (" + toOgStr(angle) + ", " + toOgStr(angle_vert) + ")  for unknown player ID_"+raknet_man.GUIDtoString(ID));
	ldbg.send("Recieved orientation  (" + toOgStr(angle) + ", " + toOgStr(angle_vert) + ")  for player ID_"+raknet_man.GUIDtoString(ID),false);
	needupdate = true;
}

void raknet_Client::ProcessPacket_Tir(RakNet::Packet* Datas)
{
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	
	RakNet::RakNetGUID ID;
	bsIn.Read(ID);
	PlayerList[ID]->fire();
}

void raknet_Client::ProcessPacket_PlayerHealth(RakNet::Packet* Datas)
{
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	char vie;
	bsIn.Read(vie);
	local_player.sethealth(vie);
	needupdate = true;
}

void raknet_Client::ProcessPacket_PlayerPts(RakNet::Packet* Datas)
{
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	int pts;
	RakNet::RakNetGUID ID;
	bsIn.Read(ID);
	bsIn.Read(pts);
	Ruler.PointList[ID]=float(pts)/100.0f;
}

void raknet_Client::ProcessPacket_WeaponSwitch(RakNet::Packet* Datas)
{
	RakNet::BitStream bsIn(packet->data,packet->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	char NewWeapon_c;
	RakNet::RakNetGUID ID;
	bsIn.Read(ID);
	bsIn.Read(NewWeapon_c);
	PlayerList[ID]->Current_Weapon->WeaponNode->setVisible(false);
	PlayerList[ID]->Current_Weapon = PlayerList[ID]->Inventory[NewWeapon_c];
	PlayerList[ID]->Current_Weapon->WeaponNode->setVisible(true);
}

void raknet_Client::SendDatas(void)
{
	if(needupdate)
	{
		// Demande au serveur une MAJ
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)CtS_UPDATE_REQUEST);
		bsOut.Write((long double)*raknet_man.gametime);
		peer->Send(&bsOut,IMMEDIATE_PRIORITY,RELIABLE,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);

		// Envoi des infos clavier
		bsOut.Reset();
		bsOut.Write((RakNet::MessageID)CtS_KEYSTATE);
		bsOut.Write((long double)*raknet_man.gametime);
		bsOut.Write(bool(local_player.FireInput));
		bsOut.Write(bool(local_player.MotionInput.x > 0.0f));
		bsOut.Write(bool(local_player.MotionInput.y > 0.0f));
		bsOut.Write(bool(local_player.MotionInput.z > 0.0f));
		bsOut.Write(bool(local_player.MotionInput.w > 0.0f));
		bsOut.Write(bool(local_player.JumpInput));
		peer->Send(&bsOut,IMMEDIATE_PRIORITY,RELIABLE_SEQUENCED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);

		// Envoi de l'orientation du personnage
		bsOut.Reset();
		bsOut.Write((RakNet::MessageID)CtS_ORIENTATION);
		bsOut.Write(float(local_player.Node->getOrientation().getYaw().valueDegrees()));
		bsOut.Write(float(local_player.rot.getPitch().valueDegrees()));
		bsOut.Write(float(local_player.rotative_velocity.x));
		bsOut.Write(float(local_player.rotative_velocity.y));
		peer->Send(&bsOut,IMMEDIATE_PRIORITY,RELIABLE_SEQUENCED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
		
		needupdate = false;
	}
}

void raknet_Client::ProcessPacket_SyncClock(RakNet::Packet* Datas)
{
	// Recois l'état de l'horloge serveur
	long double delta;
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	bsIn.Read(*raknet_man.gametime);
	bsIn.Read(delta);
	*raknet_man.gametime += delta/2;

	// La rerenvois pour confirmation	
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)CtS_SYNC_TIMESTAMP);
	bsOut.Write(long double(*raknet_man.gametime));
	bsOut.Write(long double(delta));
	peer->Send(&bsOut,IMMEDIATE_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
}

void raknet_Client::ProcessPacket_Ping(RakNet::Packet* Datas)
{
	// renvois le paquet au serveur
	RakNet::BitStream bsIn(Datas->data,Datas->length,false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	long double pingdate;
	bsIn.Read(pingdate);

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)CtS_PING);
	bsOut.Write((long double)pingdate);
	peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
}

void raknet_Client::destroy(void)
{	

}
raknet_Client::~raknet_Client(void)
{
}

















































































































