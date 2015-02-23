#include "Namuh_Main_H.h"

using namespace Ogre;
PhysX_mgr PhysX_man;
//extern Hydrax_mgr Hydrax_man;


// Code des méthodes de la classe
PhysX_mgr::PhysX_mgr(void)
{
	initialised = false;
	first_ini = true;
}
PhysX_mgr::~PhysX_mgr(void)
{
}

void PhysX_mgr::initialise(void)
{
	if(!initialised)
	{
		initialised = true;
		
		if (first_ini) // TODO : see if we can desinitialise PhysX without bug
		{
			mWorld = NxOgre::World::createWorld();
			mWorld->getRemoteDebugger()->connect();
			NxOgre::ResourceSystem::getSingleton()->openProtocol(new Critter::OgreResourceProtocol());
			// Enable CCD via the World. Normally this is off by default in PhysX. So we have to switch it on.
			mWorld->setCCDEnabled(true);
	  

			//scene description
			scndesc.mGravity = NxOgre::Constants::MEAN_EARTH_GRAVITY;
			scndesc.mUseHardware = false;
			// Create scene
			mScene = mWorld->createScene(scndesc);

			// Set some physical scene values
			mScene->getMaterial(0)->setStaticFriction(0.5f);
			mScene->getMaterial(0)->setDynamicFriction(0.5f);
			mScene->getMaterial(0)->setRestitution(0.1f);

			// Create the rendersystem.
			mRenderSystem = new Critter::RenderSystem(mScene, sys0.mgr);


			// Create the Visual Debugger
		/*
			ldbg.send("(PhysX Manager) Creating Visual debugger");
			NxOgre::VisualDebuggerDescription desc;
			desc.showLight();
			desc.mWorld.axes = true;
			desc.mCollision.AABB = true; // Axis Aligned Bounding Box
			desc.mCollision.shapes = false;//  /!\ WARING : Über VERY SLOW if true
			desc.mCollision.spheres = false;
			desc.mCollision.axes= true;
			desc.mJoint.worldAxes = true;
			desc.mJoint.limits = false;
			NxOgre::VisualDebugger debugger(mWorld);
			debugger.enable();
			debugger.loadFromDescription(desc);
			mRenderSystem->createVisualDebugger(desc);
			ldbg.send("(PhysX Manager) Visual debugger created");
			ldbg.send("(PhysX Manager) Visual debugger : shapes : "+ StringConverter::toString(desc.mCollision.shapes));
		*/



		}
		
		// Load Terrain heightfield
		MapHF = loadTerrainGeometry(Map.terrain->getMaterialName(), Map.terrain->getHeightData(), Map.terrain->getSize(),Map.terrain->getWorldSize(), Map.terrain->getMinHeight(), Map.terrain->getMaxHeight(), Map.terrain->getPosition());
		
		PhysX_Waterfall_man.make();
		
		/*
		mCube = mRenderSystem->createBody(NxOgre::BoxDescription(2.925, 2.925, 2.925), NxOgre::Vec3(-9+225,38.2,-42.74+225), "Cube.mesh");
		mCube2 = mRenderSystem->createBody(NxOgre::BoxDescription(2.925, 2.925, 2.925), NxOgre::Vec3(-9+225,42,-41.84+225), "Cube.mesh");
		mCube3 = mRenderSystem->createBody(NxOgre::BoxDescription(2.925, 2.925, 2.925), NxOgre::Vec3(-9+225,44,-41.84+225), "Cube.mesh");
		
		PhysXfloater.add(mCube);
		PhysXfloater.add(mCube2);
		PhysXfloater.add(mCube3);

		// Création d'un radeau a quille
		RocherMesh = NxOgre::MeshManager::getSingleton()->load("ogre://General/Rocher.nxs");
		RocherMesh->setName("Quille du radeau");
		
		Critter::BodyDescription QuilleActorDescription;
		QuilleActorDescription.mMass = 2000.0f;
		//QuilleActorDescription.mLinearDamping = 1.02f;  
		//QuilleActorDescription.mAngularDamping = 1.02f; // Maxi intertie
		//Quille = mRenderSystem->createBody(new NxOgre::Box(2.925, 2.925, 2.925), NxOgre::Vec3(125,32.5,260), "Cube.mesh", QuilleActorDescription);
		Quille = mRenderSystem->createBody(NxOgre::ConvexDescription(RocherMesh),NxOgre::Vec3(125,73.7,260), "rocher.mesh",QuilleActorDescription);
		Quille->setSolverIterationCount(100);
		NxOgre::Mesh* convexMesh = NxOgre::MeshManager::getSingleton()->load("ogre://General/Objects/Radeau_base/Radeau_base.nxs");
		convexMesh->setName("Maillage du radeau de la base");
		Critter::BodyDescription RadeauActorDescription;
		RadeauActorDescription.mMass = 25525.0f; 
		RadeauActorDescription.mLinearDamping = 0.002f;  
		RadeauActorDescription.mAngularDamping = 0.002f; // Petite intertie
		Critter::Body* Radeau = mRenderSystem->createBody(NxOgre::ConvexDescription(convexMesh), NxOgre::Vec3(125,72.2,260), "Radeau_base.mesh", RadeauActorDescription);
		Radeau->setSolverIterationCount(100);
		NxOgre::FixedJoint* Joint = mScene->createFixedJoint(Radeau,Quille);
		Joint->setBreakable(8000000.0f,8000000.0f); // casse si on lance un ménhir de très près 
		PhysXfloater.add(Radeau);


		NxOgre::Mesh* MenhirMesh = NxOgre::MeshManager::getSingleton()->load("ogre://General/Objects/Menhir/Menhir.nxs");
		MenhirMesh->setName("Maillaige du menhir");
		Menhirdescription.mMass = 5850.0f;
		Menhirdescription.mLinearDamping = 0.02f;  
		Menhirdescription.mAngularDamping = 0.01f;
		*/
		// Créatin du petit caillou du lance pierre
		Cailloux = NxOgre::MeshManager::getSingleton()->load("ogre://General/Rocher.nxs");
		
		// Scale du projectile par 0.05
		NxOgre::ManualMesh m(Cailloux->getMeshData());
		for (unsigned int i=0;i < m.nbVertices();i++)
			m.scaleVertex(i, NxOgre::Vec3(0.05f,0.05f,0.05f) );
		Cailloux = m.end();
		m.clean();
		Cailloux->setName("Petit cailloux du Lance Pierre");
		

		/*
		CstrMgr.RocherMesh = NxOgre::MeshManager::getSingleton()->load("ogre://General/Objects/Rocher/Rocher.nxs");
		
		Vector2 Tourelle_pos(124,261);
		NxOgre::Mesh* Tourelle_mesh = NxOgre::MeshManager::getSingleton()->load("ogre://General/Objects/Tourelle/Tourelle.nxs");
		Tourelle_mesh->setName("Ze tourelle");*/
		/*
			// Scale de la tourlle, par 0.7
			NxOgre::ManualMesh m2 = NxOgre::ManualMesh(Tourelle_mesh->getMeshData());
			for (unsigned int i=0;i < m2.nbVertices();i++)
				m2.scaleVertex(i, NxOgre::Vec3(0.7,0.7,0.7) );
			Tourelle_mesh = m2.end();
			m2.clean();*/
		// Définition dela matrice de position et d'orientation de la tourelle
	/*	NxOgre::Quat quat = NxOgre::Quat(NxOgre::Math::cos(3.14/4), 0, NxOgre::Math::sin(3.14/4), 0); // les angles sont ici au facteur 0.5
		quat.normalise();
		NxOgre::Matrix44 mat = NxOgre::Matrix44(
				NxOgre::Vec3(Tourelle_pos.x,terrain_height(Tourelle_pos.x, Tourelle_pos.y,0), Tourelle_pos.y),
				quat);
		mScene->createSceneGeometry(NxOgre::TriangleGeometryDescription(Tourelle_mesh),mat);
		Entity* Tourelle = sys0.mgr->createEntity("Tourelle","Tourelle.mesh");
		SceneNode* Tourelle_n = sys0.mgr->getRootSceneNode()->createChildSceneNode();
		Tourelle_n->attachObject(Tourelle);
		// memes tranformations a l'entité visuelle
		Tourelle_n->setPosition(Tourelle_pos.x,terrain_height(Tourelle_pos.x, Tourelle_pos.y,0), Tourelle_pos.y);
		Tourelle_n->yaw(Radian(3.141/2));
		//Tourelle_n->scale(0.7,0.7,0.7);
		*/
	

	/*
		//création d'un piaf 
		Entity* piaf = sys0.mgr->createEntity("Sapiens test","Sapiens_01.mesh");
		SceneNode* piaf_n = sys0.mgr->getRootSceneNode()->createChildSceneNode();
		piaf_n->attachObject(piaf);
		piaf_n->setPosition(Tourelle_pos.x,terrain_height(Tourelle_pos.x, Tourelle_pos.y,0), Tourelle_pos.y);
		piaf_n->scale(5,5,5);


		// set up a 20 second animation for our light, using spline interpolation for nice curves
		Animation* anim = sys0.mgr->createAnimation("LightTrack", 20);
		anim->setInterpolationMode(Animation::IM_SPLINE);

		// create a track to animate the camera's node
		NodeAnimationTrack* track = anim->createNodeTrack(0, piaf_n);

		// create keyframes for our track
		track->createNodeKeyFrame(0)->setTranslate(Vector3(142, 77, 342));
		track->createNodeKeyFrame(2)->setTranslate(Vector3(121, 84, 335));
		track->createNodeKeyFrame(4)->setTranslate(Vector3(121, 91, 314));
		track->createNodeKeyFrame(6)->setTranslate(Vector3(156, 70, 328));
		track->createNodeKeyFrame(8)->setTranslate(Vector3(128, 70, 356));
		track->createNodeKeyFrame(10)->setTranslate(Vector3(114, 63, 328));
		track->createNodeKeyFrame(12)->setTranslate(Vector3(114, 56, 328));
		track->createNodeKeyFrame(14)->setTranslate(Vector3(100, 35, 384));
		track->createNodeKeyFrame(16)->setTranslate(Vector3(114, 35, 314));
		track->createNodeKeyFrame(18)->setTranslate(Vector3(135, 84, 300));
		track->createNodeKeyFrame(20)->setTranslate(Vector3(142, 77, 342));

		piaf_n->setPosition(track->getNodeKeyFrame(0)->getTranslate());

		// create a new animation state to track this
		mLightAnimState = sys0.mgr->createAnimationState("LightTrack");
		mLightAnimState->setEnabled(true);
		// fin du piaf

		// grotte
		NxOgre::Mesh* Petite_Grotte_mesh = NxOgre::MeshManager::getSingleton()->load("ogre://General/Objects/Petite Grotte/Petite Grotte.nxs");
		mScene->createSceneGeometry(NxOgre::TriangleGeometryDescription(Petite_Grotte_mesh), NxOgre::Matrix44(NxOgre::Vec3(114.8+225,6.7,-121.49+225)));
		Entity* Petite_Grotte = sys0.mgr->createEntity("Grotte","Petite Grotte.mesh");
		SceneNode* Petite_Grotte_n = sys0.mgr->getRootSceneNode()->createChildSceneNode();
		Petite_Grotte_n->attachObject(Petite_Grotte);
		Petite_Grotte_n->setPosition(114.8+225,6.7,-121.49+225);*/

		first_ini = false;  // les prochaines initialisations ne seront plus les premières
							// Ceci gère en particulier le fait de ne pes refaire un RenderSystem a chaque initialisation (bug PhysX)
	}
}

void PhysX_mgr::update(float ellapsed_time)
{
	//mLightAnimState->addTime(ellapsed_time); 
	PhysXfloater.update(ellapsed_time); // Met a jour la physique ainsi que la flotabilité des objets}
	PhysX_Wings_man.update(ellapsed_time);
	// Gestion de la destruction des projectiles en fin de vie
	for (	std::map<RakNet::RakNetGUID, Perso*>::iterator PlayerIterator = PlayerList.begin();
			PlayerIterator != PlayerList.end();
			PlayerIterator++)		
		for (uint j=0;j<PlayerIterator->second->Inventory.size();j++)
			for (uint k=0;k<PlayerIterator->second->Inventory[j]->ProjectileList.size();k++)
				if (PlayerIterator->second->Inventory[j]->ProjectileList[k].Date_Fired + 8000 < sys0.timer.getMilliseconds()
					&& PlayerIterator->second->Inventory[j]->ProjectileList[k].CanDamage)
				{
					 //sys0.mgr->destroySceneNode(PlayerIterator->second->Inventory[j]->ProjectileList[k].body->getNode()->getSceneNode()); // Aww, buggy
					 PlayerIterator->second->Inventory[j]->ProjectileList[k].CanDamage = false;
					 mRenderSystem->destroyBody(PlayerIterator->second->Inventory[j]->ProjectileList[k].body);
				}

	mWorld->advance(ellapsed_time);
}

void PhysX_mgr::destroy(void)
{
/*
	NxOgre::MeshManager::MeshIterator mIterator = NxOgre::MeshManager::getSingleton()->getMeshes(); 
	NxOgre::MeshManager::MeshIterator mIterator2 = NxOgre::MeshManager::getSingleton()->getMeshes(); 

	int i =0;
	while(mIterator2 != mIterator.end())
	{
		sys1.logmsg("*** Debug flag reached ("+string(mIterator2->getName())+") ***");
		mIterator2++;
		i++;
	}
	sys1.logmsg("*** Iterator flag reached ( Il y a jusqu'au n°"+ StringConverter::toString(i) +" meshes) ***");

	while(mIterator != mIterator.end())
	{
		sys1.logmsg("*** Debug flag reached ("+string(mIterator->getName())+") ***");
		NxOgre::MeshManager::getSingleton()->unload(mIterator->getName());
		mIterator++;
		i++;
	}

	sys1.logmsg("*** Debug flag reached (n°BBB) ***");
	*/
	
	PhysX_man.mScene->forceDestroySceneGeometry(MapHF);
	//NxOgre::World::destroyWorld();
	//sys0.mgr->destroyAllEntities();
	//sys0.mgr->destroyAllAnimations();
	//sys0.mgr->destroyAllAnimationStates();
	//sys0.mgr->getRootSceneNode()->removeAllChildren();
	PhysXfloater.flush();

	initialised = false;
}





































