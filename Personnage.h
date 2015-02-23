/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PERSO // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PERSO

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

enum Perso_Mvt_flag
{
	Mvt_Normal				= 0,
	Mvt_DisableFallDamage	= 1,
	Mvt_Fly					= 2

};

class Perso // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
public:

	Ogre::Entity *mesh; // Maillage du perso
	Critter::Node *Node; // Neud du Maillage
	SceneNode *Mesh_n, //Neud du maillage du perso
			  *CamNode;//Neud de la caméra
	Carried_Weapon *Current_Weapon;


	std::vector<Carried_Weapon*> Inventory; //Liste des Armes portés par le joueur
	Lance_Pierre Lance_Pierre_Actuel;
	Arc_En_Bois Arc_En_Bois_Actuel;
	Javelot Javelot_Actuel;

	NxOgre::CharacterController* CharCtrl; //Controleur du personage
	Critter::AnimatedCharacter* character; // Gestionnaire physique du personnage
	Critter::AnimatedCharacterDescription* PhysXDescription; // Description physique du personnage
	Critter::CharacterInputHelper* InputHelper; // Structure d'entrée des commandes clavier/souris

	bool initialised;
    RakNet::RakNetGUID id; // identificateur du perso
	int vie; // santé du perso
	ulong Spawning_invicinbility_time;// Temps dinvincibilité au respawn, en milisec
	unsigned long	Last_movetime; // horlorge lors du dernier appel de move
	unsigned long	Last_rotatetime; // horlorge lors du dernier appel de move
	RakNet::RakNetGUID Last_Damage_Owner_id;
	bool invincible; // God mode
    static RakNet::RakNetGUID cam_sync; // perso que la camera doit suivre
    Ogre::Vector3	pos,//Position, rotation(somme des vecteurs de rotations) et echelle
					rot_vect,
					last_rot_vect,
					rotative_velocity,
					scl,
					lastpos,
					LinearVelocity,
					LastLinearVelocity,
					Acceleration,
					OrientationInput;
	Ogre::Vector4	MotionInput;		// This is a virtual keyboard for WASD
	bool			JumpInput,			// IDEM : Space
					FireInput;			// Fire button 
	Ogre::Real		movespeed;
	bool			UpdateOrientationOrPosition;

	Ogre::Quaternion rot,rot_proj_hori; //rotation

    Perso(RakNet::RakNetGUID RakNetID);
    ~Perso(void);
	void initialise();
	void spawn(Vector3 = Vector3FromName(Map.MapDataPool,"Spawn_Position"), bool MustBeTeleportedImmediately = false);
	void update(float time,Perso_Mvt_flag MovingMode = Mvt_Normal, bool ManualFire = false);
	void move(Ogre::Vector4 KeysWeights, bool Jump = false, Perso_Mvt_flag MovingMode = Mvt_Normal);
	void setpos(Ogre::Vector3 temp);
    void rotate(Ogre::Vector3 temp);
    void setOrientation(Ogre::Vector3 temp);
	void sethealth(int);
	void damage(int);
	void fire();
	void destroy();
	void SwichWeapon(uchar NewWeaponIndex,bool NoticeNewtork = false);
};
extern Perso local_player;
extern std::map<RakNet::RakNetGUID, Perso*> PlayerList;

#endif // CLASS_PERSO




































