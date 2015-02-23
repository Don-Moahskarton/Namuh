/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_PROJECTILE_CALLBACKS	// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_PROJECTILE_CALLBACKS

#include "Namuh_Main_H.h"

class ProjCallback : public NxOgre::Callback
{
	public:
	RakNet::RakNetGUID	TargetID,
						AgressorID;
	int	ProjectileID;
	Weapon type;

 void onContact(const NxOgre::ContactPair& pair)
 {
	 switch(type)
	 {
		case LancePierre:
		 {

			 if(PlayerList[AgressorID]->Inventory[0]->ProjectileList[ProjectileID].CanDamage)
			 {
				 PlayerList[TargetID]->damage(80);
				 PlayerList[TargetID]->Last_Damage_Owner_id = AgressorID;
				 ldbg.send("id " + raknet_man.GUIDtoString(AgressorID) + " Damaged avec un Lance Pierre id" + raknet_man.GUIDtoString(TargetID) + ", with is now on  "+ StringConverter::toString(PlayerList[TargetID]->vie) +"hp" );

				 Ogre::SceneNode *projectNode = PlayerList[AgressorID]->Inventory[0]->ProjectileList[ProjectileID].body->getNode()->getSceneNode();
				 PhysX_man.mRenderSystem->destroyBody(PlayerList[AgressorID]->Inventory[0]->ProjectileList[ProjectileID].body);
				 sys0.mgr->destroySceneNode(projectNode);
				 PlayerList[AgressorID]->Inventory[0]->ProjectileList[ProjectileID].CanDamage = false;
			 }
		 }
		break;
		case ArcEnBois:
		 {

			 if(PlayerList[AgressorID]->Inventory[1]->ProjectileList[ProjectileID].CanDamage)
			 {
				 PlayerList[TargetID]->damage(50);
				 PlayerList[TargetID]->Last_Damage_Owner_id = AgressorID;
				 ldbg.send("id "+raknet_man.GUIDtoString(AgressorID)+" Damaged avec un arc id"+raknet_man.GUIDtoString(TargetID)+", with is now on  "+ StringConverter::toString(PlayerList[TargetID]->vie) +"hp" );

				 Ogre::SceneNode *projectNode = PlayerList[AgressorID]->Inventory[1]->ProjectileList[ProjectileID].body->getNode()->getSceneNode();
				 PhysX_man.mRenderSystem->destroyBody(PlayerList[AgressorID]->Inventory[1]->ProjectileList[ProjectileID].body);
				 sys0.mgr->destroySceneNode(projectNode);
				 PlayerList[AgressorID]->Inventory[1]->ProjectileList[ProjectileID].CanDamage = false;
			 }
		 }
		break;
		case Javelot_:
		 {

			 if(PlayerList[AgressorID]->Inventory[2]->ProjectileList[ProjectileID].CanDamage)
			 {
				 PlayerList[TargetID]->damage(70);
				 PlayerList[TargetID]->Last_Damage_Owner_id = AgressorID;
				 // if(raknet_man.isServer)
				 ldbg.send(" id "+raknet_man.GUIDtoString(AgressorID)+" Damaged avec un Javelot id"+raknet_man.GUIDtoString(TargetID)+", with is now on  "+ StringConverter::toString(PlayerList[TargetID]->vie) +"hp" );

				 Ogre::SceneNode *projectNode = PlayerList[AgressorID]->Inventory[2]->ProjectileList[ProjectileID].body->getNode()->getSceneNode();
				 PhysX_man.mRenderSystem->destroyBody(PlayerList[AgressorID]->Inventory[2]->ProjectileList[ProjectileID].body);
				 sys0.mgr->destroySceneNode(projectNode);
				 PlayerList[AgressorID]->Inventory[2]->ProjectileList[ProjectileID].CanDamage = false;
			 }
		 }
		break;
	 }
 }

};

extern std::vector<ProjCallback> ProjCallbackList;

#endif // CLASS_PERSO



























































