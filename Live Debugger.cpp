#include "Namuh_Main_H.h"

using namespace Ogre;
LiveDebugger ldbg;


// Code des méthodes de la classe
LiveDebugger::LiveDebugger(void)
{
	initialised = false;
	overlay = NULL;
}
LiveDebugger::~LiveDebugger(void)
{
}

void LiveDebugger::enable()
{
	if (Ogre::OverlayManager::getSingleton().getByName("GraphOverlay") == 0)		// Won't recreate it if not needed, eh ?
		overlay =  Ogre::OverlayManager::getSingleton().create( "GraphOverlay" );
	overlay->show();

	if(GuiSandboxMan.winMgr->isWindowPresent("Live_debugger") && !initialised)
	{
		window = GuiSandboxMan.winMgr->getWindow("Live_debugger");
		window->setText("======= Live Debugger Initialised =======\n");
		sys1.logmsg("[Live Debugger] : Initialisation completed");
		initialised = true;
	}
	else
	{
		if(initialised)sys1.logmsg("(Live Debugger Exeption) while enabling : Debugger already enabled");
		else sys1.logmsg("(Live Debugger Exeption) while enabling : No window named 'Sandbox_interface/Live_debugger' can be found");
	}
}

void LiveDebugger::send(string msg, bool immediaterender)
{
	if(initialised)
	{
		if(raknet_man.initialised)
		{
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_REMOTE_DEBUG_MESSAGE);
			bsOut.Write(RakNet::RakString(&msg[0]));
			//raknet_man.peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
		}
		if(window->getText().size() > 12000) // protection contre la divergence de la taille du log
			window->setText("");
		window->setText(window->getText()+msg+"\n");
		window->setProperty("VertScrollPosition", window->getProperty("VertExtent"));
		if(immediaterender)GuiSandboxMan.sys->renderGUI(); // met a jour l'écran, des fois que...
		if(immediaterender)sys0.mRoot->renderOneFrame();
		sys1.logmsg("(Live Debugger Trace) : "+msg);
	}
	else
	{
		sys1.logmsg("(Live Debugger Exeption) Unable to send message '"+msg+"' : Debugger not enbled ");
	}
}

void LiveDebugger::disable()
{
	if(initialised)
	{
		if(GuiSandboxMan.winMgr->isWindowPresent("Live_debugger"))
		{
			window = GuiSandboxMan.winMgr->getWindow("Live_debugger");
			window->setText("Live Debugger disabled\n");
		}
		sys1.logmsg("(Live Debugger) : Disabled");
		initialised = false;
	}
	else
		sys1.logmsg("(Live Debugger Exeption) Unable to disable : Debugger not initialised ");
}

void LiveDebugger::graph(string Name, string YFlowName)
{
	Graphs[Name].YFlowConnexions[YFlowName] = YFlowName;
	Graphs[Name].ConnectTimeToXAxis("Time");

	Graphs[Name].XDatas["Time"].AutoKeepAllInRange = false;
	Graphs[Name].XDatas["Time"].AutoFollowLastValue = true;
	Graphs[Name].XDatas["Time"].AutoFollowRange = 60; // 1min/graph

	Graphs[Name].YDatas[YFlowName].AutoFollowLastValue = false;
	Graphs[Name].YDatas[YFlowName].AutoCenterLastValue = false;
	Graphs[Name].YDatas[YFlowName].DataColour = Ogre::ColourValue(	Ogre::Math::RangeRandom(0.5,1.0),
																	Ogre::Math::RangeRandom(0.5,1.0),
																	Ogre::Math::RangeRandom(0.5,1.0),
																	1.0);
	
	Graphs[Name].posY = 0.025f + (Graphs[Name].sizeY +0.005f) * (Graphs.size()-1);
}

void LiveDebugger::UpdateGrantedGraphes()
{
	for (	std::map<std::string, LdbgGraph>::iterator GraphIterator = Graphs.begin();
			GraphIterator != Graphs.end();
			GraphIterator++)
		if(GraphIterator->second.automatic_update)
			GraphIterator->second.Update();
}























































































































