#include "Namuh_Main_H.h"

using namespace Ogre;


TrunkDescription::TrunkDescription() : 
											materialName(""), 
											name(""),
											radius(0), 
											length(0), 
											orientation(Ogre::Quaternion::IDENTITY), 
											position(Ogre::Vector3::ZERO), 
											container(NULL),
											physXFields(Critter::BodyDescription()), 
											volume(0), 
											isCooked(false)
{
	// Simple constructor
}

bool TrunkDescription::Cook()
{
	// TODO : cook it
	ldbg.send("(Trunk Constructor) TODO : Proper trunk cooking");

	// Protections
	if(radius == 0) radius = 0.001f;
	if(length == 0) length = 0.001f;

	// Faire un BodyDescription pour chaque nouveau Body

	// now this description is cooked.
	isCooked = true;
	return true;	// correct descr
	
}

Trunk::Trunk(TrunkDescription* param) 
{
	// rigid element
	rigidBodyBasedFlag = true;
	name = param->name;


	if (!param->isCooked)
	{
		ldbg.send("(Trunk Constructor) Warning : param isn't cooked ( param.name == " + param->name + "), cook it to improve constructor performance.");
		param->Cook();
	}
	ldbg.send("(Trunk Constructor) TODO : Proper destructor, and call it in  CstrMan::Destroy");


	// Positon root at the requested position if it is the first element of the construction
	if (!param->container->element.size())
	{
		param->container->root->getNode()->setPosition(param->position);
		ldbg.send(param->container->name + " root pos is now : " + toOgStr(param->container->root->getNode()->getPosition()));
	}
	else
		ldbg.send("Container->element.size() : " + toOgStr(param->container->element.size()));

	
	param->physXFields.mNode = new Critter::Node(sys0.mgr,param->container->root->getNode()->getSceneNode(),PhysX_man.mRenderSystem); // get the trunk linked to the root of the containing construction
	// v = pi * r² * h 
	param->volume = 3.1415926*(param->radius)*(param->radius)*param->length;

	// m = µ * v
	param->physXFields.mMass = 650 * param->volume; // wood volumetric mass : 650 kg/m^3
	param->physXFields.mLinearDamping = 0.42f;  
	param->physXFields.mAngularDamping = 0.31f;

	// if provided name is empty, generate one
	if (param->name == "")
	{
		param->physXFields.mName = "Trunk"+ StringConverter::toString(sys0.timer.getMilliseconds());
		param->name = param->physXFields.mName;
	}
	else 
		param->physXFields.mName = param->name;

	// Create the trunk itself
	NxOgre::CapsuleDescription ShapeDesc = NxOgre::CapsuleDescription(param->radius, param->length);
	//ShapeDesc.mLocalPose = NxOgre::Matrix44(NxOgre::Vec3(0.0f,0.0f,1.0f * param->radius/2 ));		// half radius offset
	ldbg.send("(Trunk Constructor)  TODO : Correct that bug which makes the collision capsule lower on the world Y axis than the ogre visible mesh is");
	
	Critter::Body*	rondin = PhysX_man.mRenderSystem->createBody(ShapeDesc,NxOgre::Vec3(param->position), "Rondin.mesh", param->physXFields);
	rondin->setGlobalOrientationQuat(NxOgre::Quat(param->orientation));
	rondin->getNode()->getSceneNode()->scale(param->radius, param->length/10, param->radius);	// adjust to original mesh dimentions

	physXBody = rondin;
	descr  = *param;

	PhysXfloater.add(rondin);
	rondin->setSolverIterationCount(80); // Lent

	// container registration
	param->container->element.push_back(this);	
}

NamedDataPool* Trunk::Serialize()
{
	NamedDataPool* out = new NamedDataPool();

	out->Name = "Trunk";

	out->String_Data.push_back(NamedString("name", name));
	out->String_Data.push_back(NamedString("materialName", descr.materialName));
	// look for constructions elements names of linkedbodies
	out->Int_Data.push_back(NamedInt("linkedBodiesCount",  int(linkedBodies.size())));
	for (Ogre::uint i = 0; i < linkedBodies.size(); ++i)
		for (Ogre::uint j = 0; j < descr.container->element.size(); ++j)
			if (descr.container->element[j]->isRigidBodyBased() && linkedBodies[i] == static_cast<ConstructionRigidElement*>(descr.container->element[j])->physXBody)
				out->String_Data.push_back(NamedString("linkedBody" + toOgStr(i),  descr.container->element[j]->name));;


	out->Float_Data.push_back(NamedFloat("radius", descr.radius));
	out->Float_Data.push_back(NamedFloat("length", descr.length));
	out->Float_Data.push_back(NamedFloat("volume", descr.volume));

	out->SubPool.push_back(*(new NamedDataPool()));	// Orientation quaternion Datapool
	out->SubPool.back().Name = "orientation";
	out->SubPool.back().Float_Data.push_back(NamedFloat("w", physXBody->getGlobalOrientationQuat().w));
	out->SubPool.back().Float_Data.push_back(NamedFloat("x", physXBody->getGlobalOrientationQuat().x));
	out->SubPool.back().Float_Data.push_back(NamedFloat("y", physXBody->getGlobalOrientationQuat().y));
	out->SubPool.back().Float_Data.push_back(NamedFloat("z", physXBody->getGlobalOrientationQuat().z));


	out->Vector3_Data.push_back(NamedVector3("position", physXBody->getGlobalPosition().as<Vector3>()));


	// physXFields are hardcoded and mMass is deduced from volume.
	// so no need to seialize it.

	return out;
} 

Trunk* Trunk::MakeFromDataPool(NamedDataPool& pool, Construction* container )
{
	// Fills in the description coressponding to the provided datapool
	TrunkDescription* loadedDesc = new TrunkDescription();

	loadedDesc->position = Vector3FromName(pool, "position");
	loadedDesc->orientation = Ogre::Quaternion(FloatFromName(pool.SubPool[0],"w"), FloatFromName(pool.SubPool[0],"x"), FloatFromName(pool.SubPool[0],"y"), FloatFromName(pool.SubPool[0],"z")  );

	loadedDesc->name = StringFromName(pool, "name");
	loadedDesc->radius = FloatFromName(pool, "radius");
	loadedDesc->length = FloatFromName(pool, "length");

	loadedDesc->container = container;

	// Make that trunk
	loadedDesc->Cook ();
	Trunk* out = new Trunk(loadedDesc);

	// Won't make links here, it must be done when all components have been built.
	// So links will be made in construction class.
	
	return out;

}



















































































