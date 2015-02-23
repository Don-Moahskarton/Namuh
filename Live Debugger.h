/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_LIVE_DEBUGGER// Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_LIVE_DEBUGGER

#include "Namuh_Main_H.h"
#include <OgreVector3.h>

class LiveDebugger // Majuscule obligatoire pour éviter les conflits avec les biblio standards
{
	public:
		CEGUI::Window *window;
		bool initialised;
		Ogre::Overlay* overlay;

		std::map<std::string, LdbgGraph> Graphs;
		std::map<std::string, float*> Flows;
		
		void graph(string Name, string YFlowName);
		void UpdateGrantedGraphes();
		
		LiveDebugger(void);
		~LiveDebugger(void);
		void enable(void);
		void send(string message,bool immediate_render = true);
		void disable(void);
};
	extern LiveDebugger ldbg;
#endif // CLASS_PERSO


