#include "Namuh_Main_H.h"

using namespace Ogre;

PhysX_Trampoline_mgr PhysX_Trampoline_man;


PhysX_Trampoline_mgr::PhysX_Trampoline_mgr(void)
{
};
PhysX_Trampoline_mgr::~PhysX_Trampoline_mgr(void){};

void PhysX_Trampoline_mgr::add(NxOgre::Cloth* trampo)
{
	list.push_back(trampo);
};

void PhysX_Trampoline_mgr::remove(NxOgre::Cloth* trampo)
{
	for (int i =0;i<list.size();i++)
		if (list[i] == trampo)
			list.erase(list.begin()+i);
};

void PhysX_Trampoline_mgr::flush(void)
{
	for (int i =0;i<list.size();i++)
		list.clear();
}

void PhysX_Trampoline_mgr::update(float deltatime)
{
	for (	std::vector<NxOgre::Cloth*>::iterator trampoItt = list.begin();
			trampoItt != list.end();
			trampoItt++)
	{
		//if(someting collide)
			//if player then set linear velocity = intensity * normal
			//if rigibody the apply force at pos

	}
		
};











































































