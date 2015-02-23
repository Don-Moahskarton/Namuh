#include "Namuh_Main_H.h"

using namespace Ogre;
Amnesic_Faune_mgr Amnesic_Faune_man;

// Code des méthodes de la classe
Amnesic_Faune_mgr::Amnesic_Faune_mgr(void)
{
	initialised = false;
}
Amnesic_Faune_mgr::~Amnesic_Faune_mgr(void)
{
}

void Amnesic_Faune_mgr::initialise(void)
{  
	if(!initialised)
	{
		initialised = true;

		ActualEntityCount = 0;
		DisplayFakeDistance = 50; //temp
		MaxEntities = 10; //temp
		std::vector<Amnesic_Faune_Entity> Theonlyone;
		Amnesic_Entities.push_back(Theonlyone);
	}

}

void Amnesic_Faune_mgr::destroy(void)
{

}


void Amnesic_Faune_mgr::update(float Timelapse, Ogre::Vector2 playerpos)
{
	// Creating life in the universe
	/*
	Ogre::uint tryCount = ( MaxEntities - ActualEntityCount ) * 2; // two attempts to place each animal in average
	while(ActualEntityCount < MaxEntities && tryCount > 0)
	{
		GenerateNewAnimals(playerpos);
		tryCount--; 
	}
	*/
	Ogre::Real rnd = Ogre::Math::RangeRandom(0.0f,100.0f);
	if(ActualEntityCount < MaxEntities && rnd > 20.0f) // 80% of chance to generate a missing animal
		GenerateNewAnimals(playerpos);


	for(uint i = 0; i < Amnesic_Entities.size(); i++)
		for(uint j = 0; j < Amnesic_Entities[i].size(); j++)
		{

			// update all animals
			Amnesic_Entities[i][j].update(Timelapse);
			
			// delete too far ones
			if(FakeDistance2D(	Amnesic_Entities[i][j].position.x, 
								Amnesic_Entities[i][j].position.z,
								playerpos.x,
								playerpos.y) > DisplayFakeDistance)
			{
				Amnesic_Entities[i][j].destroy();				
				Amnesic_Entities[i].erase(Amnesic_Entities[i].begin()+j);
				if(ActualEntityCount != 0)ActualEntityCount--;
			}
		}
}


void Amnesic_Faune_mgr::GenerateNewAnimals(Ogre::Vector2 playerpos)
{
	float	PoppingAngle1			= Ogre::Math::RangeRandom(-1,1),
			PoppingAngle2			= Ogre::Math::RangeRandom(-1,1);

	//TO_IMPROVE
	Ogre::uint tryCount = 30;
	Ogre::Vector2 NewPos(	playerpos.x + DisplayFakeDistance *0.8 *(PoppingAngle1),
							playerpos.y + DisplayFakeDistance *0.8 *(PoppingAngle2)); 
	while (Hydrax_man.sea_level > terrain_height(NewPos.x, NewPos.y,0) && tryCount)			// finds a suitable place to pop
	{
		Ogre::Vector2 NewPos(	playerpos.x + DisplayFakeDistance *0.8 *(PoppingAngle1),
								playerpos.y + DisplayFakeDistance *0.8 *(PoppingAngle2)); 
		tryCount--;
	}

	Amnesic_Faune_Entity NewAmnesicAnimal;
	Amnesic_Entities[0].push_back(NewAmnesicAnimal);
	Amnesic_Entities[0].back().initialise(NewPos);
	ActualEntityCount++;
}

uint FakeDistance2D (int x1, int y1, int x2, int y2) //{ return uint( std::sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) )); } // true distance for debugging purpose
	{ return ( (abs(x1-x2) + abs(y1-y2))*0.785 ); }










 









































































































