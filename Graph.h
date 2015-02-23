/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_LDBGGRAPH // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_LDBGGRAPH

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class LdbgGraph
{
	public:

		bool	automatic_update,
				draw_cursors;
		float	sizeX,
				sizeY;
		float	posX,
				posY;

		HardwarePixelBufferSharedPtr PixelBuffer;
		TexturePtr Texture;
		Ogre::OverlayContainer* panel;

		std::map<string, LdbgGraphAxis > XDatas;
		std::map<string, LdbgGraphAxis > YDatas;
		
		std::map<string, string > XFlowConnexions;
		std::map<string, string > YFlowConnexions;
		std::set<string> XTimeConnexions;
		std::set<string> YTimeConnextions;
		std::map<string, string > XValueCountConnexions;
		std::map<string, string > YValueCountConnexions;

		Ogre::ColourValue BackgroundColor;
		
		LdbgGraph();
		void FeedAxes();
		void Draw();
		void Update();
		~LdbgGraph();


		void ConnectFlowToXAxis(string AxisName, string FlowName);
		void ConnectFlowToYAxis(string AxisName, string FlowName);


		void ConnectTimeToXAxis(string AxisName);
		void ConnectTimeToYAxis(string AxisName);

		void ConnectValueCountToXAxis(string AxisName, string FlowName);
		void ConnectValueCountToYAxis(string AxisName, string FlowName);

		void AddValue(string AxisName, float value);
};
#endif // CLASS_PERSO


