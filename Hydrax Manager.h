/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_HYDRAX_MANAGER// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_HYDRAX_MANAGER

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class Hydrax_mgr // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{

	
	public:
	Hydrax::Hydrax* mHydrax;
	Hydrax::Module::ProjectedGrid *mModule;
	float sea_level;
	bool	first_ini,
			initialised;
	

    Hydrax_mgr(void);
    ~Hydrax_mgr(void);
    void initialise(void);
	void update(float);
	void destroy(void);
};

// Déclaration du joueur local
extern Hydrax_mgr Hydrax_man;



// Hydrax Rtt Listener class
class HydraxRttListener : public Hydrax::RttManager::RttListener
{public:
	void preRenderTargetUpdate(const Hydrax::RttManager::RttType& Rtt)
	{	// If needed in any case...
		bool underwater = Hydrax_man.mHydrax->_isCurrentFrameUnderwater();
		switch (Rtt)
		{	case Hydrax::RttManager::RTT_REFLECTION:
			{	// No stars in the reflection map
				Atm_man.mSkyX->setStarfieldEnabled(false);
			}break;
			case Hydrax::RttManager::RTT_REFRACTION:{}break;
			case Hydrax::RttManager::RTT_DEPTH: case Hydrax::RttManager::RTT_DEPTH_REFLECTION:
			{	// Hide SkyX components in depth maps
				Atm_man.mSkyX->getMeshManager()->getEntity()->setVisible(false);
				Atm_man.mSkyX->getMoonManager()->getMoonBillboard()->setVisible(false);
			}break;
		}
	}
	void postRenderTargetUpdate(const Hydrax::RttManager::RttType& Rtt)
	{	bool underwater = Hydrax_man.mHydrax->_isCurrentFrameUnderwater();
		switch (Rtt)
		{	case Hydrax::RttManager::RTT_REFLECTION:
			{Atm_man.mSkyX->setStarfieldEnabled(true);
			}break;
			case Hydrax::RttManager::RTT_REFRACTION:{}break;
			case Hydrax::RttManager::RTT_DEPTH: case Hydrax::RttManager::RTT_DEPTH_REFLECTION:
			{	Atm_man.mSkyX->getMeshManager()->getEntity()->setVisible(true);
				Atm_man.mSkyX->getMoonManager()->getMoonBillboard()->setVisible(true);
			}break;
		}
	}
};

#endif // CLASS_PERSO


