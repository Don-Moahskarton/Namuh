#include "Namuh_Main_H.h"

using namespace Ogre;
Ogre::String Debug_Text = "";


 Root *Ogre_ini::mRoot;
 OIS::Keyboard *Ogre_ini::mKeyboard;
 OIS::Mouse *Ogre_ini::mMouse;
 OIS::InputManager *Ogre_ini::mInputManager;
 ExitListener *Ogre_ini::mListener;
 Ogre::String Ogre_ini::secName, Ogre_ini::typeName, Ogre_ini::archName;
 ConfigFile Ogre_ini::cf;
 Ogre::RenderSystem *Ogre_ini::rs;
 SceneManager *Ogre_ini::mgr;
 Camera* Ogre_ini::cam;
 Viewport* Ogre_ini::vp;
 size_t Ogre_ini::windowHnd;
 std::ostringstream Ogre_ini::windowHndStr;
 OIS::ParamList Ogre_ini::pl;
 RenderWindow *Ogre_ini::win;
 const OIS::MouseState *Ogre_ini::ms;
 ShadowCameraSetupPtr Ogre_ini::mPSSMSetup;
 Ogre::Timer Ogre_ini::timer;
 Ogre::RenderTarget *Ogre_ini::RefractionRenderTarget;
 bool Ogre_ini::last_mousebouttons_state[8];
 float Ogre_ini::ScreenSizeRatio;
 string Ogre_ini::windowName;
 std::vector<bool>  Ogre_ini::lastKeyboardState;

Ogre_ini::Ogre_ini(void)
{
	srand ( time(NULL) );
	#ifndef _DEBUG 
		 mRoot = new Root("plugins.cfg","display.cfg","log.txt");
	#endif
	#ifdef _DEBUG 
		 mRoot = new Root("plugins_d.cfg","display.cfg","log.txt");
	#endif
	LogManager::getSingleton().setLogDetail(LL_BOREME);
	cf.load("resources.cfg");
    ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
    if (!mRoot->restoreConfig() && !mRoot->showConfigDialog())
    //if (!mRoot->showConfigDialog()) // pour afficher a chaque fois la fenetre de configuration
    {
        //uniquement si probleme
        RenderSystem *rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
                                          // mais aussi "Direct3D9 Rendering Subsystem"
        mRoot->setRenderSystem(rs);
        rs->setConfigOption("Full Screen", "No");
        rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");
    }
	windowName =  "Namuh - " + BuildDate;

    mRoot->initialise(true,windowName); //true pour initialisation auprès de l'OS auto
	timer.reset();
    TextureManager::getSingleton().setDefaultNumMipmaps(5);
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	mgr = mRoot->createSceneManager("TerrainSceneManager");
	Map.terrain = OGRE_NEW Ogre::Terrain(sys0.mgr);
    cam = mgr->createCamera("Camera");
    vp = mRoot->getAutoCreatedWindow()->addViewport(cam);
	//vp->setBackgroundColour(ColourValue(0.265,0.422,0.703)); // bleu clair
	vp->setBackgroundColour(ColourValue(0,0,0)); // noir
	ScreenSizeRatio = vp->getActualWidth() / vp->getActualHeight();
    cam->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    cam->setNearClipDistance(0.3f);
	cam->setLodBias(3);
	//mRoot->getRenderTarget(windowName)->setAutoUpdated(false);

	//cam->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
	
	/*
	// What is that ?
	TexturePtr tex = TextureManager::getSingleton().createManual("refraction" ,ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, 1024, 1024, 0, PF_R8G8B8, TU_RENDERTARGET);
	RefractionRenderTarget = tex->getBuffer()->getRenderTarget();
	RefractionRenderTarget->addViewport(cam)->setOverlaysEnabled(false);
	RefractionRenderTarget->setAutoUpdated(false);
	RefractionRenderTarget->getViewport(0)->setCamera(cam);
	RefractionRenderTarget->getViewport(0)->setAutoUpdated(true);
	*/
	
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    OIS::ParamList pl;
    win = mRoot->getAutoCreatedWindow();
    win->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	/*
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	*/
    mInputManager = OIS::InputManager::createInputSystem(pl);
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
	ms = &mMouse->getMouseState();
	ms->width = vp->getActualWidth();
	ms->height = vp->getActualHeight();

	CompositorManager::getSingleton().initialise();
	Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();
	
	//compMgr.registerCompositorLogic("HDR", new HDRLogic);	
	//CompositorManager::getSingleton().addCompositor(vp,"HDR",-1);
	//CompositorManager::getSingleton().setCompositorEnabled(vp,"HDR", true);
	//CompositorManager::getSingleton().addCompositor(vp,"Bloom",-1);
	//CompositorManager::getSingleton().setCompositorEnabled(vp,"Bloom", true);
	//CompositorManager::getSingleton().addCompositor(vp,"Contrast",-1);
	//CompositorManager::getSingleton().setCompositorEnabled(vp,"Contrast", true); 
	//CompositorManager::getSingleton().addCompositor(vp,"ASCII",-1);
	//CompositorManager::getSingleton().setCompositorEnabled(vp,"ASCII", true);

	// Technique des ombres
	mgr->setShadowTechnique( SHADOWTYPE_NONE); 
	/*
	mgr->setShadowTechnique( SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED); 
	mgr->setShadowFarDistance(3200);
	mgr->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);
	PSSMShadowCameraSetup* pssmSetup = new PSSMShadowCameraSetup();
	pssmSetup->setSplitPadding(cam->getNearClipDistance());
	pssmSetup->calculateSplitPoints(3, cam->getNearClipDistance(),1200);
	pssmSetup->setOptimalAdjustFactor(0, 2);
	pssmSetup->setOptimalAdjustFactor(1, 1);
  	pssmSetup->setOptimalAdjustFactor(2, 0.5);
    mPSSMSetup.bind(pssmSetup);
    mgr->setShadowCameraSetup(mPSSMSetup);
	mgr->setShadowTextureCount(3);
	mgr->setShadowTextureConfig(0, 1024, 1024, PF_L8);
	mgr->setShadowTextureConfig(1, 256, 256, PF_L8);
	mgr->setShadowTextureConfig(2,  64,  64, PF_L8);
	mgr->setShadowTextureSelfShadow(true);
	mgr->setShadowCasterRenderBackFaces(false);*/
	//mgr->setShadowTextureCasterMaterial(StringUtil::BLANK);

	//Anisotropie
	MaterialManager::getSingleton().setDefaultTextureFiltering(TFO_ANISOTROPIC);
	MaterialManager::getSingleton().setDefaultAnisotropy(1);
	

	// SSAO 

	//PFXSSAO* mSSAO = new PFXSSAO(win, cam);



	// Initialise CEGUI
	ldbg.send("Initialising CEGUI...");
	Gui_manager::mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
	Gui_manager::sys = &CEGUI::System::getSingleton();
	Gui_manager::winMgr = &CEGUI::WindowManager::getSingleton();

	CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts"); 
	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::SchemeManager::getSingleton().create("GlossySerpent.scheme");
	Gui_manager::sys->setDefaultMouseCursor("TaharezLook", "MouseArrow");
	CEGUI::MouseCursor::getSingleton().setImage(Gui_manager::sys->getDefaultMouseCursor());
	ldbg.send("CEGUI sucessfully initialised");
	ldbg.send("Ogre sucessfully initialised");

	//Smart keyboard initalisation
	for(Ogre::uint i = 0; i< 250; ++i)
		lastKeyboardState.push_back(false);
	
	for(Ogre::uint i = 0; i< 250; ++i)
		lastKeyboardState[i] = false;

	this->update_SmartKeyboard();

}

void Ogre_ini::update_SmartMouse()
{
	for(int t=0;t<7;t++)	
		last_mousebouttons_state[t] = ms->buttonDown(OIS::MouseButtonID(t));
}

bool Ogre_ini::ms_pressing(char button_ID)
{
	return !last_mousebouttons_state[button_ID] &&  ms->buttonDown(OIS::MouseButtonID(button_ID));
}
bool Ogre_ini::ms_releasing(char button_ID)
{
	return last_mousebouttons_state[button_ID] &&  !ms->buttonDown(OIS::MouseButtonID(button_ID));
}

void Ogre_ini::update_SmartKeyboard()
{
	for(Ogre::uint i = 0; i< 250; ++i)	
		lastKeyboardState[i] = mKeyboard->isKeyDown( static_cast<OIS::KeyCode>(i) );
}

bool Ogre_ini::kb_pressing(OIS::KeyCode button_ID)
{
    sys0.mKeyboard->capture(); // Met a jour le clavier
	return !lastKeyboardState[static_cast<Ogre::uint>(button_ID)] &&  mKeyboard->isKeyDown( button_ID ) ;
}
bool Ogre_ini::kb_releasing(OIS::KeyCode button_ID)
{
    sys0.mKeyboard->capture(); // Met a jour le clavier
	return lastKeyboardState[static_cast<Ogre::uint>(button_ID)] &&  !mKeyboard->isKeyDown( button_ID ) ;
}

Ogre_ini sys0;

































































































