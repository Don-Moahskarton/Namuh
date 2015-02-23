#include "Namuh_Main_H.h"
/*

ldbg.send("File "+ string(__FILE__) +" passed line n°"+ StringConverter::toString(__LINE__)); // Debug =================================

*/
using namespace Ogre;

// Définition du joueur local
std::map<RakNet::RakNetGUID, Perso*> PlayerList; // et de la liste des joueurs
std::map<RakNet::RakNetGUID, KeyState> KeyPlayer; // et de leurs claviers

// Définition des membres statiques
RakNet::RakNetGUID Perso::cam_sync = RakNet::UNASSIGNED_RAKNET_GUID;


raknet_mgr raknet_man;
Perso local_player(raknet_man.local_GUID);

// Code des méthodes de la classe
Perso::Perso(RakNet::RakNetGUID ID)
{
	ldbg.send("Declaration of Perso ID_"+raknet_man.GUIDtoString(ID));
	initialised = false;
	id = ID;
	PlayerList[ID] = this;
	KeyState new_keystate;
	KeyPlayer[ID] =  new_keystate;
	Ruler.PointList[ID] = 10;
	raknet_man.AdPlayer.push_back(raknet_man.peer->GetSystemAddressFromGuid(ID));
}
Perso::~Perso(void)
{
	ldbg.send("Destruction of Perso ID_"+raknet_man.GUIDtoString(id));
	PlayerList.erase(id);
	KeyPlayer.erase(id);
	Ruler.PointList.erase(id);
}

void Perso::initialise(void) // Déplacement
{

	if(!initialised)
	{
		// Initialisation Ogre
		Node = PhysX_man.mRenderSystem->createNode(); // Neud racine du personnage
		Mesh_n = Node->createChildSceneNode(); // Neud du perso
		CamNode = Node->createChildSceneNode(); // neud de la caméra
		if(cam_sync == id)
		{
			sys0.cam->detachFromParent();
			CamNode->attachObject(sys0.cam);
		}
		else
		{
			mesh = sys0.mgr->createEntity("Sapiens" + StringConverter::toString(sys0.timer.getMicroseconds()),"Sapiens_01.mesh"); //maillage du perso
			Mesh_n = Node->createChildSceneNode(); // Neud du perso
			Mesh_n->attachObject(mesh);
			Mesh_n->setOrientation(Quaternion(Ogre::Radian(3.14f),Ogre::Vector3(0.0f,1.0f,0.0f)));
			Mesh_n->setPosition(0.0f,-1.18f,0.25f);
			Mesh_n->scale(0.78f,0.78f,0.78f);
		}

		// Initialisation PhysX
		PhysXDescription = new Critter::AnimatedCharacterDescription;
		PhysXDescription->mCollisionMask = 1;
		PhysXDescription->mSlopeLimit = 0.7f;
		PhysXDescription->mMaxGroundSpeed = 10.0f;
		PhysXDescription->mStepOffset = 0.5;
		PhysXDescription->mShape = NxOgre::SimpleCapsule(1.6f, 0.31f);
		PhysXDescription->setJumpVelocityFromMaxHeight(NxOgre::Constants::MEAN_EARTH_GRAVITY.y, 0.5f);
		character = PhysX_man.mRenderSystem->createAnimatedCharacter(Vector3(0,0,0), Ogre::Radian(0.0f), Node, *PhysXDescription); // Bug after PhysX destroy
		CharCtrl = reinterpret_cast<NxOgre::CharacterController*>(character);
		InputHelper = new Critter::CharacterInputHelper();
		Last_movetime = -1;
		LinearVelocity = Ogre::Vector3::ZERO;
		Acceleration = Ogre::Vector3::ZERO;
		UpdateOrientationOrPosition = false;

		// Initialisation des Armes
			// Lance Pierre
			Lance_Pierre_Actuel.initialise(id);
			Inventory.push_back(&Lance_Pierre_Actuel);
			Lance_Pierre_Actuel.WeaponNode->setVisible(true);
			// Arc
			Arc_En_Bois_Actuel.initialise(id);
			Inventory.push_back(&Arc_En_Bois_Actuel);
			Arc_En_Bois_Actuel.WeaponNode->setVisible(false);
			// Javelot
			Javelot_Actuel.initialise(id);
			Inventory.push_back(&Javelot_Actuel);
			Javelot_Actuel.WeaponNode->setVisible(false);
		Current_Weapon = Inventory[0]; // On commence avec le lance pierre en main
		Last_Damage_Owner_id = 	RakNet::UNASSIGNED_RAKNET_GUID;

		// Initialistion des caractéristiques
		movespeed = 0.3f;
		MotionInput = Ogre::Vector4::ZERO;
		OrientationInput = Ogre::Vector3::ZERO;
		JumpInput = false;
		FireInput = false;
		initialised = true;
	}
	else
		ldbg.send("Tried to intialise player n°" + raknet_man.GUIDtoString(id) + " ,which is already initialised. Skipping...");
}

void Perso::spawn(Vector3 temp, bool urgent) // Repositionnement
{
	// RAZ de la santé, et on redevient temporairement invincible (1sec)
	Spawning_invicinbility_time = sys0.timer.getMilliseconds()-2000;
	invincible = true;
	vie = 100;
	if(cam_sync == id)GuiSandboxMan.Affichage_vie.set(vie);
	

	// RAZ armes
	Last_Damage_Owner_id = RakNet::UNASSIGNED_RAKNET_GUID;


	//RAZ cinématique
	pos = temp;
	lastpos = temp;
	LastLinearVelocity = Ogre::Vector3::ZERO;
	LinearVelocity = Ogre::Vector3::ZERO;
	Acceleration = Ogre::Vector3::ZERO;
	MotionInput = Ogre::Vector4::ZERO;
	OrientationInput = Ogre::Vector3::ZERO;
	JumpInput = false;
	FireInput = false;


	// téléport
	InputHelper->reset();
	character->setInput(*InputHelper);
	character->setPosition(temp);
	CharCtrl->setPosition(temp);
	Node->setPosition(temp);
	character->setUsesGravity(false);
	if(urgent) PhysX_man.update(10000); // plein de temps
	UpdateOrientationOrPosition = false;
	update(100000,Mvt_DisableFallDamage); // encore plein de temps
	UpdateOrientationOrPosition = false;
	Last_movetime = -1;
}

void Perso::update(float TimeEllapsed, Perso_Mvt_flag MovingMode, bool ManualFire )
{
	if(!initialised) // Skip the whole updatre if not initialised
	{
		ldbg.send("Tried to update player n°" + raknet_man.GUIDtoString(id) + " ,which isn't initialised. Skipping...",false);
	}
	else
	{
		//Gestion de l'invincibilité
		if(invincible && Spawning_invicinbility_time + 3000 < sys0.timer.getMilliseconds())
		{
			invincible = false;
			ldbg.send("Tu n'es plus invincible, prout !",false); 
		}

		// Déplacement volant ?
		if(MovingMode == Mvt_Fly)
		{
			if(Current_Weapon != NULL && Current_Weapon->WeaponNode != NULL)
				Current_Weapon->WeaponNode->setVisible(false);

			// Compute dispalcement
			Ogre::Vector3 mTranslateVector = Ogre::Vector3::ZERO;
			if(MotionInput.x != 0) mTranslateVector.z -= 1;
			if(MotionInput.y != 0) mTranslateVector.z += 1;
			if(MotionInput.z != 0) mTranslateVector.x -= 1;
			if(MotionInput.w != 0) mTranslateVector.x += 1;
			mTranslateVector.y = -1*Ogre::Math::Sin(rot.getPitch().valueRadians())*mTranslateVector.z;
			mTranslateVector.normalise();
			mTranslateVector *= movespeed;
		
			// Apply displacement
			character->setUsesGravity(false);
			Node->getSceneNode()->translate(Node->getSceneNode()->getLocalAxes(),mTranslateVector);
			pos = Node->getPosition();

			character->setPosition(pos);
			CharCtrl->setPosition(pos);
			Node->setPosition(pos);
		}
		else		
		{
			if(pos.y > Hydrax_man.sea_level)
				character->setUsesGravity(true);
			else
				character->setUsesGravity(false);

			Current_Weapon->WeaponNode->setVisible(true);
		}

		InputHelper->reset();
		if(UpdateOrientationOrPosition) // Les deux sont impossible a faire simultanément a cause de InputHelper->input.turning_mode et que character->setInput ne peut prendre qu'un Input helper
										// Du coup on fait 2 update a la fois (et 2 PhysX update, mais physX est supra léger)
		{
			// Réeoriente le personnage
			this->rotate(OrientationInput); 
			OrientationInput = Ogre::Vector3::ZERO;
		}

		else
		{
			// Déplace le personnage
			this->move(MotionInput, JumpInput, MovingMode);
			MotionInput = Ogre::Vector4::ZERO;
			JumpInput = false;

			// Calculs cinématiques rustiques
			pos = Node->getPosition();
			Ogre::Real		dt = sys0.timer.getMilliseconds()-Last_movetime; // faire disparaitre les allocation realtime une fois que tout sera débuggué
			if(dt == 0.0f)dt = 0.001f; // Evite de diviser par 0, au cas ou

			Ogre::Vector3	dl = pos-lastpos,
							dv = LinearVelocity-LastLinearVelocity;
			LinearVelocity	 = 1000.0f*dl/dt;
			Acceleration	 = dv/dt;

			Last_movetime = sys0.timer.getMilliseconds();
			lastpos = pos;
			LastLinearVelocity = LinearVelocity;

			// Dégats de chute
			if(!invincible && MovingMode != Mvt_DisableFallDamage && Acceleration.length() > 425 && (raknet_man.isServer || !raknet_man.initialised))
			{
				Last_Damage_Owner_id = RakNet::UNASSIGNED_RAKNET_GUID;
				//damage((Acceleration.length()-425)/2);
			}
		}
		UpdateOrientationOrPosition = !UpdateOrientationOrPosition;

		// Le tir
		if(FireInput && !ManualFire) this->fire();
		FireInput = false;

		// Gestion de l'au-dela
		if(vie <= 0)
		{
			if(Last_Damage_Owner_id != RakNet::UNASSIGNED_RAKNET_GUID)Ruler.NoticeFrag(id,Last_Damage_Owner_id);
			else Ruler.NoticeScuicide(id);
			spawn(Vector3FromName(Map.MapDataPool,"Spawn_Position"));
			ldbg.send("Mort du perso d'id " + raknet_man.GUIDtoString(id) + ", due a l'id : " + raknet_man.GUIDtoString(Last_Damage_Owner_id) ,false);
		}
	}
}

void Perso::move(Ogre::Vector4 KeysWeights, bool Jump, Perso_Mvt_flag Mvt_Flag) // Déplacement
{
	if(Mvt_Flag != Mvt_Fly)
	{
		// Compute dispalcement
		InputHelper->input.turning_mode  = Critter::Enums::CharacterTurningMode_SideStep;
		if(KeysWeights.y != 0)InputHelper->forwardFractional(KeysWeights.y); // ces if sont la car faire un backwardFractional(0.0f) invalide le forwardFractional
		if(KeysWeights.x != 0)InputHelper->backwardFractional(KeysWeights.x); 
		if(KeysWeights.z != 0)InputHelper->leftFractional(KeysWeights.z);
		if(KeysWeights.w != 0)InputHelper->rightFractional(KeysWeights.w);
		if(Jump)InputHelper->up(Jump);

		// Apply displacement
		character->setInput(*InputHelper);
		//InputHelper->reset();

		if(!character->getUsesGravity())	// si on nage
		{
			Node->getSceneNode()->translate(Node->getSceneNode()->getLocalAxes(),Vector3(0.0f,Ogre::Math::Sin(rot.getPitch()) * KeysWeights.y * 10,0.0f));
			pos = Node->getPosition();

			character->setPosition(pos);
			CharCtrl->setPosition(pos);
			Node->setPosition(pos);
		}
	}
}

void Perso::setpos(Ogre::Vector3 temp) // Repositionnement
{
	//RAZ cinématique
	pos = temp;
	lastpos = temp;
	LastLinearVelocity = Ogre::Vector3::ZERO;
	LinearVelocity = Ogre::Vector3::ZERO;
	Acceleration = Ogre::Vector3::ZERO;
	MotionInput = Ogre::Vector4::ZERO;
	OrientationInput = Ogre::Vector3::ZERO;
	JumpInput = false;
	FireInput = false;


	// téléport
	InputHelper->reset();
	character->setInput(*InputHelper);
	character->setPosition(temp);
	CharCtrl->setPosition(temp);
	Node->setPosition(temp);
	UpdateOrientationOrPosition = false;
	update(100000,Mvt_DisableFallDamage); // encore plein de temps
	UpdateOrientationOrPosition = false;
	Last_movetime = -1;
}

void Perso::sethealth(int new_vie) // Modifier la vie
{
	vie = new_vie;
	if(vie<0)vie=0;
	if(Perso::cam_sync == id) GuiSandboxMan.Affichage_vie.set(vie);
}

void Perso::damage(int amount) // Infliger des dégats au joueur
{
	if(!invincible && (raknet_man.isServer || !raknet_man.initialised))
	{
		ldbg.send("degat subit par l'id " + raknet_man.GUIDtoString(id) + ", due a l'id : " + raknet_man.GUIDtoString(Last_Damage_Owner_id) + "montant : " + StringConverter::toString(amount) ,false);
		sethealth(vie - amount);
	}
}

void Perso::rotate(Ogre::Vector3 requested_rotation) // Rotation Relative
{
	last_rot_vect = rot_vect;

	// Ogre handle vertical orientation
	rot	= rot * Quaternion(Degree(requested_rotation.y), Ogre::Vector3::UNIT_X);
    CamNode->setOrientation(rot);

	// PhysX handle horisontal orientation
	InputHelper->input.turning_mode  = Critter::Enums::CharacterTurningMode_Add;
	InputHelper->input.left_right = requested_rotation.x;
	character->setInput(*InputHelper);
	//InputHelper->reset();

	//Calculs cinématiques
	Ogre::Real		 dt = sys0.timer.getMilliseconds()-Last_rotatetime;
	Ogre::Vector3	 da = rot_vect-last_rot_vect;
	rotative_velocity	= requested_rotation/dt;
	Last_rotatetime = sys0.timer.getMilliseconds();
}

void Perso::setOrientation(Ogre::Vector3 requested_rotation) // Rotation Absolue (to merge ?)
{
	// Ogre handle vertical orientation
	rot	= Quaternion(Degree(requested_rotation.y), Ogre::Vector3::UNIT_X);
    CamNode->setOrientation(rot);
		
	// PhysX handle horisontal orientation
	InputHelper->input.turning_mode  = Critter::Enums::CharacterTurningMode_Set;
	InputHelper->input.left_right = requested_rotation.x;
	character->setInput(*InputHelper);
	//InputHelper->reset();
}

void Perso::fire()
{
	Current_Weapon->fire();
	if(raknet_man.isServer)
	{
		RakNet::BitStream bsOut;		
		bsOut.Write((RakNet::MessageID)StC_TIR);
		bsOut.Write(id); // le -1 vient du fait que le local_player du serv ne compte pas dans la liste des joueurs
		raknet_man.peer->Send(&bsOut,IMMEDIATE_PRIORITY,RELIABLE,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);	
	}
}

void Perso::destroy()
{
	//Mesh_n->removeAndDestroyAllChildren();
	//CamNode->removeAllChildren();
	//Arme_n->removeAndDestroyAllChildren();
	//Node->removeEntity();
	initialised = false;
}


void Perso::SwichWeapon(uchar NewWeaponIndex,bool NoticeNewtork)
{
	// Montre la nouvelle arme
	Current_Weapon->WeaponNode->setVisible(false);
	Current_Weapon = Inventory[NewWeaponIndex];
	Current_Weapon->WeaponNode->setVisible(true);

	//  si on est client d'une partie, on en averti le serveur
	if(raknet_man.mClient.initialised && NoticeNewtork) 
	{
		ldbg.send("Sending network notification for weapon switch : "+ StringConverter::toString(char(NewWeaponIndex)));
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)CtS_WEAPONSWITCH);
		bsOut.Write(char(NewWeaponIndex));
		raknet_man.peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_SEQUENCED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
	}
}

































































