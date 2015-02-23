#include "Namuh_Main_H.h"

using namespace Ogre;
// Code des méthodes de la classe
Lance_Pierre::Lance_Pierre(void)
{
}
Lance_Pierre::~Lance_Pierre(void)
{
}


void Lance_Pierre::initialise(RakNet::RakNetGUID Requested_Owner_id) // Tir !!
{
	NextProjectileID =0;
	Cooldown = 909; // 1.1 pierre/seconde (Like UT3 Rocket Launcher)
	// For Further details about UT3 Rocket Lancher, see http://liandri.beyondunreal.com/Rocket_Launcher

	PierreDescription.mMass = 200.6f;
	PierreDescription.mLinearDamping = 0.002f;  
	PierreDescription.mAngularDamping = 0.001f;
	PierreDescription.mDynamicRigidbodyFlags += NxOgre::DynamicRigidbodyFlags::DisableGravity;

	Owner = PlayerList[Requested_Owner_id];

	WeaponMesh = sys0.mgr->createEntity("Lance Pierre" + StringConverter::toString(sys0.timer.getMicroseconds()),"Lance Pierre.mesh"); // Attention, risque de problème de nommage d'entité lors de plusieurs lance pierre (SAuf que 1 seul par jouer, m'enfin...)
	WeaponNode = Owner->CamNode->createChildSceneNode();
		WeaponNode->attachObject(WeaponMesh);
		WeaponNode->setPosition(0.2f,-0.30f,-0.5f);
}

void Lance_Pierre::fire(void) // Tir !!
{
	if(LastfiredTimer+Cooldown < sys0.timer.getMilliseconds())
	{
		ldbg.send("Tir au lance pierre");
		Ogre::Radian	angle_horizontal = Owner->Node->getOrientation().getYaw(),
						angle_vertical = Owner->rot.getPitch();
		NxOgre::Vec3 projectilepos = NxOgre::Vec3(	Owner->pos.x-Ogre::Math::Sin(angle_horizontal)*.7,
													Owner->pos.y-0.2, 
													Owner->pos.z-Ogre::Math::Cos(angle_horizontal)*.7);
		
		Critter::BodyDescription currentPierreDescription = PierreDescription; // New node for the projectile
		Projectile CurrentProjectile(Owner);
		CurrentProjectile.body = PhysX_man.mRenderSystem->createBody(NxOgre::BoxDescription(0.2f,0.2f,0.7f),NxOgre::Vec3(Owner->pos.x,Owner->pos.y,Owner->pos.z), "Pierre du Lance Pierre.mesh",currentPierreDescription);
		CurrentProjectile.body->setGlobalPosition(projectilepos);
		CurrentProjectile.body->setGlobalOrientationQuat(NxOgre::Quat(WeaponNode->convertLocalToWorldOrientation(WeaponNode->getOrientation()))); //caméra, bofbof
		CurrentProjectile.body->addLocalForce(NxOgre::Vec3(0,0,-60.0*currentPierreDescription.mMass), NxOgre::Enums::ForceMode_SmoothImpulse);
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
				myCallback.type = LancePierre;
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


























































































































