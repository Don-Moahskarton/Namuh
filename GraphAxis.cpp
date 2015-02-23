#include "Namuh_Main_H.h"

using namespace Ogre;


// Code des méthodes de la classe
LdbgGraphAxis::LdbgGraphAxis(void)
{
	origin = 0;
	max = 0;
	min = 0;
	RulerInterval = 1.5;
	GridInterval = 5;
	AutoFollowRange = 50; // Unused till Autofollow activated
	MaxRuler = 5;
	Rulersize = 4;

	AutoCenterLastValue = false;
	AutoFollowLastValue = false;
	AutoKeepAllInRange = true;
	AutoKeepOrigin = false;

	DataColour = Ogre::ColourValue(	Ogre::Math::RangeRandom(0.5,1.0),
									Ogre::Math::RangeRandom(0.5,1.0),
									Ogre::Math::RangeRandom(0.5,1.0),
									1.0);
	DataColour.saturate();
	GridColour = Ogre::ColourValue(0.3f,0.3f,0.3f,1.0f);
	RulerColour = Ogre::ColourValue::White;
	TextColour = Ogre::ColourValue::White;

	CursorValue = static_cast<Ogre::TextAreaOverlayElement*>(OverlayManager::getSingleton().createOverlayElement("TextArea", "CursorValue"+ StringConverter::toString(sys0.timer.getMicroseconds())));
	CursorValue->setMetricsMode(Ogre::GMM_PIXELS);
	CursorValue->setDimensions(16, 100);
	CursorValue->setCharHeight(12);
	CursorValue->setFontName("DejaVuSans");
	CursorValue->setColourBottom(TextColour);
	CursorValue->setColourTop(TextColour);

}
LdbgGraphAxis::~LdbgGraphAxis(void)
{
}

void LdbgGraphAxis::setRange_FollowLastValue(bool flag, float range)
{
	AutoFollowLastValue = flag;
	if(flag)
	{
		if(range > 0.0f) AutoFollowRange = range;
		else 
		{
			ldbg.send("(Internal LDBG_GraphAxis Exeption) setRange_FollowLastValue : This range (" + StringConverter::toString(range) + ") isn't greater than 0. Command ingored.");
			AutoFollowRange = -1;
			AutoFollowLastValue = false;
		}
	}
	else
		AutoFollowRange = -1;

}

void LdbgGraphAxis::setRange_CenterLastValue(bool flag, float range)
{
	AutoCenterLastValue = flag;
	if(flag)
	{
		if(range > 0.0f) AutoFollowRange = range;
		else 
		{
			ldbg.send("(Internal LDBG_GraphAxis Exeption) setRange_CenterLastValue : This range (" + StringConverter::toString(range) + ") isn't greater than 0. Command ingored.");
			AutoFollowRange = -1;
			AutoFollowLastValue = false;
		}
	}
	else
		AutoFollowRange = -1;

}

void LdbgGraphAxis::setRange_KeepAllInRange(bool flag, bool keeporgin)
{
	AutoKeepAllInRange = flag;
	AutoKeepOrigin = keeporgin;

	if(!AutoKeepAllInRange)
		AutoKeepOrigin = false;
}


void LdbgGraphAxis::update()
{
	if(AutoKeepAllInRange && AutoFollowLastValue)
	{
		AutoKeepAllInRange = AutoFollowLastValue = false;
		ldbg.send("(Internal LDBG_GraphAxis Exeption) Axis configured both to keep all datas in range and to follow only the last. Both are now disabled");
	}

	CursorValue->setCaption(StringConverter::toString(Datas.back(),3,3));

	if(Datas.size() > 0)
	{
		if(AutoKeepAllInRange)
		{
			// Ajust min to the smallest value
			if(AutoKeepOrigin) min = origin;
			else min = Datas[0];

			for (	std::vector<float>::iterator DataIterator = Datas.begin();
					DataIterator != Datas.end();
					DataIterator++)
				if(*DataIterator < min) min = *DataIterator;

			
			// Ajust max to the greatest value
			if(AutoKeepOrigin) max = origin;
			else max = Datas[0];

			for (	std::vector<float>::iterator DataIterator = Datas.begin();
					DataIterator != Datas.end();
					DataIterator++)
				if(*DataIterator > max) max = *DataIterator;
		}
		if(AutoFollowLastValue)
		{
			min = Datas.back()-AutoFollowRange; 
			max = Datas.back();
		}
		if(AutoCenterLastValue)
		{
			// La dernière valeure ajoutée est centrée sur le scope.
			max = Datas.back()+AutoFollowRange;	
			min = Datas.back()-AutoFollowRange;
		}
	}

	// Recrée les valeurs du ruler
	if(RulerInterval != 0)
	{
		if((max-min)/RulerInterval > MaxRuler) // Evite que le nombre d'overlay diverge
			RulerInterval = (max-min)/MaxRuler;
				
		while((max-min)/RulerInterval > RulerValue.size()) // Ajoute des overlay si besoin
		{
			std::pair<Ogre::uint, Ogre::TextAreaOverlayElement*> NewRulerOverlay;
			NewRulerOverlay.first = 0;
			NewRulerOverlay.second = static_cast<Ogre::TextAreaOverlayElement*>(OverlayManager::getSingleton().createOverlayElement("TextArea", "GraphRuler"+ StringConverter::toString(sys0.timer.getMicroseconds())));
			NewRulerOverlay.second->setMetricsMode(Ogre::GMM_PIXELS);
			NewRulerOverlay.second->setDimensions(16, 100);
			NewRulerOverlay.second->setCharHeight(12);
			NewRulerOverlay.second->setFontName("DejaVuSans");
			NewRulerOverlay.second->setColourBottom(TextColour);
			NewRulerOverlay.second->setColourTop(TextColour);
			RulerValue.push_back(NewRulerOverlay);
		}

		{
			unsigned int i = 0;
			for (	float Rulervalue = min;
					i<RulerValue.size() && Rulervalue < max;
					i++, Rulervalue += RulerInterval)
			{
				RulerValue[i].second->setCaption(StringConverter::toString(Rulervalue,3,3));
				RulerValue[i].first = Rulervalue;
			}
		}
	}
	else
		for (Ogre::uint i=0; i<RulerValue.size(); i++)
			RulerValue[i].second->setCaption("");

}





















































































































































