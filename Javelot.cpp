#include "Namuh_Main_H.h"

using namespace Ogre;
// Code des méthodes de la classe
Javelot::Javelot(void)
{
}
Javelot::~Javelot(void)
{
}


void Javelot::initialise(RakNet::RakNetGUID Requested_Owner_id) // Tir !!
{
	NextProjectileID =0;
	Cooldown = 1200; //0.8 Javelot/sec, (ptet un peu bcp)

	JavelotDescription.mMass = 5.0f;
	JavelotDescription.mLinearDamping = 0.007f;  
	JavelotDescription.mAngularDamping = 0.001f;
	//JavelotDescription.mDynamicRigidbodyFlags += NxOgre::DynamicRigidbodyFlags::DisableGravity;

	Owner = PlayerList[Requested_Owner_id];

	WeaponMesh = sys0.mgr->createEntity("Javelot" + StringConverter::toString(sys0.timer.getMicroseconds()),"Javelot.mesh"); // Attention, risque de problème de nommage d'entité lors de plusieurs lance pierre (SAuf que 1 seul par joueur, m'enfin...)
	WeaponNode = Owner->CamNode->createChildSceneNode();
		WeaponNode->attachObject(WeaponMesh);
		WeaponNode->setPosition(0.2f,-0.30f,-0.5f);
}

void Javelot::fire(void) // Tir !!
{

	if(LastfiredTimer+Cooldown < sys0.timer.getMilliseconds())
	{
		ldbg.send("Lancé de javelot");
		Ogre::Radian	angle_horizontal = Owner->Node->getOrientation().getYaw(),
						angle_vertical = Owner->rot.getPitch();
		NxOgre::Vec3 projectilepos = NxOgre::Vec3(	Owner->pos.x-Ogre::Math::Sin(angle_horizontal)*1.4,
													Owner->pos.y+0.5, 
													Owner->pos.z-Ogre::Math::Cos(angle_horizontal)*1.4);
		
		Critter::BodyDescription currentJavelotDescription = JavelotDescription; // New node for the projectile
		Projectile CurrentProjectile(Owner);
		CurrentProjectile.body = PhysX_man.mRenderSystem->createBody(NxOgre::BoxDescription(0.1f,0.1f,1.0f),NxOgre::Vec3(Owner->pos.x,Owner->pos.y,Owner->pos.z), "Javelot_projectile.mesh",currentJavelotDescription);
		CurrentProjectile.body->setGlobalPosition(projectilepos);
		CurrentProjectile.body->setGlobalOrientationQuat(NxOgre::Quat(WeaponNode->convertLocalToWorldOrientation(WeaponNode->getOrientation()))); //caméra, bofbof
		CurrentProjectile.body->addLocalForce(NxOgre::Vec3(0,0,-40.0*currentJavelotDescription.mMass), NxOgre::Enums::ForceMode_SmoothImpulse);
		CurrentProjectile.body->setCCDMotionThreshold(0);
		CurrentProjectile.Date_Fired = sys0.timer.getMilliseconds();
		CurrentProjectile.CanDamage = true;
		ProjectileList.push_back(CurrentProjectile);

		for (	std::map<RakNet::RakNetGUID, Perso*>::iterator PlayerIterator = PlayerList.begin();
			PlayerIterator != PlayerList.end();
			PlayerIterator++)
		{
			if(PlayerIterator->second->id != Owner->id)
			{
				
				ProjCallback myCallback;
				myCallback.ProjectileID = ProjectileList.size()-1;
				myCallback.AgressorID = Owner->id;
				myCallback.TargetID = PlayerIterator->second->id ;
				myCallback.type = Javelot_;
				ProjCallbackList.push_back(myCallback);
				CurrentProjectile.body->setContactCallback(&ProjCallbackList.back());

				PlayerIterator->second->CharCtrl->setContactCallback(&ProjCallbackList.back());
				PhysX_man.mScene->setActorFlags(ProjectileList.back().body,PlayerIterator->second->CharCtrl, NxOgre::Enums::ContactPairFlags_All);
			}
		}
		NextProjectileID++;
		LastfiredTimer = sys0.timer.getMilliseconds();
	}
}


























































































































