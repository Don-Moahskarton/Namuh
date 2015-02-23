#include "Namuh_Main_H.h"

using namespace Ogre;

//extern Hydrax::Hydrax *mHydrax;

PhysX_Raft PhysXfloater;


PhysX_Raft::PhysX_Raft(void)
{
	int last_mesh = 0,waterheight = -3.5;
	Critter::Body* list[5000] = {NULL};
};
PhysX_Raft::~PhysX_Raft(void){};

void PhysX_Raft::add(Critter::Body* new_raft)
{
	list[last_mesh] = new_raft;
	last_mesh++;
};

void PhysX_Raft::remove(Critter::Body* raft)
{
	for(int i =0;i<last_mesh;i++)
		if(list[i] == raft)
			list[i] = NULL;
};

void PhysX_Raft::flush(void)
{
	last_mesh = 0;waterheight = -6.5;
	for(int i =0;i<5000;i++)
	{
		if(list[i] != NULL)
		{
			list[i] = NULL;
		}
	}
};

void PhysX_Raft::update(float deltatime)
{
	if(Hydrax_man.initialised)
	{
		if(list[0] != NULL) // TODO : Get an efficient way to access this data at 10fps for each floating object
			waterheight = Hydrax_man.mHydrax->getHeigth(Vector2(list[0]->getGlobalPosition().x,list[0]->getGlobalPosition().z)); //update this only when required

		for(int i =0;i<last_mesh;i++)
			if(list[i] != NULL)
			{
				if(list[i]->getNode()->getPosition().y<waterheight)
				{
				// Frottements
				list[i]->setLinearVelocity(Vector3((0.97)*list[i]->getLinearVelocity().x *Ogre::Math::Exp(-0.02/(Ogre::Math::Abs(list[i]->getLinearVelocity().x)+1) ),
												   (0.97)*list[i]->getLinearVelocity().y *Ogre::Math::Exp(-0.02/(Ogre::Math::Abs( list[i]->getLinearVelocity().y)+1) ),
												   (0.97)*list[i]->getLinearVelocity().z *Ogre::Math::Exp(-0.02/(Ogre::Math::Abs( list[i]->getLinearVelocity().z)+1) ) ));

				list[i]->setAngularVelocity(Vector3((0.97)*list[i]->getAngularVelocity().x *Ogre::Math::Exp(-0.02/(Ogre::Math::Abs(list[i]->getAngularVelocity().x)+1) ),
													(0.97)*list[i]->getAngularVelocity().y *Ogre::Math::Exp(-0.02/(Ogre::Math::Abs( list[i]->getAngularVelocity().y)+1) ),
													(0.97)*list[i]->getAngularVelocity().z *Ogre::Math::Exp(-0.02/(Ogre::Math::Abs( list[i]->getAngularVelocity().z)+1) ) ));
			

				// Flotabilité des cubes (valeur multipilé ensuite par l'écart entre la surface et l'objet)
					float y =   (9.8f * list[i]->getMass());						  // counteract gravity
	//					 + (-list[i]->getLinearVelocity().y * list[i]->getMass() * list[i]->getMass())    // slowly counteract vertical velocity
	//					 + ((waterheight-list[i]->getNode()->getPosition().y)/40 * list[i]->getMass()); // add some force to move it to the top

					list[i]->addForce(NxOgre::Vec3(0,y*2*(waterheight-list[i]->getNode()->getPosition().y),0), NxOgre::Enums::ForceMode_Force); 
					list[i]->addForce(NxOgre::Vec3(Ogre::Math::RangeRandom(-100.1f,100.1f)/1000.0f* list[i]->getMass()*9.8, 0, Ogre::Math::RangeRandom(-100.1f,100.1f)/1000.0f* list[i]->getMass())*9.8f,NxOgre::Enums::ForceMode_Force); 
				
					// cap ascending speed to 1m/s
					if(list[i]->getLinearVelocity().y > 3.0f)
						list[i]->setLinearVelocity(list[i]->getLinearVelocity().x, 3.0f, list[i]->getLinearVelocity().z);
				}
			}
		}
};











































































