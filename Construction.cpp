#include "Namuh_Main_H.h"
using namespace Ogre;


Construction::Construction(void)
{
	name = "Construction" + StringConverter::toString(sys0.timer.getMicroseconds());
	root = PhysX_man.mRenderSystem->createBody(NxOgre::BoxDescription(0.0f), NxOgre::Matrix44::IDENTITY, Critter::BodyDescription());
	root->getNode()->setPosition(0,0,0);
}
Construction::~Construction(void)
{
}
/* // We don't need other link type than fixed. Revolute ones are well made by a Torus.
void Construction::CreateRevoluteLink(NxOgre::RigidBody* FirstBody, NxOgre::RigidBody* SecondBody, string name)
{
	NxOgre::RevoluteJointDescription join_descr = NxOgre::RevoluteJointDescription();
	join_descr.mLocalAnchor[0] = NxOgre::Vec3(NxOgre::Vec3(3,0,0));
	join_descr.mLocalAxis[0] = NxOgre::Vec3(0,1,0);
	join_descr.mLocalNormal[0] = NxOgre::Vec3(1,0,0);
	join_descr.mLocalAnchor[1] = NxOgre::Vec3(NxOgre::Vec3(0,0,0));
	join_descr.mLocalAxis[1] = NxOgre::Vec3(0,1,0);
	join_descr.mLocalNormal[1] = NxOgre::Vec3(1,0,0);
	join_descr.mJointFlags = NxOgre::Enums::JointFlag_Visualisation | NxOgre::Enums::JointFlag_CollisionEnabled;
	
	//join_descr.mLocalAnchor[0] = NxOgre::Vec3(FirstBody->getShape(0)->getGlobalPose()) + NxOgre::Vec3(0,0.5,0);
	
//	join_descr.mLocalAxis[1] = NxOgre::Vec3(0,1,0);
	//join_descr.mLocalNormal[1] = NxOgre::Vec3(1,0,0);
	//join_descr.mLocalAnchor[1] = NxOgre::Vec3(SecondBody->getShape(0)->getGlobalPose()) + NxOgre::Vec3(0,0,0);
	//join_descr.mMotor = NxOgre::MotorDescription(50 , 1000000,true);
	
	NxOgre_NamedJoint NewJoint;
	NewJoint.joint = PhysX_man.mScene->createRevoluteJoint(FirstBody,SecondBody,join_descr);
	if(name == "")
		NewJoint.name = "Revolute " + FirstBody->getName() + " <==> " + SecondBody->getName();
	else
		NewJoint.name = name;
	joint.push_back(NewJoint);
	NewJoint.joint->getJointType();
}
*/
void Construction::CreateFixedLink(ConstructionRigidElement* FirstBody, ConstructionRigidElement* SecondBody, string name, bool buildRopes, Ogre::Vector3 ropePosA, Ogre::Vector3 ropePosB, float ropeRadius)
{
	NxOgre_NamedJoint NewJoint;
	NewJoint.joint = PhysX_man.mScene->createFixedJoint(FirstBody->physXBody,SecondBody->physXBody);
	if(name == "")
		NewJoint.name = "Fixed "+ FirstBody->name + " <==> " + SecondBody->name;
	else
		NewJoint.name = name;
	joint.push_back(NewJoint);

	// register secondbody as a new linkedbody in firsybody list
	FirstBody->linkedBodies.push_back(SecondBody->physXBody);

	// build ropes to visualize attachment if needed
	if (buildRopes)
	{

		for (int i = 0; i < 5; i++) // 5 ropes built per body
		{
			
			Ogre::Entity* Rope;
			Ogre::SceneNode		*RopeA_n,
								*RopeB_n;

			// Rope A
			Rope = sys0.mgr->createEntity("Rope" + toOgStr(sys0.timer.getMicroseconds()),"rope.mesh");
			RopeA_n = static_cast<Critter::Body*>(joint.back().joint->getFirstRigidBody())->getNode()->getSceneNode()->createChildSceneNode();
			Ogre::Vector3 massCenterA = static_cast<Critter::Body*>(joint.back().joint->getFirstRigidBody())->getNode()->getSceneNode()->convertWorldToLocalPosition(Ogre::Vector3(	static_cast<Critter::Body*>(FirstBody->physXBody)->getCMassGlobalPosition().x,
																																													static_cast<Critter::Body*>(FirstBody->physXBody)->getCMassGlobalPosition().y,
																																													static_cast<Critter::Body*>(FirstBody->physXBody)->getCMassGlobalPosition().z));
			massCenterA.x = 0;
			massCenterA.z = 0;

			RopeA_n->attachObject(Rope);
			RopeA_n->setInheritScale(false);

			Ogre::Vector3 PosA = RopeA_n->convertWorldToLocalPosition(ropePosA) - RopeA_n->convertWorldToLocalPosition(ropePosA).normalisedCopy()*i*0.24f * ropeRadius;
			PosA.x = 0;
			PosA.z = 0;

			RopeA_n->setPosition(PosA*0.9f);
			RopeA_n->lookAt(massCenterA, Ogre::Node::TransformSpace::TS_PARENT);
			RopeA_n->setScale(ropeRadius, ropeRadius, ropeRadius*1.3);
			ropeList[&joint.back()].first.push_back(RopeA_n);
			
			
			// Rope B
			Rope = sys0.mgr->createEntity("Rope" + toOgStr(sys0.timer.getMicroseconds()),"rope.mesh");
			RopeB_n = static_cast<Critter::Body*>(joint.back().joint->getSecondRigidBody())->getNode()->getSceneNode()->createChildSceneNode();
			Ogre::Vector3 massCenterB = static_cast<Critter::Body*>(joint.back().joint->getSecondRigidBody())->getNode()->getSceneNode()->convertWorldToLocalPosition(Ogre::Vector3(	static_cast<Critter::Body*>(SecondBody->physXBody)->getCMassGlobalPosition().x,
																																														static_cast<Critter::Body*>(SecondBody->physXBody)->getCMassGlobalPosition().y,
																																														static_cast<Critter::Body*>(SecondBody->physXBody)->getCMassGlobalPosition().z));
			massCenterB.x = 0;
			massCenterB.z = 0;

			RopeB_n->attachObject(Rope);
			RopeB_n->setInheritScale(false);

			Ogre::Vector3 PosB = RopeB_n->convertWorldToLocalPosition(ropePosB) - RopeB_n->convertWorldToLocalPosition(ropePosB).normalisedCopy()*i*0.24f * ropeRadius;
			PosB.x = 0;
			PosB.z = 0;

			RopeB_n->setPosition(PosB*0.9f);
			RopeB_n->lookAt(massCenterB, Ogre::Node::TransformSpace::TS_PARENT);
			RopeB_n->setScale(ropeRadius, ropeRadius, ropeRadius*1.3);
			ropeList[&joint.back()].first.push_back(RopeB_n);

		}
	}
}


void Construction::DestroyJoint(NxOgre::Joint* Joint)
{
	// PhysX deletion
	Joint->setBreakable(0.00001f,0.000001f);
	//PhysX_man.mScene->destroyJoint(Joint);  // don't work (NxOgre bug)

	//Unregistering from construction's joint list
	for (Ogre::uint i = 0; i<joint.size(); ++i)
		if(joint[i].joint == Joint)
		{
			//NxOgre::Joint* jointToDel = joint[i].joint;
			joint.erase(joint.begin()+i);
			//PhysX_man.mScene->destroyJoint(jointToDel);
		}
}


void Construction::Rotate(Ogre::Quaternion NewRelativeOrentation)
{
	//Ogre::Matrix3 NewRelativeOrentationMat;
//	NewRelativeOrentation = element[0]->getNode()->getSceneNode()->convertWorldToLocalOrientation(NewRelativeOrentation);
	//NewRelativeOrentation.ToRotationMatrix(NewRelativeOrentationMat);
	ldbg.send("Construction rotate : using first element as root, TODO : correct this");
	//root->setGlobalOrientationQuat(root->getGlobalOrientationQuat() * NewRelativeOrentation); //Better not to interfere whith critter's job

	//ldbg.send(name + " element[0]->getNode()->getPosition() : " + toOgStr(element[0]->getNode()->getPosition()));
	for (Ogre::uint i = 0; i<element.size(); ++i)
	{
		//Ogre::Vector3 getGlobalPositionOgre = Ogre::Vector3(element[i]->getGlobalPosition().x, element[i]->getGlobalPosition().y,element[i]->getGlobalPosition().z);
		//ldbg.send("element[" + toOgStr(i) + "]->getGlobalPosition() : " + toOgStr(getGlobalPositionOgre));
		//ldbg.send("NewRelativeOrentationMat * getGlobalPositionOgre : " + toOgStr(NewRelativeOrentationMat * getGlobalPositionOgre));
		if(element[i]->isRigidBodyBased())
			static_cast<ConstructionRigidElement*>(element[i])->physXBody->setGlobalOrientationQuat(static_cast<ConstructionRigidElement*>(element[i])->physXBody->getGlobalOrientationQuat() * NewRelativeOrentation);
		//element[i]->setGlobalPosition(static_cast<NxOgre::Vec3>(element[0]->getNode()->getPosition() + NewRelativeOrentationMat * getGlobalPositionOgre));
	}
}

void Construction::setOrientation(Ogre::Quaternion NewAbsoluteOrentation)
{
	root->setGlobalOrientationQuat(NewAbsoluteOrentation);
	
	for (Ogre::uint i = 0; i<element.size(); ++i)
		if(element[i]->isRigidBodyBased())
		{
			static_cast<ConstructionRigidElement*>(element[i])->physXBody->setGlobalOrientationQuat(static_cast<ConstructionRigidElement*>(element[i])->physXBody->getGlobalOrientationQuat() * root->getGlobalOrientationQuat());
			static_cast<ConstructionRigidElement*>(element[i])->physXBody->setGlobalPosition(root->getGlobalPosition() + static_cast<ConstructionRigidElement*>(element[i])->physXBody->getCMassLocalPosition());
		}
}

void Construction::Move(Ogre::Vector3 NewRelativePosition)
{
	for (Ogre::uint i = 0; i<element.size(); ++i)
		if(element[i]->isRigidBodyBased())
		{
			NxOgre::Vec3 pos = NxOgre::Vec3(static_cast<ConstructionRigidElement*>(element[i])->physXBody->getNode()->getSceneNode()->getPosition() + NewRelativePosition) + root->getGlobalPosition();
			static_cast<ConstructionRigidElement*>(static_cast<ConstructionRigidElement*>(element[i]))->physXBody->setGlobalPose(NxOgre::Matrix44(pos));
		}

}

void Construction::setPosition(Ogre::Vector3 NewAbsolutePosition)
{	
	for (Ogre::uint i = 0; i<element.size(); ++i)
		if(element[i]->isRigidBodyBased())
		{
			NxOgre::Vec3 pos = NxOgre::Vec3(static_cast<ConstructionRigidElement*>(element[i])->physXBody->getNode()->getSceneNode()->getPosition()+ NewAbsolutePosition)  ;
			static_cast<ConstructionRigidElement*>(element[i])->physXBody->setGlobalPose(NxOgre::Matrix44(pos));
		}
}

NamedDataPool* Construction::Serialize()
{
	NamedDataPool* out = new NamedDataPool();

	out->Name = "Construction";

	out->String_Data.push_back(NamedString("name", name));

	NamedDataPool* temp;
	for (int i = 0; i < element.size(); ++i)
	{
		temp = element[i]->Serialize();
		out->SubPool.push_back(*temp);
	}

	return out;
}



ConstructionElement* Construction::getElementFromBody(NxOgre::RigidBody* target)
{
	for (int i = 0; i < element.size(); ++i)
		if (element[i]->isRigidBodyBased())
			if (target == static_cast<ConstructionRigidElement*>(element[i])->physXBody)
				return element[i];			// Founded requested element

	// Search error
	return NULL;
}



Construction* Construction::MakeFromDataPool(NamedDataPool& pool)
{
	Construction* out = new Construction();

	// build elements
	for (int i = 0; i< pool.SubPool.size(); ++i)
	{
		//  type based construction. Pass subpool containing all needed informations
		if ( pool.SubPool[i].Name == "Trunk")
			out->element.push_back (Trunk::MakeFromDataPool(pool.SubPool[i], out));
		else if (pool.SubPool[i].Name == "Menhir")
			out->element.push_back (Menhir::MakeFromDataPool(pool.SubPool[i], out));
	}
	// link elements
	for (int i = 0; i< out->element.size(); ++i)
	{
		// do all listed links for this element
		int linkcount = IntFromName(pool.SubPool[i], "linkedBodiesCount");
		for (int j = 0; j < linkcount; j++)
		{
			string secondObjName = StringFromName (pool.SubPool[i], "linkedBody" + toOgStr(j));
			ConstructionRigidElement* secondObj;

			// serach object from name
			for (int k = 0; k < out->element.size(); ++k)
				if (out->element[k]->name == secondObjName)
				{
					// when is this triggrered ?
					ldbg.send("out->element[k]->name == secondObjName with k = "+ toOgStr(k));
					secondObj = static_cast<ConstructionRigidElement*>(out->element[k]);
					break;
				}

			//finally create link
			out->CreateFixedLink ( static_cast<ConstructionRigidElement*>(out->element[i]),secondObj);
		}
	}

	return out;
}

























































































































