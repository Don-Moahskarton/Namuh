#include "Namuh_Main_H.h"

using namespace Ogre;


// Code des méthodes de la classe
Vie_Hud::Vie_Hud(void)
{
	initialised = false;
}
Vie_Hud::~Vie_Hud(void)
{
}

void Vie_Hud::initialise()
{
	initialised = true;
	vie = 100;


	overlay =  Ogre::OverlayManager::getSingleton().create( "VieOverlay" );
	overlay->show();
	
	// Create the graph's texture and store a pointer of it
	Texture = TextureManager::getSingleton().createManual(
		"Affichage vie "+ StringConverter::toString(sys0.timer.getMicroseconds()), // name
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D,      // type
		256,256,     // width & height
		0,                // number of mipmaps
		PF_BYTE_RGBA,     // pixel format
		TU_DYNAMIC_WRITE_ONLY);
	PixelBuffer = Texture->getBuffer();
	 
	// Generate the image
	PixelBuffer->lock(HardwareBuffer::HBL_DISCARD);
	const PixelBox& PixelBox = PixelBuffer->getCurrentLock();

	Ogre::uint32 *data = static_cast<Ogre::uint32*>(PixelBox.data);
	size_t height = PixelBox.getHeight();
	size_t width = PixelBox.getWidth();
	size_t pitch = PixelBox.rowPitch; // Skip between rows of image

	for(size_t y=0; y<height; ++y)
		for(size_t x=0; x<width; ++x)
			data[pitch*y + x] =  static_cast<Ogre::uint32>(Ogre::ColourValue::ZERO.getAsARGB());

		
	for(int i=0;i<360;i++)
	{
		Ogre::uint	x = 128+(50+7*(float(i)*0.01*vie/360))*Ogre::Math::Cos(Ogre::Degree(i)),
					y = 128+(50+7*(float(i)*0.01*vie/360))*Ogre::Math::Sin(Ogre::Degree(i));
		data[pitch*y + x] = Ogre::ColourValue::Red.getAsARGB();
	}

	// Réticule
	for(size_t y=127; y<129; ++y)
		for(size_t x=127; x<129; ++x)
			data[pitch*y + x] =  Ogre::ColourValue::Red.getAsARGB();

	PixelBuffer->unlock();

	// Create a material using the texture
	MaterialPtr material = MaterialManager::getSingleton().create(Texture->getName(),ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	material->getTechnique(0)->getPass(0)->createTextureUnitState(Texture->getName());
	material->getTechnique(0)->getPass(0)->setSceneBlending(SBT_TRANSPARENT_ALPHA);

	// Create a panel
	panel = static_cast<Ogre::OverlayContainer*>(OverlayManager::getSingleton().createOverlayElement("Panel",Texture->getName()));
	panel->setMetricsMode(Ogre::GMM_PIXELS);
	panel->setPosition(sys0.vp->getActualWidth()/2-128,sys0.vp->getActualHeight()/2-128);
	panel->setDimensions(256,256);
	panel->setMaterialName(material->getName());
	overlay->add2D(panel);
}

void Vie_Hud::set(int requested_vie)
{
	vie = requested_vie;
	if(initialised)
	{
		// Regenerate the image
		PixelBuffer->lock(HardwareBuffer::HBL_DISCARD);
		const PixelBox& PixelBox = PixelBuffer->getCurrentLock();

		Ogre::uint32 *data = static_cast<Ogre::uint32*>(PixelBox.data);
		size_t height = PixelBox.getHeight();
		size_t width = PixelBox.getWidth();
		size_t pitch = PixelBox.rowPitch; // Skip between rows of image

		for(size_t y=0; y<height; ++y)
			for(size_t x=0; x<width; ++x)
				data[pitch*y + x] =  static_cast<Ogre::uint32>(Ogre::ColourValue::ZERO.getAsARGB());

			
		for(int i=0;i<360;i++)
		{
			Ogre::uint	x = 128+(50+7*(float(i)*0.01*vie/360))*Ogre::Math::Cos(Ogre::Degree(i)),
						y = 128+(50+7*(float(i)*0.01*vie/360))*Ogre::Math::Sin(Ogre::Degree(i));
			data[pitch*y + x] = Ogre::ColourValue::Red.getAsARGB();

		}

		// Réticule
		for(size_t y=127; y<129; ++y)
			for(size_t x=127; x<129; ++x)
				data[pitch*y + x] =  Ogre::ColourValue::Red.getAsARGB();

		PixelBuffer->unlock();
	}



}























































































































