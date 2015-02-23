#include "Namuh_Main_H.h"

using namespace Ogre;

PhysX_Wings_mgr PhysX_Wings_man;


PhysX_Wings_mgr::PhysX_Wings_mgr(void)
{
	std::vector<pair<NxOgre::Cloth*,pair<float,std::vector<Critter::Body*>>>> emptylist;
	list = emptylist;
};
PhysX_Wings_mgr::~PhysX_Wings_mgr(void){};

void PhysX_Wings_mgr::add(NxOgre::Cloth* wing, float surface, std::vector<Critter::Body*>* attachedBodies)
{
	pair<float, std::vector<Critter::Body*>> temp = pair<float, std::vector<Critter::Body*>>(surface, *attachedBodies);
	pair<NxOgre::Cloth*, pair<float, std::vector<Critter::Body*> > > elem(wing, temp);
	list.push_back(elem);

	ldbg.send("TODO : Solve the problem arround wing force computation");
};

void PhysX_Wings_mgr::remove(NxOgre::Cloth* wing)
{
	for (int i =0;i<list.size();i++)
		if (list[i].first == wing)
			list.erase(list.begin()+i);
};

void PhysX_Wings_mgr::flush(void)
{
	for (int i =0;i<list.size();i++)
		list.clear();
}

void PhysX_Wings_mgr::update(float deltatime)
{
	// Add portance component
	Vec3 releventSpeed;
	float portance;
	for (	std::vector<pair<NxOgre::Cloth*, pair<float, std::vector<Critter::Body*>>>>::iterator wingItt = list.begin();
			wingItt != list.end();
			wingItt++)
	{
		if((*wingItt).first->getVelocity(3).y > 0 )
			releventSpeed = Vec3((*wingItt).first->getVelocity(3).x, 0, (*wingItt).first->getVelocity(3).z);
		else
			releventSpeed = (*wingItt).first->getVelocity(3);


		// hack is hack
		if (releventSpeed.magnitudeSquared() > 1000000)
			releventSpeed /= 1000;
		

		//compute portance : K * ( V^2 ) * S
		// TODO : recompute wing surface based on junction point positions.
		// (enabling deltaplanes to be thrown form catapult and be opened in mid-air)
		portance = 250 * float(sqrt(releventSpeed.magnitude())) * (*wingItt).second.first;


		(*wingItt).first->setExternalAcceleration(Vec3(0,portance/100,0)); // distords the hide (only graphic effect)
	
		for (	std::vector<Critter::Body*>::iterator attachedItt = (*wingItt).second.second.begin();
				attachedItt != (*wingItt).second.second.end();
				attachedItt++)
			{
				Vec3 force = NxOgre::Vec3(0,portance,0) * (*attachedItt)->getGlobalOrientationQuat();
				force.normalise();
				(*attachedItt)->addLocalForce( portance * force, NxOgre::Enums::ForceMode_Force); 
			}
		
	}

	// Let's say we have a local basis attavched to the wing, calculating the angular velocity is just a math problem. (3x3 system to solve)
	// Thus Damping ? Well, only if we can retrieve the relevant attached body and apply a force on it to counteract momentum

	/*
	Principe de la solution 

	Soit x0, y0 et z0 la base a l'instant t
	Soit x1, y1 et z1 la base a l'instant t + dt
	Les vitesses angulaires pourraient être retrouvées en prenant l'angle entre la projection de x1 sur le plan y0z0 et x0 pour Wx. Idem pour Wy et Wz.

	Idem, on pourrait faire un amortissement en mettant une force sur les 4 corps liés en sens contraire de la rotation.
	Voire même en les faisant proportionnelles a la distance entre leurs centre de masse ( point d'application de la force)
	et l'axe de rotation, ce qui permanenterai d'obtenir un vrai couple de frottement.

	Awi, et ne pas oublier que la base se trouve en prenant la régression planaire des 4 coins de la peau de bète.
	Keyword  :orthogonal distance regression plane
	On a alors un vecteur normal complètement défini.
	Ensuite, pour la rotation d'axe normal au plan, on prend de sorte que x ou z soit parallèle a un des cotés de la peau.
	*/
	

	/*
		
	// add angular daming on X and Z rotation axis
	float damping;
	for (	std::vector<pair<NxOgre::Cloth*, pair<float, std::vector<Critter::Body*>>>>::iterator wingItt = list.begin();
			wingItt != list.end();
			wingItt++)
	{
	
		for (	std::vector<Critter::Body*>::iterator attachedItt = (*wingItt).second.second.begin();
				attachedItt != (*wingItt).second.second.end();
				attachedItt++)
			{
				
				(*wingItt).second.second[0]->addTorque(NxOgre::Vec3(0,portance,0), NxOgre::Enums::ForceMode_Force); 
			}
	}
	*/


};











































































