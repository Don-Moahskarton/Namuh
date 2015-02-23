#include "Namuh_Main_H.h"
#include "NxOgre/NxOgreClasses.h" // Plugin PhysX
using namespace Ogre;

Sandbox_Constructeur Constructeur;


Sandbox_Constructeur::Sandbox_Constructeur(void)
{
	initialised = false;
	//zcursor = 2.0f;
	Construction_num = -1.0f; //pas encore de construction en cours !
	diametre_rondin = 0.5f;
	longeur_rondin = 10.0f;
	orientation_rondin = 90.0f;
	diskOrientation = 90.0f;
	diskLength = 0.6f;
	diskRadius = 1.0f;
	triangleOrientation = 90.0f;
	triangleDirection = 0.0f;
	triangleLength = 1.5f;
	triangleRadius = 0.2f;
	CurrentBuildTool = NONE;
	hideCornerNum = 1;
	ClickedBody = NULL;

	// initialise fur attachement corners 
	hideCornersAttachment.push_back(std::pair<Ogre::Vector3, NxOgre::RigidBody*>(Ogre::Vector3::ZERO, NULL));
	hideCornersAttachment.push_back(std::pair<Ogre::Vector3, NxOgre::RigidBody*>(Ogre::Vector3::ZERO, NULL));
	hideCornersAttachment.push_back(std::pair<Ogre::Vector3, NxOgre::RigidBody*>(Ogre::Vector3::ZERO, NULL));
	hideCornersAttachment.push_back(std::pair<Ogre::Vector3, NxOgre::RigidBody*>(Ogre::Vector3::ZERO, NULL));
	
}
Sandbox_Constructeur::~Sandbox_Constructeur(void)
{
}

void Sandbox_Constructeur::intitalise(void)
{

	if(!initialised)
	{
		// charge le curseurs

		SphereCursor = sys0.mgr->createEntity("SphereCursor","sphere.mesh");
		SphereCursor->setMaterial(MaterialManager::getSingleton().getByName("blink"));
		SphereCursor_n = sys0.mgr->getRootSceneNode()->createChildSceneNode();
		SphereCursor_n->attachObject(SphereCursor);
		SphereCursor_n->setVisible(false);


		
		RondinCursor = sys0.mgr->createEntity("LogCursor","Rondin.mesh");
		RondinCursor_n = sys0.mgr->getRootSceneNode()->createChildSceneNode();
		RondinCursor_n->attachObject(RondinCursor);
		RondinCursor_n->setVisible(false);

		RocherCursor = sys0.mgr->createEntity("RockCursor","rocher.mesh");
		RocherCursor_n = sys0.mgr->getRootSceneNode()->createChildSceneNode();
		RocherCursor_n->attachObject(RocherCursor);
		RocherCursor_n->setVisible(false);


		// TODO : conceive how to attach them to allow making weels and other funny stuff
		DiskCursor = sys0.mgr->createEntity("DiskCursor","disk.mesh");
		DiskCursor_n = sys0.mgr->getRootSceneNode()->createChildSceneNode();
		DiskCursor_n->attachObject(DiskCursor);
		DiskCursor_n = sys0.mgr->getRootSceneNode()->createChildSceneNode();
		DiskCursor_n->setVisible(false);

		// TODO : Correct that one ( for some arguments, orientation is biased between the cursor and the created object)
		TriangleCursor_n = sys0.mgr->getRootSceneNode()->createChildSceneNode();
		Ogre::SceneNode* log1 = TriangleCursor_n->createChildSceneNode("TriangleCursor1",Ogre::Vector3(0.0f,0.0f, 0.43f)*triangleLength,Quaternion(Degree(0), Vector3::UNIT_X));
		log1->attachObject(sys0.mgr->createEntity("TriangleCursor1","Rondin.mesh"));
		Ogre::SceneNode* log2 = TriangleCursor_n->createChildSceneNode("TriangleCursor2",Ogre::Vector3(0.37f,0.0f, -0.22f)*triangleLength,Quaternion(Degree(60), Vector3::UNIT_X));
		log2->attachObject(sys0.mgr->createEntity("TriangleCursor2","Rondin.mesh"));
		Ogre::SceneNode* log3 = TriangleCursor_n->createChildSceneNode("TriangleCursor3",Ogre::Vector3(-0.37f,0.0f, -0.22f)*triangleLength,Quaternion(Degree(120), Vector3::UNIT_X));
		log3->attachObject(sys0.mgr->createEntity("TriangleCursor3","Rondin.mesh"));

		TriangleCursor_n->setVisible(false);

		Ogre::SceneNode* axis  = TriangleCursor_n->createChildSceneNode("AxisX",Ogre::Vector3(1.0f,0.01f,0.01f),Quaternion::IDENTITY);
		Ogre::Entity* cube = sys0.mgr->createEntity("AxisX 6846","Cube.mesh");
		cube->setMaterial(MaterialManager::getSingleton().getByName("Template/Red"));
		axis->attachObject(cube);
		axis->setScale(Ogre::Vector3(0.4f,0.01f,0.01f));
		axis  = TriangleCursor_n->createChildSceneNode("AxisY",Ogre::Vector3(0.01f,1.0f,0.01f),Quaternion::IDENTITY);
		cube = sys0.mgr->createEntity("AxisY 6fdgdf46","Cube.mesh");
		cube->setMaterial(MaterialManager::getSingleton().getByName("Templates/RadioactiveGreen"));
		axis->attachObject(cube);
		axis->setScale(Ogre::Vector3(0.01f,0.4f,0.01f));
		axis  = TriangleCursor_n->createChildSceneNode("AxisZ",Ogre::Vector3(0.01f,0.01f,1.0f),Quaternion::IDENTITY);
		cube = sys0.mgr->createEntity("AxisX 684cwvcv6","Cube.mesh");
		cube->setMaterial(MaterialManager::getSingleton().getByName("Template/Blue"));
		axis->attachObject(cube);
		axis->setScale(Ogre::Vector3(0.01f,0.01f,0.4f));
		
		// initailise construction primitives proprieties

		mTrunkDesc.isCooked = false;
		mMenhirDesc.isCooked = false;
		mDiskDesc.isCooked = false;

		initialised = true;
	}
	else
		ldbg.send("Sandbox_Constructeur : initialisation request rejected because already initialised");

	

}

void Sandbox_Constructeur::destroy(void)	//TODO (and call it in partie destructor)
{
	
	if(initialised)
	{
		initialised = false;
	}

}

void Sandbox_Constructeur::update(void)
{
	if(CurrentBuildTool != NONE)
		this->UpdateConstructeur();
}


Ogre::Vector3 Sandbox_Constructeur::ComputeCursor3DPos(float offset)
{
	// Rayon a tester
	CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getPosition();
	Ogre::Ray mRay = sys0.cam->getCameraToViewportRay(mousePos.d_x/float(sys0.vp->getActualWidth()), mousePos.d_y/float(sys0.vp->getActualHeight()));

	// Intersection Terrain
	// intersection est un couple dont le premier élement est un booléen indiquent si le rayon a tapé ou non, le deuxième est un vector3 donnant le lieu de l'intersection
	std::pair <bool, Ogre::Vector3> intersection = static_cast<std::pair <int, Ogre::Vector3>>(Map.terrain->rayIntersects(mRay,10000)); 
	float terrainDist = (intersection.second - mRay.getOrigin()).length();
/*
	// Intersection objets (ogre)
	Ogre::RaySceneQuery* mRayScnQuery = sys0.mgr->createRayQuery(Ogre::Ray());
	mRayScnQuery->setQueryTypeMask(SceneManager::ENTITY_TYPE_MASK);
	mRayScnQuery->setRay(mRay);
	mRayScnQuery->setSortByDistance(true);
	RaySceneQueryResult &result = mRayScnQuery->execute();
	RaySceneQueryResult::iterator itr;
	for ( itr = result.begin( ); itr != result.end(); itr++ )
		if ( itr->movable )
			if(itr->movable->getName().find("Ogre/MO",0) == 0)		// Trouve un objet de la sandbox		
				break;
*/

	//Intersection Rigibody
	NxOgre::Ray Nxray;
	Nxray.mDirection.from(mRay.getDirection());
	Nxray.mOrigin.from(mRay.getOrigin() + (mRay.getDirection().normalisedCopy())*0.3f);
	NxOgre::RaycastHit Nxresult = PhysX_man.mScene->raycastClosestShape(Nxray,NxOgre::Enums::ShapesType_Dynamic,2147483647,10000);
	ClickedBody = Nxresult.mRigidBody;

	// BUG : Cursor3D_Type never reaches CAST_OBJECT
	// either INVALID or CAST_TERRAIN 

	// Prend l'intersection la plus proche
	if( Nxresult.mDistance < terrainDist && ClickedBody != NULL) 
	{
		bool ClickedBodyIsValid = false;
		Cursor3D_Type = INVALID;
		//ldbg.send("Now looking for  : " + ClickedBody->getName());
		//ldbg.send("Now looking for construction : " +  BuiltCstr[Constructeur.getConstruction_num()]->name);
		for (	std::vector<ConstructionElement*>::iterator itt = BuiltCstr[Constructeur.getConstruction_num()]->element.begin();
				itt != BuiltCstr[Constructeur.getConstruction_num()]->element.end();
				++itt)
		{
			//ldbg.send("----Now looking for ConstructionElement : " + (*itt)->name);
			if((*itt)->isRigidBodyBased() && (*itt) != NULL )
			{
				if((*itt)->name == ClickedBody->getName())
				{
					ClickedBodyIsValid = true;
				//	ldbg.send("----Cursor3D_Type = CAST_OBJECT. ");
					Cursor3D_Type = CAST_OBJECT; // CAST_OBJECT uniquement si l'objet casté est dans la construction en cours
					break;
				}
			}
			else
				;//ldbg.send("----Now looking for non RigidBodyBasedobject. ");
		}


		if(!ClickedBodyIsValid)	ClickedBody = NULL; // ClickedBody ne peut pointer sur autre chose qu'un élément de la construction en cours
		Ogre::Vector3 interPos = Ogre::Vector3(Nxresult.mWorldImpact.x,Nxresult.mWorldImpact.y,Nxresult.mWorldImpact.z) + offset * Ogre::Vector3(Nxresult.mWorldNormal.x,Nxresult.mWorldNormal.y,Nxresult.mWorldNormal.z);
		return interPos;
	}
	else
	{
		Cursor3D_Type = CAST_TERRAIN;
		return intersection.second + offset * Ogre::Vector3::UNIT_Y;
	}


}

void Sandbox_Constructeur::UpdateConstructeur(void)
{
	if(CurrentBuildTool == ROCHER || CurrentBuildTool == RONDIN || CurrentBuildTool == DISK || CurrentBuildTool == TRIANGLE || CurrentBuildTool == ANIMAL_HIDE) this->UpdateBuildUI();
	else if(CurrentBuildTool == DELOBJ)  this->UpdateDeleteUI();
	else if(CurrentBuildTool == CREATE_LINK_PICK_FIRST) this->UpdateCreateLinkPickFirstUI();
	else if(CurrentBuildTool == CREATE_LINK_PICK_SECOND) this->UpdateCreateLinkPickSecondUI();
	else 
		ldbg.send("(Sandbox_Constructeur Exeption) Called UpdateConstructeur with incorrect currentMode, Nothing to do.");
}


void Sandbox_Constructeur::UpdateCreateLinkPickFirstUI(void)
{
	// Met a jour le ClickedBody
	this->ComputeCursor3DPos();

	// envois le Body si l'on clique
	if(Cursor3D_Type == CAST_OBJECT)
		if (sys0.ms_releasing(OIS::MB_Left))
			if(ClickedBody != NULL)
			{
				CEGUI::Listbox* Objlist = static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("SanboxLinkCreator/FirstObjLst"));
				if(Objlist->getFirstSelectedItem() != NULL) // Si objet séléctionné il y a, désélectionné il sera
					Objlist->getFirstSelectedItem()->setSelected(false);
				for(Ogre::uint i = 0; i<Objlist->getItemCount(); ++i)
					if(Objlist->getListboxItemFromIndex(i)->getText() == ClickedBody->getName()) // Sélectionne le Body cliqué
						Objlist->getListboxItemFromIndex(i)->setSelected(true);
							
				GuiSandboxMan.winMgr->getWindow("SanboxLinkCreator")->enable();
				GuiSandboxMan.winMgr->getWindow("SanboxLinkCreator")->show();
				setConstructionMode(NONE);
			}
}

void Sandbox_Constructeur::UpdateCreateLinkPickSecondUI(void)
{
	// Met a jour le ClickedBody
	this->ComputeCursor3DPos();

	// envois le Body si l'on clique
	if(Cursor3D_Type == CAST_OBJECT)
		if (sys0.ms_releasing(OIS::MB_Left))
			if(ClickedBody != NULL)
			{
				CEGUI::Listbox* Objlist = static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("SanboxLinkCreator/SecondObjLst"));
				if(Objlist->getFirstSelectedItem() != NULL) // Si objet séléctionné il y a, désélectionné il sera
					Objlist->getFirstSelectedItem()->setSelected(false);
				for(Ogre::uint i = 0; i<Objlist->getItemCount(); ++i)
					if(Objlist->getListboxItemFromIndex(i)->getText() == ClickedBody->getName()) // Sélectionne le Body cliqué
						Objlist->getListboxItemFromIndex(i)->setSelected(true);
							
				GuiSandboxMan.winMgr->getWindow("SanboxLinkCreator")->enable();
				GuiSandboxMan.winMgr->getWindow("SanboxLinkCreator")->show();
				setConstructionMode(NONE);
			}
}

void Sandbox_Constructeur::UpdateDeleteUI(void)
{
	// Met a jour le ClickedBody
	this->ComputeCursor3DPos();

	// Détruit si l'on clique
	if(Cursor3D_Type == CAST_OBJECT)
		if(sys0.ms_releasing(OIS::MB_Left))
			if(ClickedBody != NULL)
			{				
				// Unregister related links				
				for(Ogre::uint i = 0; i<BuiltCstr[Constructeur.getConstruction_num()]->joint.size(); ++i)
					if(BuiltCstr[Constructeur.getConstruction_num()]->joint[i].joint->getFirstRigidBody() == ClickedBody ||
					   BuiltCstr[Constructeur.getConstruction_num()]->joint[i].joint->getSecondRigidBody() == ClickedBody)
						BuiltCstr[Constructeur.getConstruction_num()]->DestroyJoint(BuiltCstr[Constructeur.getConstruction_num()]->joint[i].joint);
				
				// Unregister the element from the construction
				for(Ogre::uint i = 0; i<BuiltCstr[Constructeur.getConstruction_num()]->element.size(); ++i)
					if(BuiltCstr[Constructeur.getConstruction_num()]->element[i]->isRigidBodyBased())
					if(static_cast<ConstructionRigidElement*>(BuiltCstr[Constructeur.getConstruction_num()]->element[i])->physXBody == static_cast<Critter::Body*>(ClickedBody))
						BuiltCstr[Constructeur.getConstruction_num()]->element.erase(BuiltCstr[Constructeur.getConstruction_num()]->element.begin()+i);

				//  Unregister floater if needed
				PhysXfloater.remove(static_cast<Critter::Body*>(ClickedBody));

				// delete Ogre's node
				Critter::Node* ClickedNode = static_cast<Critter::Body*>(ClickedBody)->getNode();
				sys0.mgr->destroySceneNode(ClickedNode->getSceneNode());

				// Delete PhysX's body (Yes, we can cast a Rigidbody to a Critter's Body)
				PhysX_man.mRenderSystem->destroyBody(static_cast<Critter::Body*>(ClickedBody));
				
				Constructeur.refreshLinkLst();
				ClickedBody = NULL;
			}
		
}

void Sandbox_Constructeur::UpdateBuildUI(void)
{
	SceneNode* Cursor = SphereCursor_n;

	//Curseur immatériel
	Ogre::Vector3 Cursor3DPos;
	if(CurrentBuildTool == RONDIN)				Cursor3DPos = this->ComputeCursor3DPos(diametre_rondin/2);
	else if(CurrentBuildTool == ROCHER)			Cursor3DPos = this->ComputeCursor3DPos(3.0f);
	else if(CurrentBuildTool == DISK)			Cursor3DPos = this->ComputeCursor3DPos(diskLength);
	else if(CurrentBuildTool == TRIANGLE)		Cursor3DPos = this->ComputeCursor3DPos(longeur_rondin * (2/3) + diametre_rondin/2);
	else if(CurrentBuildTool == ANIMAL_HIDE)	Cursor3DPos = this->ComputeCursor3DPos(-0.05f);
	else										Cursor3DPos = this->ComputeCursor3DPos();

	bool	isbeginningCstr = (Cursor3D_Type == CAST_TERRAIN && BuiltCstr[Constructeur.getConstruction_num()]->element.size() == 0),
			isCatingObj = Cursor3D_Type == CAST_OBJECT,
			drawing = isDrawing == true;
	
	if( isbeginningCstr || isCatingObj || drawing )
	{
		Cursor->resetToInitialState();
		Cursor->setVisible(true);
		Cursor->setPosition(Vector3(	Cursor3DPos.x,
										Cursor3DPos.y,
										Cursor3DPos.z));
		Cursor->scale(0.25f,0.25f,0.25f);

		// Stretch the tool to the current cursor position
		Ogre::Vector3	displacement =  (Cursor3DPos - clikedPos);

		// Drawing cursor
		if (sys0.ms->buttonDown(OIS::MB_Left) && isDrawing)
		{
			if (CurrentBuildTool == RONDIN)
			{
				RondinCursor_n->resetToInitialState();
				RondinCursor_n->setVisible(true);
				RondinCursor_n->setPosition(Cursor3DPos - displacement / 2 );
				RondinCursor_n->scale(diametre_rondin/2,displacement.length()/10,diametre_rondin/2);
				RondinCursor_n->lookAt(Cursor3DPos,Ogre::Node::TransformSpace::TS_WORLD);
				RondinCursor_n->rotate(Ogre::Quaternion(Ogre::Radian(3.141/2),Vector3::UNIT_X));

				// scroll to adjust log radius
				diametre_rondin = GuiSandboxMan.mouseZ*0.0025f + 0.5f;
				if(diametre_rondin<0.2f)
					diametre_rondin=0.2f;
			}
			if(CurrentBuildTool == ROCHER)
			{
				RocherCursor_n->resetToInitialState();
				RocherCursor_n->setVisible(true);
				RocherCursor_n->setPosition(Cursor3DPos);
			}
			if(CurrentBuildTool == DISK)
			{
			
				DiskCursor_n->resetToInitialState();
				DiskCursor_n->setVisible(true);
				DiskCursor_n->setPosition(Cursor3DPos);
				DiskCursor_n->scale(displacement.length()/2,diskLength*10,displacement.length()/2);
				DiskCursor_n->lookAt(Cursor3DPos,Ogre::Node::TransformSpace::TS_WORLD);
				DiskCursor_n->rotate(Ogre::Quaternion(Ogre::Radian(3.141/2),Vector3::UNIT_X));

				
				// scroll to adjust log radius
				diskLength = GuiSandboxMan.mouseZ*0.001f + 1.5f;
				if(diskLength<0.1f)
					diskLength=0.1f;
			}
			if (CurrentBuildTool == TRIANGLE)
			{
				TriangleCursor_n->resetToInitialState();
				TriangleCursor_n->setVisible(true);
				TriangleCursor_n->setPosition(Cursor3DPos - displacement / 2 );
				triangleLength = displacement.length()* 0.866; // Sqrt(3)/2
				//TriangleCursor_n->scale(triangleLength/2,displacement.length()/10,triangleLength/2);
				TriangleCursor_n->lookAt(Cursor3DPos,Ogre::Node::TransformSpace::TS_WORLD);
				//TriangleCursor_n->rotate(Ogre::Quaternion(Ogre::Radian(-3.141/3),Vector3::UNIT_Y),Ogre::Node::TransformSpace::TS_LOCAL);


				
				Ogre::Quaternion	Orientation =	  TriangleCursor_n->getOrientation();

				Ogre::Matrix3	localToGlobal = Ogre::Matrix3::ZERO,
								GLobalToLocal;
				Orientation.ToRotationMatrix(localToGlobal);
				GLobalToLocal = localToGlobal.Inverse();

				TriangleCursor_n->getChild("TriangleCursor1")->setPosition(localToGlobal * 0.5 * Ogre::Vector3(0.0f,0.0f, 0.43f * triangleLength));				
				TriangleCursor_n->getChild("TriangleCursor2")->setPosition(localToGlobal * 0.5  * Ogre::Vector3(0.37f * triangleLength,0.0f, -0.22f * triangleLength));				
				TriangleCursor_n->getChild("TriangleCursor3")->setPosition(localToGlobal * 0.5  * Ogre::Vector3(-0.37f * triangleLength,0.0f, -0.22f * triangleLength));	

				TriangleCursor_n->getChild("TriangleCursor1")->setOrientation(Orientation * Ogre::Quaternion(Degree(90), Vector3::UNIT_X) * Ogre::Quaternion(Degree(90), Vector3::UNIT_Z));
				TriangleCursor_n->getChild("TriangleCursor2")->setOrientation(Orientation* Ogre::Quaternion(Degree(90), Vector3::UNIT_X) * Ogre::Quaternion(Degree(-30), Vector3::UNIT_Z));
				TriangleCursor_n->getChild("TriangleCursor3")->setOrientation(Orientation * Ogre::Quaternion(Degree(90), Vector3::UNIT_X) * Ogre::Quaternion(Degree(30), Vector3::UNIT_Z));

				TriangleCursor_n->getChild("TriangleCursor1")->setScale(triangleRadius/2,triangleLength/10,triangleRadius/2);
				TriangleCursor_n->getChild("TriangleCursor2")->setScale(triangleRadius/2,triangleLength/10,triangleRadius/2);
				TriangleCursor_n->getChild("TriangleCursor3")->setScale(triangleRadius/2,triangleLength/10,triangleRadius/2);



				// scroll to adjust log radius
				triangleRadius = GuiSandboxMan.mouseZ*0.0025f + 1;
				if(triangleRadius<0.2f)
					triangleRadius=0.2f;

			}
			if (CurrentBuildTool == ANIMAL_HIDE)
			{
				// cursor display, when clicking hide corners
			}


		}


		// Create object on mouse release


		// ===================================================================================================================================================================================================
		// ===================================================================================================================================================================================================
		// ===================================================================================================================================================================================================
		// =========================================================================  Thoses constructions won't appear on another map than Caribean island ==================================================
		// ===================================================================================================================================================================================================
		// ===================================================================================================================================================================================================
		// ===================================================================================================================================================================================================

		if(sys0.ms_releasing(OIS::MB_Left) && displacement.length() > 0.01f  && isDrawing  && displacement.length() < 100.0f) // also cap displacement to avoid huge lags (dunno why they appears)
		{
		
			drawBodyToLinkB = ClickedBody;

			if (CurrentBuildTool == RONDIN)
			{
				mTrunkDesc.container = BuiltCstr[Constructeur.getConstruction_num()];
				mTrunkDesc.orientation = RondinCursor_n->getOrientation();
				mTrunkDesc.position = (Cursor3DPos + clikedPos) / 2;
				mTrunkDesc.radius = diametre_rondin/2;
				mTrunkDesc.length = displacement.length();
				mTrunkDesc.name = "Trunk" + toOgStr(sys0.timer.getMilliseconds());

				ldbg.send("Creating trunk :" + mTrunkDesc.name);


				Trunk* createdTrunk = CstrMgr.CreateTrunk( &mTrunkDesc);
				// get the log done for good
				PhysX_man.update(0.016f);
				
				RondinCursor_n->setVisible(false);

				// TODO : correct ropes positions at thoses two points
				if(drawBodyToLinkA != NULL)
				{
					// Crée le joint PhysX entre le nouveau rigibody et celui que l'on a cliqué en premier
					if(drawBodyToLinkA != NULL)
						BuiltCstr[Constructeur.getConstruction_num()]->CreateFixedLink(	static_cast<ConstructionRigidElement*>(BuiltCstr[Constructeur.getConstruction_num()]->getElementFromBody(drawBodyToLinkA)),
																						createdTrunk, 
																						"",
																						true,
																						clikedPos,	// Where we began to click on drawBodyToLinkA
																						clikedPos,	// Where we began to click on createdTrunk
																						diametre_rondin*1.3/2);
		
					// retrace la liste des links
					Constructeur.refreshLinkLst();
				}	

				if(drawBodyToLinkB != NULL)
				{
					// Crée le joint PhysX entre le nouveau rigibody et celui que l'on a relaché la souris
					BuiltCstr[Constructeur.getConstruction_num()]->CreateFixedLink(	static_cast<ConstructionRigidElement*>(BuiltCstr[Constructeur.getConstruction_num()]->getElementFromBody(drawBodyToLinkB)),
																					createdTrunk, 
																					"",
																					true,
																					Cursor3DPos,	// where we released mouse on drawBodyToLinkB
																					Cursor3DPos,	// where we released mouse on createdTrunk
																					diametre_rondin*1.3/2);
					// retrace la liste des links
					Constructeur.refreshLinkLst();
				}
			}

			else if (CurrentBuildTool == ROCHER)
			{
				mMenhirDesc.container = BuiltCstr[Constructeur.getConstruction_num()];
				mMenhirDesc.orientation = Quaternion::IDENTITY;
				mMenhirDesc.position = Cursor3DPos;
				mMenhirDesc.name = "Menhir" + toOgStr(sys0.timer.getMilliseconds());

				CstrMgr.CreateMenhir( &mMenhirDesc);
				
				// get the menhir done for good
				PhysX_man.update(0.016f);

				RocherCursor_n->setVisible(false);

				if(drawBodyToLinkB != NULL &&  BuiltCstr[Constructeur.getConstruction_num()]->element.back()->isRigidBodyBased())
				{
					// Crée le joint PhysX entre le nouveau rigibody et celui que l'on a relaché la souris
					BuiltCstr[Constructeur.getConstruction_num()]->CreateFixedLink(	static_cast<ConstructionRigidElement*>(BuiltCstr[Constructeur.getConstruction_num()]->getElementFromBody(drawBodyToLinkB)),
																					static_cast<ConstructionRigidElement*>(BuiltCstr[Constructeur.getConstruction_num()]->element.back()));
	
					// retrace la liste des links
					Constructeur.refreshLinkLst();
				}
			}

			else if (CurrentBuildTool == DISK)
			{

				mDiskDesc.container = BuiltCstr[Constructeur.getConstruction_num()];
				mDiskDesc.orientation = DiskCursor_n->getOrientation();
				mDiskDesc.position = clikedPos;
				mDiskDesc.radius = displacement.length();
				mDiskDesc.thickness = diskLength/2;
				mDiskDesc.name = "Disk" + toOgStr(sys0.timer.getMilliseconds());

				ldbg.send("Creating Disk :" + mTrunkDesc.name);


				Disk* createdDisk = CstrMgr.Create_disk( &mDiskDesc);

				// get the disk done for good
				PhysX_man.update(0.016f);

				DiskCursor_n->setVisible(false);

				if(drawBodyToLinkB != NULL &&  BuiltCstr[Constructeur.getConstruction_num()]->element.back()->isRigidBodyBased())
				{
					// Crée le joint PhysX entre le nouveau rigibody et celui que l'on a relaché la soursi
					BuiltCstr[Constructeur.getConstruction_num()]->CreateFixedLink(	static_cast<ConstructionRigidElement*>(BuiltCstr[Constructeur.getConstruction_num()]->getElementFromBody(drawBodyToLinkB)),
																					static_cast<ConstructionRigidElement*>(BuiltCstr[Constructeur.getConstruction_num()]->element.back()));
		
					// retrace la liste des links
					Constructeur.refreshLinkLst();
				}
			}
			
			else if(CurrentBuildTool == TRIANGLE)
			{
				CstrMgr.Create_triangle(BuiltCstr[Constructeur.getConstruction_num()],
										(Cursor3DPos + clikedPos) / 2, 
										TriangleCursor_n->getOrientation(),
										triangleRadius,
										triangleLength);
				
				TriangleCursor_n->setVisible(false);

				ldbg.send("(triangle creation) TODO : link creation is based on last contruction rigid element. But triangle have to be adapted so the proper trunk is linked to correct other body");

				// TODO : make some ropes at thoses two points
				if(drawBodyToLinkA != NULL &&  BuiltCstr[Constructeur.getConstruction_num()]->element.back()->isRigidBodyBased())
				{
					// Crée le joint PhysX entre le nouveau rigibody et celui que l'on a cliqué en premier
					BuiltCstr[Constructeur.getConstruction_num()]->CreateFixedLink(	static_cast<ConstructionRigidElement*>(BuiltCstr[Constructeur.getConstruction_num()]->getElementFromBody(drawBodyToLinkA)), 
																					static_cast<ConstructionRigidElement*>(BuiltCstr[Constructeur.getConstruction_num()]->element.back()));
		
					// retrace la liste des links
					Constructeur.refreshLinkLst();
				}	

				if(drawBodyToLinkB != NULL &&  BuiltCstr[Constructeur.getConstruction_num()]->element.back()->isRigidBodyBased())
				{
					// Crée le joint PhysX entre le nouveau rigibody et celui que l'on a relaché la souris
					BuiltCstr[Constructeur.getConstruction_num()]->CreateFixedLink(	static_cast<ConstructionRigidElement*>(BuiltCstr[Constructeur.getConstruction_num()]->getElementFromBody(drawBodyToLinkB)),
																					static_cast<ConstructionRigidElement*>(BuiltCstr[Constructeur.getConstruction_num()]->element.back()));
		
					// Redraw link list
					Constructeur.refreshLinkLst();
				}
			}
			
			else if(CurrentBuildTool == ANIMAL_HIDE)
			{
				
				// memorize clicked pos and body to attach
				hideCornersAttachment[hideCornerNum - 1] = std::pair<Ogre::Vector3, NxOgre::RigidBody*>(Cursor3DPos, ClickedBody);

				if(hideCornerNum >= 4 )
				{
					// little debug string
					string log = "(hide build tool) : creating hide with options : ";

					if( GuiSandboxMan.isHideBounce)
						if (GuiSandboxMan.isHideWing)
							log += " Bounce and Wing";
						else
							log += " bounce";
					else if (GuiSandboxMan.isHideWing)
						log += "wing";
					else
						log += " None";

					// generate the hide based on memorized corners
					CstrMgr.Create_hide(	BuiltCstr[Constructeur.getConstruction_num()], 
											hideCornersAttachment[0].first, 
											static_cast<Critter::Body*>(hideCornersAttachment[0].second), 
											hideCornersAttachment[1].first, 
											static_cast<Critter::Body*>(hideCornersAttachment[1].second), 
											hideCornersAttachment[3].first, 
											static_cast<Critter::Body*>(hideCornersAttachment[3].second), 
											hideCornersAttachment[2].first,
											static_cast<Critter::Body*>(hideCornersAttachment[2].second),  // 2 et 3 inversé pour plus d'ergonomie
											GuiSandboxMan.isHideWing,
											GuiSandboxMan.isHideBounce,
											"Hide" + toOgStr(sys0.timer.getMilliseconds()));

					hideCornerNum = 1;
				}
				else
					hideCornerNum++;
			}

			
			isDrawing = false; // object placed, so no longer drawing
		}


		// Begin draw on pressing mouse
		if(sys0.ms_pressing(OIS::MB_Left) && !isDrawing)
		{
			clikedPos = Cursor3DPos;	// Memorise clicked position
			isDrawing = true;
			drawBodyToLinkA = ClickedBody;

			if (CurrentBuildTool == RONDIN)
				GuiSandboxMan.setToaster(2000, "Use wheel to adjust log's diameter");
			else if(CurrentBuildTool == ROCHER)			
				GuiSandboxMan.setToaster(2000, "Release mouse to place rock");
			else if (CurrentBuildTool == DISK)
				GuiSandboxMan.setToaster(2000, "Use wheel to adjust disk's thickness");
			else if(CurrentBuildTool == TRIANGLE)
				GuiSandboxMan.setToaster(2000, "Use wheel to adjust triangle's thickness");
			else if(CurrentBuildTool == ANIMAL_HIDE)
				if(hideCornerNum == 1)
					GuiSandboxMan.setToaster(1000, "Release mouse to place first corner");
				else if(hideCornerNum == 2)
					GuiSandboxMan.setToaster(1000, "Release mouse to place second corner");
				else if(hideCornerNum == 3)
					GuiSandboxMan.setToaster(1000, "Release mouse to place third corner");
				else if(hideCornerNum == 4)
					GuiSandboxMan.setToaster(1000, "Release mouse to place last corner");
		
		}
	}
	else
		Cursor->setVisible(false);
}

void Sandbox_Constructeur::NewConstruction(void)
{
	// Crée la construction
	BuiltCstr.push_back(CstrMgr.CreateNew());
	
	// Crée un élément dans la liste
	CEGUI::Listbox* Cstrlist = static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("Sandbox_interface/Sandbox/Liste_constructions"));
	CEGUI::ListboxTextItem* CstrListItem = new CEGUI::ListboxTextItem(BuiltCstr.back()->name);
	CstrListItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
	Cstrlist->addItem(CstrListItem);

	// La définit comme active
	Constructeur.setConstruction_num(BuiltCstr.size()-1);
}

void Sandbox_Constructeur::setConstructionMode(Construction_mode NewMode)
{	
	Constructeur.CurrentBuildTool = NewMode;
	
	// retour visuel a l'utilisateur
	CEGUI::WindowManager * winMgr = CEGUI::WindowManager::getSingletonPtr();
	if(NewMode == RONDIN)
	{
		ldbg.send("Contructeur :  Now using logs");
		winMgr->getWindow("Sandbox_interface/StatusBar")->setText("Click on a existing object to place a log on it.");
		GuiSandboxMan.setToaster(3000, "Click and hold to place a log, use wheel to change radius");
	}
	if(NewMode == ROCHER)
	{
		ldbg.send("Contructeur :  Now using rocks");
		winMgr->getWindow("Sandbox_interface/StatusBar")->setText("Click on a existing object to place a rock on it");
		GuiSandboxMan.setToaster(3000, "Click and hold where to place a rock");
	}
	if(NewMode == DISK)
	{
		ldbg.send("Contructeur :  Now using disks");
		winMgr->getWindow("Sandbox_interface/StatusBar")->setText("Click on a existing object to place a disk on it");
		GuiSandboxMan.setToaster(3000, "Click and hold to place a disk, use wheel to adjust thickness");
	}
	
	if(NewMode == TRIANGLE)
	{
		ldbg.send("Contructeur :  Now using triangles");
		winMgr->getWindow("Sandbox_interface/StatusBar")->setText("Click on a existing object to place a triangle on it");
		GuiSandboxMan.setToaster(3000, "Click where to place a triangle, use wheel to adjust thickness");
	}
	if(NewMode == ANIMAL_HIDE)
	{
		ldbg.send("Contructeur :  Now using Hide");
		ldbg.send("TODO : Enable attachement (once the hide is built) to any point of the hide");
		winMgr->getWindow("Sandbox_interface/StatusBar")->setText("Click four time to place the hide corners");
		GuiSandboxMan.setToaster(3000, "CLick four time on existing objects to place corners of the hide");
	}

	if(NewMode == DELOBJ)
	{
		ldbg.send("Contructeur :  Now deleting objects");
		winMgr->getWindow("Sandbox_interface/StatusBar")->setText("Click on a existing object to delete it");
		GuiSandboxMan.setToaster(3000, "Click to destroy an object, be carefull. Related links will also be destroyed");
	}


}

Construction_mode Sandbox_Constructeur::getConstructionMode(void)
{
	return CurrentBuildTool;
}

void Sandbox_Constructeur::sethideCornerNum(int requestedhideCornerNum)
{
	// Verify requestedhideCornerNum
	if(requestedhideCornerNum < 1)
		requestedhideCornerNum = 1; 
	if(requestedhideCornerNum > 4)
		requestedhideCornerNum = 4;
	// Change hideCornerNum
	hideCornerNum = requestedhideCornerNum;
}

int Sandbox_Constructeur::gethideCornerNum(void)
{
	return Constructeur.hideCornerNum;
}



void Sandbox_Constructeur::setConstruction_num(int NewConstruction_num)
{
	// Change Construction_num
	Construction_num = NewConstruction_num;

	// Sélectionne la construction dans la liste des constructions
	CEGUI::Listbox* Cstrlist = static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("Sandbox_interface/Sandbox/Liste_constructions"));
	if(Cstrlist->getSelectedCount())
		Cstrlist->getFirstSelectedItem()->setSelected(false);
	Cstrlist->getListboxItemFromIndex(Construction_num)->setSelected(true);

	// Redessine la liste des liaisons
	this->refreshLinkLst();
}

int Sandbox_Constructeur::getConstruction_num(void)
{
	return Constructeur.Construction_num;
}


void Sandbox_Constructeur::refreshLinkLst(void)
{
	// Redessine la liste des liaisons
	CEGUI::Listbox* LnkLst = static_cast<CEGUI::Listbox*>(GuiSandboxMan.winMgr->getWindow("Sandbox_interface/Sandbox/Liste_liaisons"));
	LnkLst->resetList();
	CEGUI::ListboxTextItem* LinkListItem;
	for( Ogre::uint i = 0; i<BuiltCstr[Constructeur.getConstruction_num()]->joint.size(); ++i)
	{
		LinkListItem = new CEGUI::ListboxTextItem(BuiltCstr[Constructeur.getConstruction_num()]->joint[i].name);
		LinkListItem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
		LnkLst->addItem(LinkListItem);
	}
}
































