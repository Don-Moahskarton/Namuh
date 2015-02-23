/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef System1 // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define System1 // Ne pas mettre System tout court, sinon CEGUI bug

#include "Namuh_Main_H.h"
class namuh_system
{
	public:

		bool	escQuit;
		bool	MustQuit;
		bool	last_esc;
		float	MoyFPS,
				FPS,
				BestFPS,
				WorstFPS;
		ulong	Watch_LoopStart,
				Watch_LoopEnd;
		int Polycount;
		char hour;
		char min;
		std::vector<ParserBalise*> MainConfigFileSyntax;

		namuh_system(void); //Constructeur
		~namuh_system(void); //Destructeur
		void render(bool force_full_render = true);
		void update(float ellapsedTime, bool force_full_render = true);
		void update_wo_render(void);
		void logmsg(string msg);
		void resetMustQuit(void);

};

extern namuh_system sys1;
#endif

