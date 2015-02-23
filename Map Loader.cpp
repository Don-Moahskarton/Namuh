#include "Namuh_Main_H.h"

using namespace Ogre;
Map_loader Map;

NxOgre::SceneGeometry* loadTerrainGeometry(const Ogre::String&, float* , Ogre::uint16, Ogre::Real, Ogre::Real , Ogre::Real , const Ogre::Vector3&);

// Code des méthodes de la classe
Map_loader::Map_loader(void)
{
	//Defining map file syntax
	ParserBalise* Map = new ParserBalise;
	Map->Name = "Map";

	MapSyntax.push_back(Map);

		MapSyntax.back()->BoolKeywords.push_back("Vegetation_Enabled");
		MapSyntax.back()->BoolKeywords.push_back("Weather_Cloud_Enabled");

		MapSyntax.back()->IntKeywords.push_back("Heightmap_Size");
		MapSyntax.back()->IntKeywords.push_back("Heightmap_MaxPixelError");
		MapSyntax.back()->IntKeywords.push_back("World_Bach_Size_min");
		MapSyntax.back()->IntKeywords.push_back("World_Bach_Size_Max");

		MapSyntax.back()->FloatKeywords.push_back("World_Horisontal_Size");
		MapSyntax.back()->FloatKeywords.push_back("World_Vertical_Size");
		MapSyntax.back()->FloatKeywords.push_back("World_Water_Height");
		MapSyntax.back()->FloatKeywords.push_back("Time_Starting");
		MapSyntax.back()->FloatKeywords.push_back("Time_Multiplier");
		MapSyntax.back()->FloatKeywords.push_back("Time_Sunrise");
		MapSyntax.back()->FloatKeywords.push_back("Time_Sunset");
		MapSyntax.back()->FloatKeywords.push_back("World_East_Direction");
		MapSyntax.back()->FloatKeywords.push_back("Weather_Cloud_Humidity");
		MapSyntax.back()->FloatKeywords.push_back("Weather_Cloud_Average_Width");
		MapSyntax.back()->FloatKeywords.push_back("Weather_Cloud_Altitude");
		MapSyntax.back()->FloatKeywords.push_back("Weather_Cloud_Height");
		MapSyntax.back()->FloatKeywords.push_back("Weather_Wind_Speed");
		MapSyntax.back()->FloatKeywords.push_back("Weather_Wind_Direction");
		MapSyntax.back()->FloatKeywords.push_back("Weather_Sky_Exposure");
		MapSyntax.back()->FloatKeywords.push_back("Weather_Sky_MieMultiplier");
		MapSyntax.back()->FloatKeywords.push_back("Faune_Page_Min_Size");
		MapSyntax.back()->FloatKeywords.push_back("Faune_Display_Distance");

		MapSyntax.back()->Vector3Keywords.push_back("Weather_Sky_WaveLength");
		MapSyntax.back()->Vector3Keywords.push_back("World_Position");
		MapSyntax.back()->Vector3Keywords.push_back("Spawn_Position");

		MapSyntax.back()->StringKeywords.push_back("Name");
		MapSyntax.back()->StringKeywords.push_back("Heightmap_File");

		ParserBalise Layer = {"Layer"};
		MapSyntax.back()->SousBalises.push_back(Layer);
		
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Size");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("MaskFactor");
			
			MapSyntax.back()->SousBalises.back().StringKeywords.push_back("Diffuse");
			MapSyntax.back()->SousBalises.back().StringKeywords.push_back("Normal");
			MapSyntax.back()->SousBalises.back().StringKeywords.push_back("Mask");

		ParserBalise Vegetation_object_pager = {"Vegetation object pager"};
		MapSyntax.back()->SousBalises.push_back(Vegetation_object_pager);

			MapSyntax.back()->SousBalises.back().IntKeywords.push_back("Impostor_Resolution");
			MapSyntax.back()->SousBalises.back().IntKeywords.push_back("Object_Count");

			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Page_Size");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Baches_Distance");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Baches_FadingDistance");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Impostor_Distance");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Impostor_FadingDistance");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("RandomPosition_Y_UpperBound");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("RandomPosition_Y_LowerBound");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("RandomPosition_Y_MaxDelta");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("RandomPosition_Y_minDelta");
			
			ParserBalise Entity = {"Entity"};
			MapSyntax.back()->SousBalises.back().SousBalises.push_back(Entity);

				MapSyntax.back()->SousBalises.back().SousBalises.back().FloatKeywords.push_back("RandomScale_MinValue");
				MapSyntax.back()->SousBalises.back().SousBalises.back().FloatKeywords.push_back("RandomScale_MaxValue");
				MapSyntax.back()->SousBalises.back().SousBalises.back().FloatKeywords.push_back("windFactorX");
				MapSyntax.back()->SousBalises.back().SousBalises.back().FloatKeywords.push_back("windFactorY");

				MapSyntax.back()->SousBalises.back().SousBalises.back().StringKeywords.push_back("Visual_FileName");
				MapSyntax.back()->SousBalises.back().SousBalises.back().StringKeywords.push_back("PhysX_FileName");
				
		
		ParserBalise Vegetation_image_pager = {"Vegetation image pager"};
		MapSyntax.back()->SousBalises.push_back(Vegetation_image_pager);

			MapSyntax.back()->SousBalises.back().IntKeywords.push_back("RenderTechnique");

			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Page_Size");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Distance");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("FadeDistance");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Size_min");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Size_Max");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Sway_Distribution");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Sway_Length");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Sway_Speed");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Density");

			MapSyntax.back()->SousBalises.back().StringKeywords.push_back("Material_Name");
			MapSyntax.back()->SousBalises.back().StringKeywords.push_back("ColorMap");
			MapSyntax.back()->SousBalises.back().StringKeywords.push_back("DensityMap");

		ParserBalise Particle = {"Fluid"};
		MapSyntax.back()->SousBalises.push_back(Particle);
		
			MapSyntax.back()->SousBalises.back().IntKeywords.push_back("MaxParticles");
			MapSyntax.back()->SousBalises.back().IntKeywords.push_back("RenderQueueGroupOffset");

			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("KernelRadiusMultiplier");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("RestParticlesPerMetre");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("MotionLimitMultiplier");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("PacketSizeMultiplier");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("CollisionDistanceMultiplier");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Stiffness");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Viscosity");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("ParticleLifetime");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("Rate");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("FluidSpeed");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("RestDensity");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("ReplusionCoefficient");
			MapSyntax.back()->SousBalises.back().FloatKeywords.push_back("ParticleSize");

			MapSyntax.back()->SousBalises.back().Vector3Keywords.push_back("Position");
			MapSyntax.back()->SousBalises.back().Vector3Keywords.push_back("Orientation");
			MapSyntax.back()->SousBalises.back().Vector3Keywords.push_back("EmmiterSize"); // width height depth
			
			MapSyntax.back()->SousBalises.back().StringKeywords.push_back("SimulationMethod");
			MapSyntax.back()->SousBalises.back().StringKeywords.push_back("Type");
			MapSyntax.back()->SousBalises.back().StringKeywords.push_back("Material");

}
Map_loader::~Map_loader(void)
{
}

void Map_loader::load(string mapfile)
{
	// Parsing file with that brand new syntax
	std::vector<NamedDataPool> MapDataPoolArray = Map_Parser.ParseFile(MapSyntax,mapfile);
	MapDataPool = MapDataPoolArray[FindPoolIndexFromName(MapDataPoolArray,"Map")]; // On ne garde que la Pool map

	// Map Loading
	terrainGlobals = OGRE_NEW TerrainGlobalOptions();
	terrainGlobals->setCompositeMapDistance(300);
	TerrainMaterialGeneratorA::SM2Profile* matProfile = static_cast<TerrainMaterialGeneratorA::SM2Profile*>(terrainGlobals->getDefaultMaterialGenerator()->getActiveProfile());
//	matProfile->setReceiveDynamicShadowsEnabled(true);
//	matProfile->setReceiveDynamicShadowsPSSM(static_cast<PSSMShadowCameraSetup*>(sys0.mPSSMSetup.get()));

	string MapName = StringFromName(MapDataPool,"Heightmap_File");

	Ogre::uchar* Heightmap_bin;
	if(MapName != "random")
		img.load(MapName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	else
	{
		// Perlin noise heightmap
		Heightmap_bin = PerlinNoise(IntFromName(MapDataPool,"Heightmap_Size"), 8.0f, 0.45f);
		img.loadDynamicImage(Heightmap_bin,IntFromName(MapDataPool,"Heightmap_Size"),IntFromName(MapDataPool,"Heightmap_Size"),1,PF_L16);
	}
	

   Terrain::ImportData imp;
   imp.inputImage = &img;
   imp.terrainSize = IntFromName(MapDataPool,"Heightmap_Size");
   imp.inputScale = FloatFromName(MapDataPool,"World_Vertical_Size");
   imp.worldSize = FloatFromName(MapDataPool,"World_Horisontal_Size");
   imp.minBatchSize = IntFromName(MapDataPool,"World_Bach_Size_min");
   imp.maxBatchSize = IntFromName(MapDataPool,"World_Bach_Size_Max");
   imp.pos = Vector3FromName(MapDataPool,"World_Position");
   terrainGlobals->setMaxPixelError(IntFromName(MapDataPool,"Heightmap_MaxPixelError"));
   terrainGlobals->setLayerBlendMapSize(IntFromName(MapDataPool,"Heightmap_Size")); // Les blends maps seront redimentionnés a la taille de la heightmap
   
    // Light
	Ogre::Light *mLight0;

   if(!sys0.mgr->hasLight("SunLight"))
   {
		mLight0 = sys0.mgr->createLight("SunLight"); // True sun
		mLight0->setDiffuseColour(0.6f, 0.6f, 0.6f);
		mLight0->setCastShadows(false);
   }
   else
	   mLight0 = sys0.mgr->getLight("SunLight");

   // Create a light
   Ogre::Vector3 lightdir(0, -1, 0);
   lightdir.normalise();

   Ogre::Light* l;
   if(!sys0.mgr->hasLight("SunLight1"))
   {
	   l = sys0.mgr->createLight("SunLight1"); // Shadow and parallax light
	   l->setType(Light::LT_DIRECTIONAL);
	  // l->setShadowFarDistance(200);
	   l->setDirection(lightdir);
	   l->setDiffuseColour(ColourValue(0.4f, 0.4f,0.4f));
	   l->setSpecularColour(ColourValue(0.000f, 0.000f, 0.000f));
   }
   else
	   l = sys0.mgr->getLight("SunLight1");

   terrainGlobals->setLightMapSize(128);
   terrainGlobals->setLightMapDirection(l->getDerivedDirection());
   terrainGlobals->setCompositeMapDiffuse(l->getDiffuseColour());

   std::vector<int> LayerIndexes = SubPoolsIndexesFromName(MapDataPool,"Layer");
   Ogre::uint NbLayer = LayerIndexes.size();
   // textures
   imp.layerList.resize(NbLayer);
   for(Ogre::uint16 t=0;t<NbLayer;++t)
   {
	   imp.layerList[t].worldSize = FloatFromName(MapDataPool.SubPool[LayerIndexes[t]],"Size");
	   imp.layerList[t].textureNames.push_back(StringFromName(MapDataPool.SubPool[LayerIndexes[t]],"Diffuse"));
	   imp.layerList[t].textureNames.push_back(StringFromName(MapDataPool.SubPool[LayerIndexes[t]],"Normal"));
   }
   
	terrain = new Ogre::Terrain(sys0.mgr);
   terrain->prepare(imp); terrain->load();
   //terrain->save("terrain.dat");

   std::vector<TerrainLayerBlendMap*> blendMapList;
   for(Ogre::uint t=1;t<NbLayer;++t)// la première n'a pas de blendmap, ce pourquoi on part de 1
	   blendMapList.push_back(terrain->getLayerBlendMap(t)); 

   int blendmapsize = terrain->getLayerBlendMapSize();
   std::vector<Ogre::Image> ImageList;
   Ogre::Image temp;
   if (MapName != "random")
	   for(Ogre::uint t=1;t<NbLayer;++t)// la première n'a pas de blendmap, ce pourquoi on part de 1
	   {
			ImageList.push_back(temp);
			ImageList.back().load(StringFromName(MapDataPool.SubPool[LayerIndexes[t]],"Mask"), "General"); //Mask de la première Layer n'est jamais utilisé
		    ImageList.back().resize(blendmapsize, blendmapsize);
	   }
   else
   {
		// Cliffs
		ImageList.push_back(temp);
		ImageList.back().loadDynamicImage(Derivation2D(IntFromName(MapDataPool,"Heightmap_Size"),Heightmap_bin),IntFromName(MapDataPool,"Heightmap_Size"),IntFromName(MapDataPool,"Heightmap_Size"),1,PF_L16);
		// Cliffs
		ImageList.push_back(temp);
		ImageList.back().loadDynamicImage(Derivation2D(IntFromName(MapDataPool,"Heightmap_Size"),Heightmap_bin),IntFromName(MapDataPool,"Heightmap_Size"),IntFromName(MapDataPool,"Heightmap_Size"),1,PF_L16);
		// Dry sand
		ImageList.push_back(temp);
		ImageList.back().loadDynamicImage(Heightmap_bin,IntFromName(MapDataPool,"Heightmap_Size"),IntFromName(MapDataPool,"Heightmap_Size"),1,PF_L16);

		ImageList.push_back(temp);
		ImageList.back().loadDynamicImage(Heightmap_bin,IntFromName(MapDataPool,"Heightmap_Size"),IntFromName(MapDataPool,"Heightmap_Size"),1,PF_L16);

		ImageList.push_back(temp);
		ImageList.back().loadDynamicImage(Heightmap_bin,IntFromName(MapDataPool,"Heightmap_Size"),IntFromName(MapDataPool,"Heightmap_Size"),1,PF_L16);

   }
   std::vector<float*> pBlend;
   for(Ogre::uint t=1;t<NbLayer;++t)// blendMapList[0] est la blendmap de la layer 1 d'ou le décalage en t-1
	   pBlend.push_back(blendMapList[t-1]->getBlendPointer());
   
   // blend textures
   for(Ogre::uint t=0;t<NbLayer-1;++t) 
   {
	   float MaskFactor = FloatFromName(MapDataPool.SubPool[LayerIndexes[t]],"MaskFactor");
	   if(MaskFactor == -1) MaskFactor = 1;

       for (Ogre::uint y = 0; y < terrain->getLayerBlendMapSize(); ++y)
	       for (Ogre::uint x = 0; x < terrain->getLayerBlendMapSize(); ++x)
			  *pBlend[t]++ = ImageList[t].getColourAt(x,y,0).r*MaskFactor;
   }

   for(Ogre::uint t=0;t<NbLayer-1;++t)
   {
       blendMapList[t]->dirty();
	   blendMapList[t]->update();
   }
  // terrain->setGlobalColourMapEnabled(true,1024);

   terrainGlobals->getDefaultMaterialGenerator()->generate(terrain);
   terrain->getMaterial()->setShininess(128);
   terrain->getMaterial()->setSpecular(ColourValue(0.1f,0.1f,0.1f));

}


// Perlin noise function
Ogre::uchar* Map_loader::PerlinNoise(Ogre::uint sizeXY, unsigned char octaves, float persistence)
{
	std::vector<unsigned short*> data;
	size_t height = sizeXY;
	size_t width = sizeXY;
	size_t pitch = sizeXY; // Skip between rows of image

	// each data element contain an octave image
	for(Ogre::uchar i = 1; i <= octaves; ++i)
	{
		data.push_back(new unsigned short[height*width]);
		Ogre::uint step = (width+1)/(pow((float)2,(int)i));
		Ogre::uint X = 0, Y = 0;
		while (X<width)
		{
			Y = 0;
			while(Y <height)
			{
				//random mid-point elevation
				data.back()[pitch*Y + X] =  Ogre::Math::RangeRandom(0,65536);

				size_t miny = Y-step,maxy = Y+step; // Define the y range that can be affected by the interpolated midpoint elevation
				if(miny<0)miny=0;
				if(maxy>height)maxy=height;
				
				size_t minx = X-step,maxx = X+step; // Define the x range that can be affected by the interpolated midpoint elevation
				if(minx<0)minx=0;
				if(maxx>width)maxx=width;

				// Cosinusoidal Interpolation (just in the previouly defined range, faster)
				for(size_t y=miny; y<maxy; ++y)
					for(size_t x=minx; x<maxx; ++x)
					{
						// interpolate concernard pixel
						float dist = sqrt(float((x-X)*(x-X)+(y-Y)*(y-Y)));
						dist /= step;
						if(dist < 1)
						{
							dist = (1-cos(3.141*dist))/2;
							data.back()[pitch*y + x] = data.back()[pitch*y + x]*dist + data.back()[pitch*Y + X] * (1-dist);

						}
					}
				
				Y += step;
			}
			X += step;
		}
		
	}

	float factor;
	data.push_back(new unsigned short[height*width]);// ce dernier élément de data contient la somme (pondéré du factor) des octaves

	for(size_t y=0; y<height; ++y)
		for(size_t x=0; x<width; ++x)
		{
			// Chaque pixel est modifié comme étant la somme de tout les octaves pondérés
			factor = (1-persistence)/(1-pow(persistence,(int)octaves));
			for(Ogre::uchar i = 0; i < octaves; ++i)
			{
				data.back()[pitch*y + x] += data[i][pitch*y + x]*factor;
				factor *= persistence;
			}
		}
	return reinterpret_cast<Ogre::uchar*>(data.back());
}

// 2D derivation Analysis function
Ogre::uchar* Map_loader::Derivation2D(Ogre::uint sizeXY, Ogre::uchar* input_char)
{
	unsigned short* input = reinterpret_cast<unsigned short*>(input_char);
	size_t height = sizeXY;
	size_t width = sizeXY;
	size_t pitch = sizeXY; // Skip between rows of image
	unsigned short* output = new unsigned short[height*width];
	
	for(size_t y=0; y<height-1; ++y)
		for(size_t x=0; x<width-1; ++x)
		{
			int	deltaX = input[pitch*y + x + 1] - input[pitch*y + x],
				deltaY = input[pitch*(y+1) + x] - input[pitch*y + x];
			output[pitch*y + x] = 100*(unsigned short)sqrt(float(deltaX*deltaX+deltaY*deltaY)); // factor of 100 on the output
		}

	return reinterpret_cast<Ogre::uchar*>(output);
}

void Map_loader::unload(void)
{
	//sys0.mgr->destroyAllLights();
	terrain->unload();
	terrain->unprepare();
	delete terrain;
}


Ogre::Real terrain_height(float x,float z, void *userData = NULL)
{
	return Map.terrain->getHeightAtWorldPosition(Ogre::Vector3(x,0,z));
}

NxOgre::SceneGeometry* loadTerrainGeometry(const Ogre::String& name, float* data, Ogre::uint16 size, Ogre::Real worldSize, Ogre::Real minHeight, Ogre::Real maxHeight, const Ogre::Vector3& position)
{
	// Create the manual heightfield
	NxOgre::ManualHeightField mhf;
	mhf.begin(size, size);
	Ogre::Real normMin = -32768.0f;
	Ogre::Real normMax = 32767.0f;
	// Sample the data to the manual heightfield
	for(int x = 0; x < size; ++x)
	{
		NxOgre::Enums::HeightFieldTesselation tess = NxOgre::Enums::HeightFieldTesselation_NW_SE;
		for(int z = size-1; z >= 0; --z)
		{
			Ogre::Real height = data[(size * z) + x];
			short sample = (short)(((height - minHeight) / (maxHeight - minHeight)) * (normMax - normMin) + normMin);
			mhf.sample(sample, 0, 0, tess);
			if(tess == NxOgre::Enums::HeightFieldTesselation_NE_SW)
				tess = NxOgre::Enums::HeightFieldTesselation_NW_SE;
			else
				tess = NxOgre::Enums::HeightFieldTesselation_NE_SW;
		}
	}
	// Create the actual heightfield
	NxOgre::HeightField *hf = mhf.end(name.c_str());
	Ogre::Real hf_size = worldSize + (worldSize / size);
	Ogre::Real hf_height = (maxHeight - minHeight) / 2.0f;
	Ogre::Real hf_pose_x = position.x - (worldSize / 2.0f);
	Ogre::Real hf_pose_y = position.y + ((maxHeight + minHeight) / 2.0f);
	Ogre::Real hf_pose_z = position.z - (worldSize / 2.0f);
#if NxOgreVersionMajor <= 1 && NxOgreVersionMinor <= 5
	NxOgre::HeightFieldGeometry* hfg = new NxOgre::HeightFieldGeometry(hf, NxOgre::Vec3(hf_size, hf_height, hf_size));
	hfg->setLocalPose(NxOgre::Matrix44(NxOgre::Vec3(hf_pose_x, hf_pose_y, hf_pose_z)));
	return mScene->createSceneGeometry(hfg);
#else
	NxOgre::HeightFieldGeometryDescription desc(hf, NxOgre::Vec3(hf_size, hf_height, hf_size));
	return PhysX_man.mScene->createSceneGeometry(desc, NxOgre::Matrix44(NxOgre::Vec3(hf_pose_x, hf_pose_y, hf_pose_z)));
#endif

}









































































