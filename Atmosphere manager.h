/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_ATMOSPHERE_MANAGER// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_ATMOSPHERE_MANAGER

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class Atm_mgr // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{

	
	public:
	SkyX::SkyX* mSkyX;
	SkyX::ColorGradient mWaterGradient,mAmbientGradient,mSunGradient;
	SkyX::AtmosphereManager::Options Options;
	SkyX::BasicController mControler;

	bool	initialised,
			first_ini;

	Vector3 lightDir,
			lightAmb,
			sunCol,
			sunPos;
	float requested_hour,lastrequested_hour,
		Eastdir,
		Sunsethour,
		Sunrisehour;
	
    Atm_mgr(void);
    ~Atm_mgr(void);
    void initialise(void);
	void update(float);
    void destroy(void);
};

// Déclaration du joueur local
extern Atm_mgr Atm_man;

#endif // CLASS_PERSO


