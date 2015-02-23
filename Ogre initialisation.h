/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#include "Namuh_Main_H.h"
#ifndef OgreIni // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define OgreIni

class Ogre_ini
{
public:
    static Root *mRoot;
    static OIS::Keyboard *mKeyboard;
    static OIS::Mouse *mMouse;
    static OIS::InputManager *mInputManager;
    static ExitListener *mListener;
    static string secName, typeName, archName;
    static ConfigFile cf;
    static Ogre::RenderSystem *rs;
    static SceneManager *mgr;
    static Camera* cam;
    static Viewport* vp;
    static size_t windowHnd;
    static std::ostringstream windowHndStr;
    static OIS::ParamList pl;
    static RenderWindow *win;
	static const OIS::MouseState* ms;
	static ShadowCameraSetupPtr mPSSMSetup;
	static Ogre::Timer timer;
	static Ogre::RenderTarget* RefractionRenderTarget;
	static string windowName;
	static std::vector<bool> lastKeyboardState;

	static float ScreenSizeRatio;
	static bool last_mousebouttons_state[8];

    Ogre_ini(void);
	void update_SmartMouse();
	void update_SmartKeyboard();
	bool ms_pressing(char);
	bool ms_releasing(char);
	bool kb_pressing(OIS::KeyCode);
	bool kb_releasing(OIS::KeyCode);
};

extern Ogre_ini sys0;
#endif
