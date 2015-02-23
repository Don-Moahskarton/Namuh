#include "Namuh_Main_H.h"

using namespace Ogre;


Faune_Entity::Faune_Entity(void)
{	
}
Faune_Entity::~Faune_Entity(void)
{
}

void Faune_Entity::initialise(Faune_Page* Containing_Page, Ogre::Vector3 RequestedPos)
{  
	Container = Containing_Page;
	AnimalEntity = sys0.mgr->createEntity("Animal" + StringConverter::toString(sys0.timer.getMicroseconds()),"Fish01.mesh");
	AnimalNode = sys0.mgr->getRootSceneNode()->createChildSceneNode();
	//AnimalNode->setScale(0.5,0.5,0.5);
	AnimalNode->attachObject(AnimalEntity);
	AnimalNode->setPosition(	RequestedPos.x,
								Ogre::Math::RangeRandom(terrain_height(RequestedPos.x, RequestedPos.z,0),Hydrax_man.sea_level),
								RequestedPos.z); //Fishes are always upside up
	AnimalNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);  //
	position = AnimalNode->getPosition();
	linearVelocity = Ogre::Vector3::ZERO;

}

void Faune_Entity::destroy(void)
{
	Container = NULL;
	AnimalNode->detachAllObjects();
	sys0.mgr->destroyEntity(AnimalEntity);
	sys0.mgr->destroySceneNode(AnimalNode);
} 


void Faune_Entity::update(float Timelapse)
{
	if(Timelapse <= 0.0f) Timelapse = 0.002f;
	// See details at http://fr.wikipedia.org/wiki/Boids
		

	// Compute attraction center (cohesion component) plus the number of enlarged neiboors
	Ogre::Vector3 AttractionCenter = Ogre::Vector3::ZERO;
	float AttractionCenterWeightstSum = 0.0f;

	if( Container->Bottom != NULL)
	{
		for (Ogre::uint i = 0; i < Container->Bottom->ContainedEntities.size(); ++i)
		{
			AttractionCenter += (Container->Bottom->ContainedEntities[i]->position - position) * (Container->Bottom->ContainedEntities[i]->position - position).squaredLength();
			AttractionCenterWeightstSum += 1 - 0.002 * (Container->Bottom->ContainedEntities[i]->position - position).squaredLength();
		}
	}
	if( Container->BottomLeft != NULL)
	{
		for (Ogre::uint i = 0; i < Container->BottomLeft->ContainedEntities.size(); ++i)
		{
			AttractionCenter += (Container->BottomLeft->ContainedEntities[i]->position - position) * (Container->BottomLeft->ContainedEntities[i]->position - position).squaredLength();
			AttractionCenterWeightstSum += 1 - 0.002 * (Container->BottomLeft->ContainedEntities[i]->position - position).squaredLength();
		}
	}
	if( Container->BottomRight != NULL)
	{
		for (Ogre::uint i = 0; i < Container->BottomRight->ContainedEntities.size(); ++i)
		{
			AttractionCenter += (Container->BottomRight->ContainedEntities[i]->position - position) * (Container->BottomRight->ContainedEntities[i]->position - position).squaredLength();
			AttractionCenterWeightstSum += 1 - 0.002 * (Container->BottomRight->ContainedEntities[i]->position - position).squaredLength();
		}
	}
	if( Container->Right != NULL)
	{
		for (Ogre::uint i = 0; i < Container->Right->ContainedEntities.size(); ++i)
		{
			AttractionCenter += (Container->Right->ContainedEntities[i]->position - position) * (Container->Right->ContainedEntities[i]->position - position).squaredLength();
			AttractionCenterWeightstSum += 1 - 0.002 * (Container->Right->ContainedEntities[i]->position - position).squaredLength();
		}
	}
	if( Container->Left != NULL)
	{
		for (Ogre::uint i = 0; i < Container->Left->ContainedEntities.size(); ++i)
		{
			AttractionCenter += (Container->Left->ContainedEntities[i]->position - position) * (Container->Left->ContainedEntities[i]->position - position).squaredLength();
			AttractionCenterWeightstSum += 1 - 0.002 * (Container->Left->ContainedEntities[i]->position - position).squaredLength();
		}
	}
	if( Container->Top != NULL)
	{
		for (Ogre::uint i = 0; i < Container->Top->ContainedEntities.size(); ++i)
		{
			AttractionCenter += (Container->Top->ContainedEntities[i]->position - position) * (Container->Top->ContainedEntities[i]->position - position).squaredLength();
			AttractionCenterWeightstSum += 1 - 0.002 * (Container->Top->ContainedEntities[i]->position - position).squaredLength();
		}
	}
	if( Container->TopLeft != NULL)
	{
		for (Ogre::uint i = 0; i < Container->TopLeft->ContainedEntities.size(); ++i)
		{
			AttractionCenter += (Container->TopLeft->ContainedEntities[i]->position - position) * (Container->TopLeft->ContainedEntities[i]->position - position).squaredLength();
			AttractionCenterWeightstSum += 1 - 0.002 * (Container->TopLeft->ContainedEntities[i]->position - position).squaredLength();
		}
	}
	if( Container->TopRight != NULL)
	{
		for (Ogre::uint i = 0; i < Container->TopRight->ContainedEntities.size(); ++i)
		{
			AttractionCenter += (Container->TopRight->ContainedEntities[i]->position - position) * (Container->TopRight->ContainedEntities[i]->position - position).squaredLength();
			AttractionCenterWeightstSum += 1 - 0.002 * (Container->TopRight->ContainedEntities[i]->position - position).squaredLength();
		}
	}
	

	if(AttractionCenterWeightstSum != 0.0f)
		AttractionCenter /= AttractionCenterWeightstSum;

	Ogre::Vector3 AttractionComponent = AttractionCenter;


	
	// Compute repultion center (separation component)
	Ogre::Vector3 RepultionTerms = Ogre::Vector3::ZERO;
	float RepulionTermsWeightstSum = 0.0f;

	float RepulionTermWeight = 0.0f;
	for (Ogre::uint i = 0; i < Container->ContainedEntities.size(); ++i)
	{
		RepulionTermWeight = 1 - 0.004 * (Container->ContainedEntities[i]->position - position).squaredLength(); // The closer is the neiboor, the more is it imortant to get away from him
		RepulionTermsWeightstSum += RepulionTermWeight;
		RepultionTerms += (Container->ContainedEntities[i]->position - position) * RepulionTermWeight;
	}

	if(RepulionTermsWeightstSum != 0.0f)
		RepultionTerms /= RepulionTermsWeightstSum;
	// Since now RepultionTerms points the center of repultion
	
	Ogre::Vector3 RepultionComponent = -1 * RepultionTerms;
	

	// Compute average speed vector (alignment component)
	Ogre::Vector3 AverageVelocity = Ogre::Vector3::ZERO;

	if( Container->Bottom != NULL)
		for (Ogre::uint i = 0; i < Container->Bottom->ContainedEntities.size(); ++i)
			if(Container->Bottom->ContainedEntities[i]->linearVelocityHistory.size())
				// on prend a la fin de l'historique pour simuler une perception retardée
				AverageVelocity += Container->Bottom->ContainedEntities[i]->linearVelocityHistory.back() * (1 - 0.002 * (Container->Bottom->ContainedEntities[i]->position - position).squaredLength()); 
	if( Container->BottomLeft != NULL)
		for (Ogre::uint i = 0; i < Container->BottomLeft->ContainedEntities.size(); ++i)
			if(Container->BottomLeft->ContainedEntities[i]->linearVelocityHistory.size())
				AverageVelocity += Container->BottomLeft->ContainedEntities[i]->linearVelocityHistory.back() * (1 - 0.002 * (Container->BottomLeft->ContainedEntities[i]->position - position).squaredLength());
	if( Container->BottomRight != NULL)
		for (Ogre::uint i = 0; i < Container->BottomRight->ContainedEntities.size(); ++i)
			if(Container->BottomRight->ContainedEntities[i]->linearVelocityHistory.size())
				AverageVelocity += Container->BottomRight->ContainedEntities[i]->linearVelocityHistory.back() * (1 - 0.002 * (Container->BottomRight->ContainedEntities[i]->position - position).squaredLength());
	if( Container->Right != NULL)
		for (Ogre::uint i = 0; i < Container->Right->ContainedEntities.size(); ++i)
			if(Container->Right->ContainedEntities[i]->linearVelocityHistory.size())
				AverageVelocity += Container->Right->ContainedEntities[i]->linearVelocityHistory.back() *(1 - 0.002 *  (Container->Right->ContainedEntities[i]->position - position).squaredLength());
	if( Container->Left != NULL)
		for (Ogre::uint i = 0; i < Container->Left->ContainedEntities.size(); ++i)
			if(Container->Left->ContainedEntities[i]->linearVelocityHistory.size())
				AverageVelocity += Container->Left->ContainedEntities[i]->linearVelocityHistory.back() *(1 - 0.002 *  (Container->Left->ContainedEntities[i]->position - position).squaredLength());
	if( Container->Top != NULL)
		for (Ogre::uint i = 0; i < Container->Top->ContainedEntities.size(); ++i)
			if(Container->Top->ContainedEntities[i]->linearVelocityHistory.size())
				AverageVelocity += Container->Top->ContainedEntities[i]->linearVelocityHistory.back() * (1 - 0.002 * (Container->Top->ContainedEntities[i]->position - position).squaredLength());
	if( Container->TopLeft != NULL)
		for (Ogre::uint i = 0; i < Container->TopLeft->ContainedEntities.size(); ++i)
			if(Container->TopLeft->ContainedEntities[i]->linearVelocityHistory.size())
				AverageVelocity += Container->TopLeft->ContainedEntities[i]->linearVelocityHistory.back() * (1 - 0.002 * (Container->TopLeft->ContainedEntities[i]->position - position).squaredLength());
	if( Container->TopRight != NULL)
		for (Ogre::uint i = 0; i < Container->TopRight->ContainedEntities.size(); ++i)
			if(Container->TopRight->ContainedEntities[i]->linearVelocityHistory.size())
				AverageVelocity += Container->TopRight->ContainedEntities[i]->linearVelocityHistory.back() * (1 - 0.002 * (Container->TopRight->ContainedEntities[i]->position - position).squaredLength());
	

	if(AttractionCenterWeightstSum != 0.0f) 
		AverageVelocity /= AttractionCenterWeightstSum;

	// Fishy Component : 
	Ogre::Vector3 PhishyComponent = Ogre::Vector3::ZERO;
	if(Hydrax_man.sea_level > terrain_height(position.x, position.z,0))
		PhishyComponent.y = 1.0f * pow((Hydrax_man.sea_level + terrain_height(position.x, position.z,0) - 2 * position.y)/(Hydrax_man.sea_level - terrain_height(position.x, position.z,0)),3);
	else
		PhishyComponent.y = -1000;

	if (Faune_man.debugInfoTimestamp + 500 < sys0.timer.getMilliseconds())
	{
	/*
	ldbg.send("Fish update info : \n Position  = " + toOgStr(position) 
			+ "\n PagePos = " + toOgStr(Container->MinXBorder) + "   " + toOgStr(Container->MinZBorder)
			+ "\n Playerpos = " + toOgStr(local_player.pos)
			+ "\n AverageVelocity = " + toOgStr(AverageVelocity*100)
			+ "\n (AttractionComponent / Timelapse) =  " + toOgStr((AttractionComponent / Timelapse)/10000) 
			+ "\n (RepultionComponent / Timelapse) =  " + toOgStr((RepultionComponent / Timelapse))
			+ "\n (PhishyComponent/Timelapse) =  " + toOgStr((PhishyComponent/Timelapse)*6));
		Faune_man.debugInfoTimestamp = sys0.timer.getMilliseconds();*/
	}

	// Compute displacement vector
	linearVelocity = AverageVelocity*100 + (AttractionComponent / Timelapse)/10000 + (RepultionComponent/Timelapse) + (PhishyComponent/Timelapse)*30 ;
	linearVelocity.normalise();
	//linearVelocity *= 2;

	// final displacement vector is the averaging of last quarter of displacement history	
	std::list<Ogre::Vector3>::iterator it = linearVelocityHistory.begin();
	Ogre::uint i = 1;
	while (i < linearVelocityHistory.size()/40)
	{
		linearVelocity += *it/((i+4)/2);
		++it;
		++i;
	}
	linearVelocity.normalise();
		



	Ogre::Vector3 NewPos = position + linearVelocity * Timelapse;

	
	AnimalNode->lookAt(NewPos, Ogre::Node::TransformSpace::TS_WORLD, Ogre::Vector3::UNIT_Z); // Makes the animal look at where he is heading 

	// Makes the animal's body vertical
	/*Ogre::Vector3 transform =  Ogre::Vector3(  AnimalNode->getOrientation().getPitch().valueAngleUnits(),
											   AnimalNode->getOrientation().getYaw().valueAngleUnits(),
											   0 );
	AnimalNode->setOrientation(Ogre::Quaternion(&transform));*/

	// Apply displacement
	AnimalNode->setPosition(	NewPos.x,
								NewPos.y,
								NewPos.z);

	position = AnimalNode->getPosition();

	//Store new velocity in history
	linearVelocityHistory.push_front(linearVelocity);
	if(linearVelocityHistory.size() > 400) //40 * Timelapse de temps de réponse donc (de l'ordre de la seconde) TODO : implement real reponse time, and random variation of it
		linearVelocityHistory.pop_back();

}


Amnesic_Faune_Entity::Amnesic_Faune_Entity(void)
{	
}
Amnesic_Faune_Entity::~Amnesic_Faune_Entity(void)
{
}

void Amnesic_Faune_Entity::initialise(Ogre::Vector2 RequestedPos)
{  
	AnimalEntity = sys0.mgr->createEntity("AmnseicAnimal" + StringConverter::toString(sys0.timer.getMicroseconds()),"Mamoth std.mesh");
	//AnimalEntity->setMaterial(MaterialManager::getSingleton().getByName("fur"));
	AnimalNode = sys0.mgr->getRootSceneNode()->createChildSceneNode();
	//AnimalNode->setScale(0.4,0.4,0.4);
	AnimalNode->attachObject(AnimalEntity);
	AnimalNode->setPosition(	RequestedPos.x,
								terrain_height(RequestedPos.x, RequestedPos.y,0),
								RequestedPos.y);

}

void Amnesic_Faune_Entity::update(float Timelapse = 1/60)
{
	position = AnimalNode->getPosition();
}

void Amnesic_Faune_Entity::destroy(void)
{
	AnimalNode->detachAllObjects();
	sys0.mgr->destroyEntity(AnimalEntity);
	sys0.mgr->destroySceneNode(AnimalNode);
} 




























































































































