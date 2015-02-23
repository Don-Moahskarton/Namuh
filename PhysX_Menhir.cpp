#include "Namuh_Main_H.h"

using namespace Ogre;


MenhirDescription::MenhirDescription() :  materialName(""), position(Ogre::Vector3::ZERO), volume(0), isCooked(false)
{
	PhysX_descr.reset();
}

bool MenhirDescription::Cook()
{
	// now this description is cooked.
	isCooked = true;
	
	return true;	// correct descr
}

Menhir::Menhir(MenhirDescription* param)
{
	// rigid element
	rigidBodyBasedFlag = true;
	name = param->name;

	if (!param->isCooked)
	{
		ldbg.send("(Menhir Constructor) Warning : param isn't cooked ( param.name == " + param->name + "), cook it to improve constructor performance.");
		param->Cook();
	}



	// Positon root at the requested position if it is the first element of the construction
	if (!param->container->element.size())
		param->container->root->getNode()->setPosition(param->position);

	// Faire un BodyDescription pour chaque nouveau Body
	Critter::BodyDescription menhirDescription;
	menhirDescription.mNode = new Critter::Node(sys0.mgr,param->container->root->getNode()->getSceneNode(),PhysX_man.mRenderSystem); // accroche le rocher au noeud racine de la construction
	menhirDescription.mMass = 5850;
	menhirDescription.mLinearDamping = 0.12f;  
	menhirDescription.mAngularDamping = 0.11f;
	if (param->name == "")
		menhirDescription.mName = "Menhir"+ toOgStr(sys0.timer.getMilliseconds());
	else 
		menhirDescription.mName = param->name;

	// Crée l'objet en lui meme
	if (PhysX_man.MenhirMesh == NULL) // if the physX geometry isn't loaded, load it
		PhysX_man.MenhirMesh = NxOgre::MeshManager::getSingleton()->load("ogre://General/Rocher.nxs");

	Critter::Body* rocher = PhysX_man.mRenderSystem->createBody(NxOgre::ConvexDescription(PhysX_man.MenhirMesh),NxOgre::Vec3(param->position), "rocher.mesh",menhirDescription);  //bug Oo
	rocher->setGlobalOrientationQuat(NxOgre::Quat(param->orientation));
	//rocher->setSolverIterationCount(80); // Lent

	physXBody = rocher;
	descr  = *param;
	// L'enregistre comme partie intégrante de la construction demandée
	param->container->element.push_back(this);	

}

NamedDataPool* Menhir::Serialize()
{
	NamedDataPool* out = new NamedDataPool();

	out->Name = "Menhir";

	out->String_Data.push_back(NamedString("name", name));
	out->String_Data.push_back(NamedString("materialName", descr.materialName));
	// look for constructions elements names of linkedbodies
	out->Int_Data.push_back(NamedInt("linkedBodiesCount",  int(linkedBodies.size())));
	for (Ogre::uint i = 0; i < linkedBodies.size(); ++i)
		for (Ogre::uint j = 0; j < descr.container->element.size(); ++j)
			if (descr.container->element[j]->isRigidBodyBased() && linkedBodies[i] == static_cast<ConstructionRigidElement*>(descr.container->element[j])->physXBody)
				out->String_Data.push_back(NamedString("linkedBody" + toOgStr(i),  descr.container->element[j]->name));;


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

Menhir* Menhir::MakeFromDataPool(NamedDataPool& pool, Construction* container)
{
	// Fills in the description coressponding to the provided datapool
	MenhirDescription* loadedDesc = new MenhirDescription();


	loadedDesc->position = Vector3FromName(pool, "position");
	loadedDesc->orientation = Ogre::Quaternion(FloatFromName(pool.SubPool[0],"w"), FloatFromName(pool.SubPool[0],"x"), FloatFromName(pool.SubPool[0],"y"), FloatFromName(pool.SubPool[0],"z")  );

	loadedDesc->name = StringFromName(pool, "name");

	loadedDesc->container = container;
	// Make that trunk
	loadedDesc->Cook ();
	Menhir* out = new Menhir(loadedDesc);

	// Won't make links here, it must be done when all components have been built.
	// So links will be made in construction class.

	return out;
}



















































































