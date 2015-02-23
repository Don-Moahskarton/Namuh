#include "Namuh_Main_H.h"

using namespace Ogre;


DiskDescription::DiskDescription() : materialName(""), isCooked(false)
{

}

bool DiskDescription::Cook()
{
	// TODO : cook it
	ldbg.send("(Disk Cokking) TODO : Proper cooking");


	// Protections
	if(radius == 0) radius = 0.001f;
	if(thickness == 0) thickness = 0.001f;



	// now this description is cooked.
	isCooked = true;
	
	return true;
}

Disk::Disk(DiskDescription* param)
{
	// rigid element
	rigidBodyBasedFlag = true;
	name = param->name;

	if (!param->isCooked)
	{
		ldbg.send("(Disk Constructor) Warning : param isn't cooked ( param.name == " + param->name + "), cook it to improve constructor performance.");
		param->Cook();
	}
	ldbg.send("(Disk Constructor) TODO : Proper destructor, and call it in  CstrMan::Destroy");

	// Positon root at the requested position if it is the first element of the construction
	if (!param->container->element.size())
	{
		param->container->root->getNode()->setPosition(param->position);
		ldbg.send(param->container->name + " root pos is now : " + toOgStr(param->container->root->getNode()->getPosition()));
	}
	else
		ldbg.send("Container->element.size() : " + toOgStr(param->container->element.size()));

	param->physXFields.mNode = new Critter::Node(sys0.mgr,param->container->root->getNode()->getSceneNode(),PhysX_man.mRenderSystem); // get the disk linked to the root of the containing construction
	// m = µ * pi * r² * h 
	param->physXFields.mMass = 2000*3.1415926*(param->radius)*(param->radius)*param->thickness; // wood volumic mass : 2000 kg/m^3
	param->physXFields.mLinearDamping = 0.12f;  
	param->physXFields.mAngularDamping = 0.11f;


	// if provided name is empty, generate one
	if (param->name == "")
	{
		param->physXFields.mName = "Disk"+ StringConverter::toString(sys0.timer.getMilliseconds());
		param->name = param->physXFields.mName;
	}
	else 
		param->physXFields.mName = param->name;

	// Create the disk itself

	ldbg.send("(Disk Constructor) TODO : Make a real Disk NxMesh");
	NxOgre::WheelDescription ShapeDesc = NxOgre::WheelDescription();
	// ShapeDesc.mLocalPose = NxOgre::Matrix44(NxOgre::Vec3(0.0f,0.0f,1.0f * param->radius/2 ));		// half radius offset, used to correct a bug with trunks, dunno if disks are concerned
	ShapeDesc.mRadius = param->radius;
	ShapeDesc.mWheelFlags = NxOgre::Enums::WheelFlags_UnscaledSpringBehaviour;

	Critter::Body*	disk = PhysX_man.mRenderSystem->createBody(ShapeDesc,NxOgre::Vec3(param->position), "Disk.mesh", param->physXFields);
	disk->setGlobalOrientationQuat(NxOgre::Quat(param->orientation));
	disk->getNode()->getSceneNode()->scale(param->radius, param->thickness/3, param->radius);	// adjust to original mesh dimentions

	physXBody = disk;

	// container registration
	param->container->element.push_back(this);	
}

NamedDataPool* Disk::Serialize()
{
	NamedDataPool* out = new NamedDataPool();

	// TODO : fill the datapool with interesting things

	return out;
} 


Disk* Disk::MakeFromDataPool(NamedDataPool&)
{
	HideDescription loadedDesc;



	return NULL;

}



















































































