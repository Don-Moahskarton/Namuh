#include "Namuh_Main_H.h"

using namespace Ogre;
Hydrax_mgr Hydrax_man;


// Code des méthodes de la classe
Hydrax_mgr::Hydrax_mgr(void)
{
	first_ini = true;
	initialised = false;
}
Hydrax_mgr::~Hydrax_mgr(void)
{
	delete mHydrax; 
}

void Hydrax_mgr::initialise(void)
{
	if (!initialised)
	{
		sea_level= FloatFromName(Map.MapDataPool,"World_Water_Height");
		if(first_ini)mHydrax = new Hydrax::Hydrax(sys0.mgr,sys0.cam,sys0.vp);
		mModule = new Hydrax::Module::ProjectedGrid(mHydrax,new Hydrax::Noise::Perlin(),Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),Hydrax::MaterialManager::NM_VERTEX,Hydrax::Module::ProjectedGrid::Options());
		mHydrax->setModule(static_cast<Hydrax::Module::Module*>(mModule));
		mHydrax->loadCfg("Namuh.hdx");
		mHydrax->create();
		mHydrax->setUnderwaterCameraSwitchDelta(0.01f);

		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("base_material"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("lambert2SGaa"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("material16"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Falaise_long"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("grass"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Falaise_vert"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("particles_generic"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("FresnelReflectionRefraction"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Fish01"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Falaise_Long"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Falaise_long"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Rocher_6968k"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("Rocher_6968kbis"))->createTechnique());
		mHydrax->getMaterialManager()->addDepthTechnique(static_cast<Ogre::MaterialPtr>(Map.terrain->getMaterial())->createTechnique());


		mHydrax->setPosition(Vector3(sys0.cam->getPosition().x,//ici point de parent node
									 -2,
									sys0.cam->getPosition().z));
	
		initialised = true;
		first_ini = false; // a garder a la fin
	}
	else
		ldbg.send("Hydrax_man : tried to initailise hydrax, which is already initialised : nothing to do");
}

void Hydrax_mgr::update(float ellapsed_time)
{/*
	mHydrax->setWaterColor(Atm_man.mWaterGradient.getColor(((Atm_man.lightDir.y + 1.0f) / 2.0f)));
	mHydrax->setSunPosition(Atm_man.sunPos);
	mHydrax->setSunColor(Atm_man.sunCol);*/
	mHydrax->setPosition(Vector3(sys0.cam->getParentNode()->convertLocalToWorldPosition(sys0.cam->getPosition()).x,
								sea_level,//+2*sin(-3.14*Atm_man.mControler.getTime().x/12)
								sys0.cam->getParentNode()->convertLocalToWorldPosition(sys0.cam->getPosition()).z));
	mHydrax->update(ellapsed_time);
		
}

void Hydrax_mgr::destroy(void)
{
	if (initialised)
	{
		initialised = false;
		mHydrax->remove();
		mModule->remove();
		//delete mModule;
	}
	else 
		ldbg.send("Hydrax_man : tried to destroy hydrax, which is not initialised : nothing to do");

}











































































