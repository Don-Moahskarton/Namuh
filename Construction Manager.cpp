#include "Namuh_Main_H.h"

using namespace Ogre;

CstrMan CstrMgr;

// Code des méthodes de la classe
CstrMan::CstrMan(void)
{
	// Definition de la syntaxe des fichier de Construction
	ParserBalise *Construction = new ParserBalise;
	Construction->Name = "Construction";
	Construction->StringKeywords.push_back("name");
	Syntax.push_back(Construction);

		ParserBalise Element = {"Trunk"};
		Syntax.back()->SousBalises.push_back(Element);

			Syntax.back()->SousBalises.back().IntKeywords.push_back("linkedBodiesCount");
			Syntax.back()->SousBalises.back().Vector3Keywords.push_back("position");
			Syntax.back()->SousBalises.back().FloatKeywords.push_back("length");
			Syntax.back()->SousBalises.back().FloatKeywords.push_back("volume");
			Syntax.back()->SousBalises.back().FloatKeywords.push_back("radius");
			Syntax.back()->SousBalises.back().StringKeywords.push_back("name");
			Syntax.back()->SousBalises.back().StringKeywords.push_back("materialname");

			// find how to deal with linkedBodyX Because X varies from 0 to +oo and will confuse the parser.
			// (like a patch to accept foreign keyword if preceded by a type declaration, and keep retrocompatibility)

			ParserBalise Orientation = {"orientation"};
				Syntax.back()->SousBalises.back().SousBalises.push_back(Orientation);
				Syntax.back()->SousBalises.back().SousBalises.back().FloatKeywords.push_back("w");
				Syntax.back()->SousBalises.back().SousBalises.back().FloatKeywords.push_back("x");
				Syntax.back()->SousBalises.back().SousBalises.back().FloatKeywords.push_back("y");
				Syntax.back()->SousBalises.back().SousBalises.back().FloatKeywords.push_back("z");

				
		Element = *(new ParserBalise());
		Element.Name = "Menhir";
		Syntax.back()->SousBalises.push_back(Element);

			Syntax.back()->SousBalises.back().IntKeywords.push_back("linkedBodiesCount");
			Syntax.back()->SousBalises.back().Vector3Keywords.push_back("position");
			Syntax.back()->SousBalises.back().FloatKeywords.push_back("volume");
			Syntax.back()->SousBalises.back().StringKeywords.push_back("name");
			Syntax.back()->SousBalises.back().StringKeywords.push_back("materialName");

			// find how to deal with linkedBodyX Because X varies from 0 to +oo and will confuse the parser.
			// (like a patch to accept foreign keyword if preceded by a type declaration, and keep retrocompatibility)

			Orientation = *(new ParserBalise());
			Orientation.Name = "orientation";
				Syntax.back()->SousBalises.back().SousBalises.push_back(Orientation);
				Syntax.back()->SousBalises.back().SousBalises.back().FloatKeywords.push_back("w");
				Syntax.back()->SousBalises.back().SousBalises.back().FloatKeywords.push_back("x");
				Syntax.back()->SousBalises.back().SousBalises.back().FloatKeywords.push_back("y");
				Syntax.back()->SousBalises.back().SousBalises.back().FloatKeywords.push_back("z");

		RocherMesh = NULL;

}
CstrMan::~CstrMan(void)
{
}
 // loads a construction from a file and place it in the world at specified posistion en
Construction* CstrMan::Load(string FileName, Ogre::Vector3 CstrPosition, Ogre::Quaternion CstrOrientation)
{
	if(FileName != "")
	{
		std::vector<NamedDataPool> ConstructionDataPoolArray = Map_Parser.ParseFile(Syntax,ProfileMgr.ProfilePath+"Constructions/"+FileName);
		NamedDataPool ConstructionDataPool = ConstructionDataPoolArray[FindPoolIndexFromName(ConstructionDataPoolArray,"Construction")]; // On ne garde que la Pool construction
/*		LoadedConstructions.push_back(new Construction);


		// Construit tout les nouveaux éléments
		for(	std::vector<NamedDataPool>::iterator PoolIterator = ConstructionDataPool.SubPool.begin();
				PoolIterator != ConstructionDataPool.SubPool.end();
				++PoolIterator)
			if((*PoolIterator).Name == "Element")
			{
				Ogre::Quaternion ElemOrientation = Ogre::Quaternion(	FloatFromName((*PoolIterator).SubPool[SubPoolsIndexesFromName((*PoolIterator), "Orientation").back()],"w"),
																		FloatFromName((*PoolIterator).SubPool[SubPoolsIndexesFromName((*PoolIterator), "Orientation").back()],"x"),
																		FloatFromName((*PoolIterator).SubPool[SubPoolsIndexesFromName((*PoolIterator), "Orientation").back()],"y"),
																		FloatFromName((*PoolIterator).SubPool[SubPoolsIndexesFromName((*PoolIterator), "Orientation").back()],"z"));

				if(StringFromName(*PoolIterator,"Type") == "Rondin")
					this->CreateTrunk(	LoadedConstructions.back(),
											Vector3FromName(*PoolIterator,"Position"),
											ElemOrientation,
											FloatFromName(*PoolIterator,"Diametre"),				
											FloatFromName(*PoolIterator,"Longueur"),
											StringFromName(*PoolIterator,"Name"));
				if(StringFromName(*PoolIterator,"Type") == "Rocher")
					this->Create_rocher(	LoadedConstructions.back(),
											Vector3FromName(*PoolIterator,"Position"),
											ElemOrientation,
											StringFromName(*PoolIterator,"Name"));
			}

		// Construit tout les nouveaux link
		for(	std::vector<NamedDataPool>::iterator PoolIterator = ConstructionDataPool.SubPool.begin();
				PoolIterator != ConstructionDataPool.SubPool.end();
				++PoolIterator)
			if ((*PoolIterator).Name == "Link")
			{
				Critter::Body	*FirstBdy = NULL,
								*SecondBdy = NULL;
				// Cherche le premier body
				string firstbdyName = StringFromName(*PoolIterator,"FirstBodyName");
				for (	std::vector<Critter::Body*>::iterator itt = LoadedConstructions.back()->element.begin();
						itt != LoadedConstructions.back()->element.end();
						++itt)
						if((*itt)->getName() == firstbdyName)
							FirstBdy = *itt;
				// Cherche le second body
				string secondbdyName = StringFromName(*PoolIterator,"SecondBodyName");
				for (	std::vector<Critter::Body*>::iterator itt = LoadedConstructions.back()->element.begin();
						itt != LoadedConstructions.back()->element.end();
						++itt)
						if((*itt)->getName() == secondbdyName)
							SecondBdy = *itt;
				string LnkName = StringFromName(*PoolIterator,"LinkName");
				LoadedConstructions.back()->CreateFixedLink(FirstBdy, SecondBdy, LnkName);
			}

		// Applique la rotation et l'orientation demandée
			LoadedConstructions.back()->setOrientation(CstrOrientation);
			LoadedConstructions.back()->setPosition(CstrPosition);
			*/
		LoadedConstructions.push_back (Construction::MakeFromDataPool (ConstructionDataPool));
		return LoadedConstructions.back();
	}
	else
		return NULL;
}

void CstrMan::SaveAs(Construction* cstr, string FileName)
{
	std::vector<NamedDataPool*> Construction_data_vector;
	Construction_data_vector.push_back(cstr->Serialize());
	
	// écris le fichier
	if(FileName == "")
		FileName = "Untitled" + StringConverter::toString(sys0.timer.getMicroseconds());
	File_Serialiser.WriteFile(Construction_data_vector,ProfileMgr.ProfilePath+"Constructions/"+FileName+".nmh");

}


Construction* CstrMan::CreateNew(void)
{
	LoadedConstructions.push_back(new Construction);
	return LoadedConstructions.back();
}

void CstrMan::Destroy(Construction* Cstr)
{
	// Unregister all links				
	while (Cstr->joint.size())
			Cstr->DestroyJoint(Cstr->joint.back().joint);

	// détruits les bodys	
	while (Cstr->element.size())
	{
		// delete Ogre's node
		//sys0.mgr->destroySceneNode(Cstr->element.back()->getNode()->getSceneNode());

		// Delete PhysX's body (Yes, we can cast a Rigidbody to a Critter's Body)
		//PhysX_man.mRenderSystem->destroyBody(Cstr->element.back());

		// unregister
		Cstr->element.pop_back();
	}
}


Trunk* CstrMan::CreateTrunk( TrunkDescription* param)
{
	return new Trunk(param);
}

Menhir* CstrMan::CreateMenhir( MenhirDescription*  param)
{
	return new Menhir(param);
}



void CstrMan::Create_triangle(Construction* container, Ogre::Vector3 position, Ogre::Quaternion orientation, float radius, float length, string name)
{

	if (name == "")
		name = "Triangle " + toOgStr(sys0.timer.getMilliseconds());

	Ogre::Matrix3	localToGlobal = Ogre::Matrix3::ZERO,
		GLobalToLocal;
	orientation.ToRotationMatrix(localToGlobal);
	GLobalToLocal = localToGlobal.Inverse();


	TrunkDescription	trunk1Desc,
						trunk2Desc,
						trunk3Desc;

	// trunks descriptions
	// 0.43 = sqrt(3)/4
	trunk1Desc.container = container;
	trunk1Desc.orientation = orientation * Ogre::Quaternion(Degree(90), Vector3::UNIT_X) * Ogre::Quaternion(Degree(90), Vector3::UNIT_Z);
	trunk1Desc.position = position + localToGlobal * 0.5 * Ogre::Vector3(0.0f,0.0f, 0.43f * length);
	trunk1Desc.radius = radius;
	trunk1Desc.length = length;
	trunk1Desc.name =  name + "_Trunk1";

	trunk2Desc.container = container;
	trunk2Desc.orientation = orientation* Ogre::Quaternion(Degree(90), Vector3::UNIT_X) * Ogre::Quaternion(Degree(-30), Vector3::UNIT_Z);
	trunk2Desc.position = position + localToGlobal * 0.5  * Ogre::Vector3(0.37f * length,0.0f, -0.22f * length);
	trunk2Desc.radius = radius;
	trunk2Desc.length = length;
	trunk2Desc.name =  name + "_Trunk2";

	trunk3Desc.container = container;
	trunk3Desc.orientation = orientation * Ogre::Quaternion(Degree(90), Vector3::UNIT_X) * Ogre::Quaternion(Degree(30), Vector3::UNIT_Z);
	trunk3Desc.position = position + localToGlobal * 0.5  * Ogre::Vector3(-0.37f * length,0.0f, -0.22f * length);
	trunk3Desc.radius = radius;
	trunk3Desc.length = length;
	trunk3Desc.name =  name + "_Trunk3";

	// les trois rondins du triangle
	ConstructionRigidElement	*firstTrunk = CreateTrunk(&trunk1Desc),
								*secondTrunk = CreateTrunk(&trunk2Desc),
								*thirdTrunk = CreateTrunk(&trunk3Desc);

	
	// Triangl internal links
	container->CreateFixedLink(	firstTrunk,
								secondTrunk,
								"triangle" + toOgStr(sys0.timer.getMilliseconds()) + " Fixed " + firstTrunk->name + " <==> " + secondTrunk->name,
								true,
								position + localToGlobal * 0.5 * Ogre::Vector3(0.0f,0.0f, -0.43f * length),
								position + localToGlobal * 0.5 * Ogre::Vector3(0.0f,0.0f, -0.43f * length),
								radius);
	container->CreateFixedLink(	secondTrunk, 
								thirdTrunk, 
								"triangle" + toOgStr(sys0.timer.getMilliseconds()) + " Fixed " + secondTrunk->name + " <==> " + thirdTrunk->name,
								true,
								position + localToGlobal * 0.5  * Ogre::Vector3(-0.37f * length,0.0f, 0.22f * length),
								position + localToGlobal * 0.5  * Ogre::Vector3(-0.37f * length,0.0f, 0.22f * length),
								radius);
	container->CreateFixedLink(	thirdTrunk, 
								firstTrunk,
								"triangle" + toOgStr(sys0.timer.getMilliseconds()) + " Fixed " + thirdTrunk->name + " <==> " + firstTrunk->name,
								true,
								position + localToGlobal * 0.5  * Ogre::Vector3(0.37f * length,0.0f, 0.22f * length),
								position + localToGlobal * 0.5  * Ogre::Vector3(0.37f * length,0.0f, 0.22f * length),
								radius);	// TODO : change CreateFixedLink to accept rope coordinates in local space. (Give an argument with the transsformspace enum)
								
}

Disk* CstrMan::Create_disk( DiskDescription* param)
{
	return new Disk(param);
}


NxOgre::Cloth* CstrMan::Create_hide(	Construction* container,
										Ogre::Vector3 firstCornerPos, 
										Critter::Body* firstCornerToLink, 
										Ogre::Vector3 secondCornerPos, 
										Critter::Body* secondCornerToLink, 
										Ogre::Vector3 thirdCornerPos, 
										Critter::Body* thirdCornerToLink, 
										Ogre::Vector3 fourthCornerPos, 
										Critter::Body* fourthCornerToLink,
										bool isWing,
										bool isBounce,
										string name)
{
	ldbg.send("TODO : implement balloons (with pressure, inflated thing etc..) (can also be used with an invisible material to simulate imprisoned underwater air)");

	// Positon root at the requested position if it is the first element of the construction
	if (!container->element.size())
		container->root->getNode()->setPosition(firstCornerPos);

	// PhysX description
	HideDescription* descr = new HideDescription();
	descr->container = container;
	descr->PhysX_descr.mThickness = 1;
	descr->PhysX_descr.mFriction = 1;
	descr->cornerPos[0] = firstCornerPos;
	descr->cornerPos[1] = secondCornerPos;
	descr->cornerPos[2] = thirdCornerPos;
	descr->cornerPos[3] = fourthCornerPos;

	descr->linkedBodies[0] = firstCornerToLink;
	descr->linkedBodies[1] = secondCornerToLink;
	descr->linkedBodies[2] = thirdCornerToLink;
	descr->linkedBodies[3] = fourthCornerToLink;

	descr->materialName = "hide";

	descr->isWing = isWing;
	descr->isBounce = isBounce;
	
	//  descr->PhysX_descr.mPressure = 0.8;
	//  descr->PhysX_descr.mFlags |= NxOgre::Enums::ClothFlags_Pressure;
	descr->PhysX_descr.mFlags |= NxOgre::Enums::ClothFlags_BendingResistance;
	descr->PhysX_descr.mFlags |= NxOgre::Enums::ClothFlags_OrthogonalBendingResistance;
	descr->PhysX_descr.mFlags |= NxOgre::Enums::ClothFlags_TwoWayCollisions;
	descr->PhysX_descr.mGlobalPose.set(Vec3::ZERO);			


	if (name == "")
		descr->PhysX_descr.mName = "Hide"+ toOgStr(sys0.timer.getMicroseconds());
	else 
		descr->PhysX_descr.mName = name;

	descr->Cook();

	Hide* generatedHide = new Hide(descr);

	// L'enregistre comme partie intégrante de la construction demandée
	container->element.push_back(generatedHide);	

	return NULL;	// wait what ?

}



























































































































































