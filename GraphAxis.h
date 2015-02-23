/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_LDBGGRAPHAXIS // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_LDBGGRAPHAXIS

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class LdbgGraphAxis
{
	public:

		LdbgGraphAxis();
		~LdbgGraphAxis();
		void update();

		float	origin,
				max,
				min,
				RulerInterval,
				GridInterval,
				AutoFollowRange;

		Ogre::uint	MaxRuler,
					Rulersize;

		Ogre::ColourValue DataColour;
		Ogre::ColourValue GridColour;
		Ogre::ColourValue RulerColour;
		Ogre::ColourValue TextColour;

		Ogre::TextAreaOverlayElement* CursorValue;

		std::vector<std::pair<float, Ogre::TextAreaOverlayElement*>> RulerValue;
		std::vector<float> Datas;

		bool	AutoFollowLastValue,
				AutoCenterLastValue,
				AutoKeepAllInRange,
				AutoKeepOrigin;
				
		// Pre-Made range tweaks
		void setRange_CenterLastValue(bool followlastvalue = true, float range = 10);
		void setRange_FollowLastValue(bool followlastvalue = true, float range = 10);
		void setRange_KeepAllInRange(bool keepallinrange = true, bool keeporigin = false);
};

#endif // CLASS_PERSO


