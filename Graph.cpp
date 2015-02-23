#include "Namuh_Main_H.h"

using namespace Ogre;


// Code des méthodes de la classe
LdbgGraph::LdbgGraph(void)
{
	// Default params
	BackgroundColor = Ogre::ColourValue::Black*0.75;
	sizeX = 0.20f;
	sizeY = 0.1236f;
	posX = 0.28f;
	posY = 0.025f;
	automatic_update = true;
	draw_cursors = true;


	// Create the graph's texture and store a pointer of it
	Texture = TextureManager::getSingleton().createManual(
		"Graph"+ StringConverter::toString(sys0.timer.getMicroseconds()), // name
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D,      // type
		floor(sizeX*sys0.vp->getActualWidth()), floor(sizeY*sys0.vp->getActualHeight()),     // width & height
		0,                // number of mipmaps
		PF_BYTE_RGBA,     // pixel format
		TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	PixelBuffer = Texture->getBuffer();
	 
	// Create a material using the texture
	MaterialPtr material = MaterialManager::getSingleton().create(Texture->getName(),ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	material->getTechnique(0)->getPass(0)->createTextureUnitState(Texture->getName());
	material->getTechnique(0)->getPass(0)->setSceneBlending(SBT_TRANSPARENT_ALPHA);

	// Create a panel
	panel = static_cast<Ogre::OverlayContainer*>(OverlayManager::getSingleton().createOverlayElement("Panel",Texture->getName()));
	panel->setMetricsMode(Ogre::GMM_RELATIVE);
	panel->setPosition(posX,posY);
	panel->setDimensions(sizeX,sizeY);
	panel->setMaterialName(material->getName());
	ldbg.overlay->add2D(panel);
}
LdbgGraph::~LdbgGraph(void)
{
}

void LdbgGraph::ConnectFlowToXAxis(string AxisName, string FlowName)
{
	XFlowConnexions[AxisName] = FlowName;
}

void LdbgGraph::ConnectFlowToYAxis(string AxisName, string FlowName)
{
	YFlowConnexions[AxisName] = FlowName;
}

void LdbgGraph::ConnectTimeToXAxis(string AxisName)
{
	XTimeConnexions.insert(AxisName);
}

void LdbgGraph::ConnectTimeToYAxis(string AxisName)
{
	YTimeConnextions.insert(AxisName);
}

void LdbgGraph::ConnectValueCountToXAxis(string AxisName, string FlowName)
{
	XValueCountConnexions[AxisName] = FlowName;
}

void LdbgGraph::ConnectValueCountToYAxis(string AxisName, string FlowName)
{
	YValueCountConnexions[AxisName] = FlowName;
}

void LdbgGraph::AddValue(string AxisName, float value)
{
	if(XDatas.count(AxisName))XDatas[AxisName].Datas.push_back(value);
	if(YDatas.count(AxisName))YDatas[AxisName].Datas.push_back(value);
}


void LdbgGraph::Draw()
{

	// Repositionne le grpahe, des fois qu'il aurai bougé
	panel->setPosition(posX,posY);

	// Enlève tout les child du panel (Pour pouvoir en avoir un nombre dynamique) TODO : Find a solution do dignostic if a child is already attached to the panel
	{
		Ogre::OverlayContainer::ChildIterator it = panel->getChildIterator();
		while(it.hasMoreElements())
		  panel->removeChild(dynamic_cast<Ogre::TextAreaOverlayElement*>(it.getNext())->getName());
	}



	PixelBuffer->lock(HardwareBuffer::HBL_DISCARD);
	const PixelBox& PixelBox = PixelBuffer->getCurrentLock();

	Ogre::uint32 *data = static_cast<Ogre::uint32*>(PixelBox.data);
	size_t height = PixelBox.getHeight();
	size_t width = PixelBox.getWidth();
	size_t pitch = PixelBox.rowPitch; // Skip between rows of image

	// Remplit le graphe avec l'arrière plan
	for(size_t y=0; y<height; ++y)
		for(size_t x=0; x<width; ++x)
		{
			data[pitch*y + x] =  static_cast<Ogre::uint32>(BackgroundColor.getAsARGB());
		}


		
	// Chaque Yaxis trace une coube par Xaxis
	for (	std::map<string, LdbgGraphAxis >::iterator XAxisIterator = XDatas.begin();
			XAxisIterator != XDatas.end();
			XAxisIterator++)
	{
		// Trace la grille X
		Ogre::uint Xgridpixelstep = 0.9*width*XAxisIterator->second.GridInterval/(XAxisIterator->second.max-XAxisIterator->second.min);
		if(Xgridpixelstep != 0)
			for(Ogre::uint  x=0.1*width; x<width; x += Xgridpixelstep)
				for(Ogre::uint  y=0; y<height*0.9; ++y)
					data[pitch*y + x] =  static_cast<Ogre::uint32>(XAxisIterator->second.GridColour.getAsARGB());

		// Trace l'axe X
		for(size_t x=(0.1*width+1); x<width; ++x)
			data[pitch*size_t(height*0.9) + x] =  static_cast<Ogre::uint32>(XAxisIterator->second.DataColour.getAsARGB());

		for (	std::map<string, LdbgGraphAxis >::iterator YAxisIterator = YDatas.begin();
				YAxisIterator != YDatas.end();
				YAxisIterator++)
		{
			
			// Trace la grille Y
			Ogre::uint Ygridpixelstep = 0.9*height*YAxisIterator->second.GridInterval/(YAxisIterator->second.max-YAxisIterator->second.min);
			if(Ygridpixelstep != 0)
				for(Ogre::uint y=0; y<(height*0.9-1); y += Ygridpixelstep)
					for(Ogre::uint  x=0.1*width; x<width; ++x)
						data[pitch*y + x] =  static_cast<Ogre::uint32>(XAxisIterator->second.GridColour.getAsARGB());

			// Trace l'axe Y
			for(size_t y=0; y<height*0.9; ++y)
				data[pitch*y + size_t(width*0.1)] =  static_cast<Ogre::uint32>(YAxisIterator->second.DataColour.getAsARGB());

			// Trace le graphe pour cette paire d'axe
			for (	std::vector<float>::iterator YValueIterator = YAxisIterator->second.Datas.begin(), XValueIterator = XAxisIterator->second.Datas.begin();
					YValueIterator != YAxisIterator->second.Datas.end() || XValueIterator != XAxisIterator->second.Datas.end();
					YValueIterator++, XValueIterator++)
			{
				if( XAxisIterator->second.max > *XValueIterator && *XValueIterator > XAxisIterator->second.min &&
					YAxisIterator->second.max > *YValueIterator && *YValueIterator > YAxisIterator->second.min )
				{
					Ogre::uint	x = 0.1*width+0.9*width*(*XValueIterator-XAxisIterator->second.min)/(XAxisIterator->second.max-XAxisIterator->second.min),
								y = 0.9*height*(1-(*YValueIterator-YAxisIterator->second.min)/(YAxisIterator->second.max-YAxisIterator->second.min));
					
					data[pitch*y + x] = static_cast<Ogre::uint32>(YAxisIterator->second.DataColour.getAsARGB());
				}
			}
		}
	}

	//  Trace les RulesMarks
	for (	std::map<string, LdbgGraphAxis >::iterator XAxisIterator = XDatas.begin();
			XAxisIterator != XDatas.end();
			XAxisIterator++)
		for (	float Rulervalue = XAxisIterator->second.min;Rulervalue < XAxisIterator->second.max;Rulervalue += XAxisIterator->second.RulerInterval)
		{
			Ogre::uint	x = 0.1*width+0.9*width*(Rulervalue-XAxisIterator->second.min)/(XAxisIterator->second.max-XAxisIterator->second.min);
			for (	size_t y=(0.9*height-XAxisIterator->second.Rulersize/2);
					y<(height*0.9+XAxisIterator->second.Rulersize/2); 
					++y)
				data[pitch*y + x] =  static_cast<Ogre::uint32>(XAxisIterator->second.RulerColour.getAsARGB());
		}
		
	for (	std::map<string, LdbgGraphAxis >::iterator YAxisIterator = YDatas.begin();
			YAxisIterator != YDatas.end();
			YAxisIterator++)
		for (	float Rulervalue = YAxisIterator->second.min;Rulervalue < YAxisIterator->second.max;Rulervalue += YAxisIterator->second.RulerInterval)
		{
			Ogre::uint	y = 0.9*height*(1-(Rulervalue-YAxisIterator->second.min)/(YAxisIterator->second.max-YAxisIterator->second.min));
			for (	size_t x=(0.1*width-YAxisIterator->second.Rulersize/2);
					x<(width*0.1+YAxisIterator->second.Rulersize/2);
					++x)
				data[pitch*y + x] =  static_cast<Ogre::uint32>(YAxisIterator->second.RulerColour.getAsARGB());
		}

		// Si activé, trace les curseurs
		if(draw_cursors)
		{
			for (	std::map<string, LdbgGraphAxis >::iterator XAxisIterator = XDatas.begin();
					XAxisIterator != XDatas.end();
					XAxisIterator++)
			{
				for (	std::map<string, LdbgGraphAxis >::iterator YAxisIterator = YDatas.begin();
						YAxisIterator != YDatas.end();
						YAxisIterator++)
				{
					// Trace le curseur pour cet axe
					Ogre::uint	y = 0.9*height*(1-(YAxisIterator->second.Datas.back()-YAxisIterator->second.min)/(YAxisIterator->second.max-YAxisIterator->second.min));
					for (size_t x=0.1*width; x<width; ++x)
						data[pitch*y + x] =  static_cast<Ogre::uint32>(YAxisIterator->second.RulerColour.getAsARGB());

					// Affiche la valeur
					if(y>12)y-=12;
					YAxisIterator->second.CursorValue->setPosition(0.1*width,y);
					panel->addChild(YAxisIterator->second.CursorValue);

				}

				// Trace le curseur pour cet axe
				Ogre::uint	x = 0.1*width+0.9*width*(XAxisIterator->second.Datas.back()-XAxisIterator->second.min)/(XAxisIterator->second.max-XAxisIterator->second.min);
				for (size_t y=0; y<height*0.9; ++y)
					data[pitch*y + x] =  static_cast<Ogre::uint32>(XAxisIterator->second.RulerColour.getAsARGB());

				// Affiche la valeur
				if(x+16 > width)x-=20;
				XAxisIterator->second.CursorValue->setPosition(x,height*0.9-12);
				panel->addChild(XAxisIterator->second.CursorValue);
			}
		}

			

	PixelBuffer->unlock();

	
	// Met a jour les rulers	
	for (	std::map<string, LdbgGraphAxis >::iterator XAxisIterator = XDatas.begin();// Remet les nouveaux sur X
			XAxisIterator != XDatas.end();
			XAxisIterator++)
			for (Ogre::uint i=0;i<XAxisIterator->second.RulerValue.size();i++)
		{
			XAxisIterator->second.RulerValue[i].second->setPosition(0.1*width+0.9*width*(XAxisIterator->second.RulerValue[i].first-XAxisIterator->second.min)/(XAxisIterator->second.max-XAxisIterator->second.min),0.92*height);
			panel->addChild(XAxisIterator->second.RulerValue[i].second);
		}
	
	for (	std::map<string, LdbgGraphAxis >::iterator YAxisIterator = YDatas.begin();// Remet les nouveaux sur Y
			YAxisIterator != YDatas.end();
			YAxisIterator++)
		for (Ogre::uint i=0;i<YAxisIterator->second.RulerValue.size();i++)
		{
			YAxisIterator->second.RulerValue[i].second->setPosition(0.01*width,0.9*height*(1-(YAxisIterator->second.RulerValue[i].first-YAxisIterator->second.min)/(YAxisIterator->second.max-YAxisIterator->second.min)));
			panel->addChild(YAxisIterator->second.RulerValue[i].second);
		}

}

void LdbgGraph::FeedAxes()
{
	// Met dans les Datas des Axis les valeurs courantes des Flow
	for (	std::map<string, string >::iterator ConnexionsIterator = XFlowConnexions.begin();
			ConnexionsIterator != XFlowConnexions.end();
			ConnexionsIterator++)
		XDatas[ConnexionsIterator->first].Datas.push_back(*ldbg.Flows[ConnexionsIterator->second]);
	for (	std::map<string, string >::iterator ConnexionsIterator = YFlowConnexions.begin();
			ConnexionsIterator != YFlowConnexions.end();
			ConnexionsIterator++)
		YDatas[ConnexionsIterator->first].Datas.push_back(*ldbg.Flows[ConnexionsIterator->second]);

	// Met dans les Datas des Axis la date
	for (	std::set<string>::iterator ConnexionsIterator = XTimeConnexions.begin();
			ConnexionsIterator != XTimeConnexions.end();
			ConnexionsIterator++)
		XDatas[*ConnexionsIterator].Datas.push_back(float(sys0.timer.getMilliseconds())/1000.0f);
	for (	std::set<string>::iterator ConnexionsIterator = YTimeConnextions.begin();
			ConnexionsIterator != YTimeConnextions.end();
			ConnexionsIterator++)
		YDatas[*ConnexionsIterator].Datas.push_back(float(sys0.timer.getMilliseconds())/1000.0f);
	
	// Met dans les Datas des Axis la nombre de valeurs mémorisé par l'axis connecté
	for (	std::map<string, string >::iterator ConnexionsIterator = XValueCountConnexions.begin();
			ConnexionsIterator != XValueCountConnexions.end();
			ConnexionsIterator++)
		XDatas[ConnexionsIterator->first].Datas.push_back(XDatas[ConnexionsIterator->second].Datas.size());
	for (	std::map<string, string >::iterator ConnexionsIterator = YValueCountConnexions.begin();
			ConnexionsIterator != YValueCountConnexions.end();
			ConnexionsIterator++)
		YDatas[ConnexionsIterator->first].Datas.push_back(YDatas[ConnexionsIterator->second].Datas.size());
}

void LdbgGraph::Update()
{
	this->FeedAxes();

	for (	std::map<string, LdbgGraphAxis > ::iterator AxisIterator = XDatas.begin();
			AxisIterator != XDatas.end();
			AxisIterator++)
		AxisIterator->second.update();
	for (	std::map<string, LdbgGraphAxis > ::iterator AxisIterator = YDatas.begin();
			AxisIterator != YDatas.end();
			AxisIterator++)
		AxisIterator->second.update();
	this->Draw();
}























































































































