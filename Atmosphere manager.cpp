#include "Namuh_Main_H.h"

using namespace Ogre;
Atm_mgr Atm_man;


// Code des méthodes de la classe
Atm_mgr::Atm_mgr(void)
{	
	SkyX::ColorGradient mWaterGradient= SkyX::ColorGradient(),mAmbientGradient= SkyX::ColorGradient(),mSunGradient = SkyX::ColorGradient();
	first_ini = true;
	initialised = false;

}
Atm_mgr::~Atm_mgr(void)
{
}

void Atm_mgr::initialise(void)
{  
	Vector3 lightDir= Ogre::Vector3::ZERO,lightAmb= Ogre::Vector3::ZERO;
	requested_hour = FloatFromName(Map.MapDataPool,"Time_Starting");
	Eastdir = FloatFromName(Map.MapDataPool,"World_East_Direction");
	mWaterGradient= SkyX::ColorGradient();
	mAmbientGradient= SkyX::ColorGradient();
	mSunGradient = SkyX::ColorGradient();
	Sunsethour = FloatFromName(Map.MapDataPool,"Time_Sunset");
	Sunrisehour = FloatFromName(Map.MapDataPool,"Time_Sunrise");
	 
	// Water color
	mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.168209f,0.635822f,0.779105f)*0.8f, 1.0f));
	mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.168209f,0.635822f,0.729105f)*0.6f, 0.8f));
	mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.168209f,0.635822f,0.679105f)*0.5f, 0.6f));
	mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.168209f,0.635822f,0.679105f)*0.4f, 0.5f));
	mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.168209f,0.635822f,0.679105f)*0.1f, 0.45f));
	mWaterGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.168209f,0.635822f,0.679105f)*0.025f, 0));
	// Ambient color
	mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1.0f,1.0f,1.0f)*1.0f, 1.0f));
	mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1.0f,1.0f,1.0f)*1.0f, 0.6f));
	mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1.0f,1.0f,1.0f)*0.6f, 0.5f));
	mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1.0f,1.0f,1.0f)*0.3f, 0.45f));
	mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1.0f,1.0f,1.0f)*0.1f, 0.35f));
	mAmbientGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(1.0f,1.0f,1.0f)*0.05f, 0.0f));
	// Sun
	mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8f,0.75f,0.55f)*1.5f, 1.0f));
	mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8f,0.75f,0.55f)*1.4f, 0.75f));
	mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.8f,0.75f,0.55f)*1.3f, 0.5625f));
	mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.6f,0.5f,0.2f)*1.5f, 0.5f));
	mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.5f,0.5f,0.5f)*0.25f, 0.45f));
	mSunGradient.addCFrame(SkyX::ColorGradient::ColorFrame(Ogre::Vector3(0.5f,0.5f,0.5f)*0.01f, 0.0f));

	if(first_ini)
	{
		mControler = new SkyX::BasicController();
		mSkyX = new SkyX::SkyX(sys0.mgr, &mControler);
		mSkyX->create();
		mControler.setMoonPhase(0.75f);
		sys0.mRoot->addFrameListener(mSkyX);
		sys0.win->addListener(mSkyX);

		mSkyX->getCloudsManager()->add(SkyX::CloudLayer::Options());
	}
	mSkyX->getVCloudsManager()->setHeight(Vector2(FloatFromName(Map.MapDataPool,"Weather_Cloud_Altitude"),FloatFromName(Map.MapDataPool,"Weather_Cloud_Height")));
	
	if(BoolFromName(Map.MapDataPool,"Weather_Cloud_Enabled"))
	{
		if(first_ini)
			mSkyX->getVCloudsManager()->create(20000); // Le radius de SkyX influe sur l'altitude que l'on peut donner à la couche nuageuse
		mSkyX->getVCloudsManager()->setAutoupdate(true);
		mSkyX->getVCloudsManager()->getVClouds()->setWheater(FloatFromName(Map.MapDataPool,"Weather_Cloud_Humidity"),FloatFromName(Map.MapDataPool,"Weather_Cloud_Average_Width"),true); // average : 0.15,60,2
		mSkyX->getVCloudsManager()->getVClouds()->setWindDirection(Ogre::Radian(FloatFromName(Map.MapDataPool,"Weather_Wind_Direction")));
		mSkyX->getVCloudsManager()->getVClouds()->setWindSpeed(FloatFromName(Map.MapDataPool,"Weather_Wind_Speed"));
	}
	if(!first_ini && !BoolFromName(Map.MapDataPool,"Weather_Cloud_Enabled") && mSkyX->getVCloudsManager() != NULL) // Si il faut enlever les nuwage
	{
		mSkyX->getVCloudsManager()->remove();
		delete mSkyX->getVCloudsManager();
	}
	Options.Exposure = FloatFromName(Map.MapDataPool,"Weather_Sky_Exposure");
	Options.MieMultiplier = FloatFromName(Map.MapDataPool,"Weather_Sky_MieMultiplier");
	mControler.setTime(Vector3(FloatFromName(Map.MapDataPool,"Time_Starting"),Sunrisehour,Sunsethour));
	Options.WaveLength = Vector3FromName(Map.MapDataPool,"Weather_Sky_WaveLength");
	mControler.setEastDirection(Vector2(cos(Eastdir),sin(Eastdir)));
	mSkyX->getAtmosphereManager()->setOptions(Options);

	mSkyX->getVCloudsManager()->getVClouds()->setDistanceFallingParams(Ogre::Vector2(2,955));
	mSkyX->setTimeMultiplier(FloatFromName(Map.MapDataPool,"Time_Multiplier"));

	
	initialised = true;
	first_ini = false;
}






void Atm_mgr::update(float ellapsed_time)
{
	Options = mSkyX->getAtmosphereManager()->getOptions();
	sys1.hour = static_cast<char>(mControler.getTime().x);
	sys1.min  = static_cast<char>((mControler.getTime().x - sys1.hour)*60);
	Ogre::Light *Light0 = sys0.mgr->getLight("SunLight"),*Light1 = sys0.mgr->getLight("SunLight1");
	lightDir =	mControler.getSunDirection();
	Light0->setPosition(sys0.cam->getDerivedPosition() - lightDir*mSkyX->getMeshManager()->getSkydomeRadius(sys0.cam)*0.02f);
	Light1->setDirection(lightDir);
	Light0->setDirection(lightDir);
	Map.terrainGlobals->setLightMapDirection(Light1->getDerivedDirection());
	Map.terrainGlobals->setCompositeMapDiffuse(Light1->getDiffuseColour());
	sunCol = mSunGradient.getColor(((lightDir.y +1 ) / 2.0f)); // Time_gradient : ((lightDir.y + 1.0f) / 2.0f)
	sunPos = sys0.cam->getDerivedPosition() - lightDir*mSkyX->getMeshManager()->getSkydomeRadius(sys0.cam)*0.1f;
	lightAmb = mAmbientGradient.getColor(((lightDir.y + 1) / 2.0f));
	Light0->setDiffuseColour(ColourValue(sunCol.x,sunCol.y,sunCol.z));
	Light1->setDiffuseColour(ColourValue(sunCol.x,sunCol.y,sunCol.z));
	sys0.mgr->setAmbientLight(ColourValue(lightAmb.x,lightAmb.y,lightAmb.z));
	//if(lastrequested_hour != requested_hour) mControler.setTime(Vector3(requested_hour,Sunrisehour,Sunsethour));
//	lastrequested_hour = requested_hour;
	mControler.setEastDirection(Vector2(cos(Eastdir),sin(Eastdir)));
	mSkyX->getAtmosphereManager()->setOptions(Options);
	mControler.update(ellapsed_time*mSkyX->getTimeMultiplier());
	mSkyX->update(ellapsed_time); 
}


void Atm_mgr::destroy(void)
{
//	delete mSkyX;
}















































































































