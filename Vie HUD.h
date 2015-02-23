/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_VIE_HUD // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_VIE_HUD

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class Vie_Hud // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
	public:

	HardwarePixelBufferSharedPtr PixelBuffer;
	TexturePtr Texture;
	Ogre::OverlayContainer* panel;
	Ogre::Overlay* overlay;

	int vie;
	bool initialised;
	
    Vie_Hud(void);
    ~Vie_Hud(void);
    void initialise(void);
	void set(int);
};
	extern Vie_Hud Affichage_vie;
#endif // CLASS_PERSO


