#include "Namuh_Main_H.h"

using namespace Ogre;
Paged_geometry_mgr Paged_man;


// Code des méthodes de la classe
Paged_geometry_mgr::Paged_geometry_mgr(void)
{
	initialised = false;
}
Paged_geometry_mgr::~Paged_geometry_mgr(void)
{
}

void Paged_geometry_mgr::initialise(bool PhysX)
{

	if( !initialised && BoolFromName(Map.MapDataPool,"Vegetation_Enabled"))
	{
		initialised = true;

		sys1.logmsg("[Paged Manager] Initialistation reached");

		std::vector<int> Vegetation_object_Indexes = SubPoolsIndexesFromName(Map.MapDataPool,"Vegetation object pager");
		uint Nb_Vegetation_object = Vegetation_object_Indexes.size();
		float World_Horisontal_Size = FloatFromName(Map.MapDataPool,"World_Horisontal_Size");
		Entity_list.clear();	// Makes sure we have a clean entity set

		for(uint i=0;i<Nb_Vegetation_object;i++)
		{
			ldbg.send("(Paged Manager) Loading Vegetation object n°"+StringConverter::toString(i+1)+" of " +StringConverter::toString(Nb_Vegetation_object));
			PagedGeometry* Vegetation_object = new PagedGeometry(sys0.cam, FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]],"Page_Size"));
			Geom_Pager.push_back(Vegetation_object);						
			Vegetation_object->setInfinite();

			Vegetation_object->addDetailLevel<WindBatchPage>(FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]],"Baches_Distance"),FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]],"Baches_FadingDistance"));
			Vegetation_object->addDetailLevel<ImpostorPage>(FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]],"Impostor_Distance"),FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]],"Impostor_FadingDistance"));
			
			//Vegetation_object_loader = new TreeLoader3D(Vegetation_object, TBounds(0, 0, Map.World_Horisontal_Size, Map.World_Horisontal_Size));
			Vegetation_object_loader = new TreeLoader2D(Vegetation_object, TBounds(0, 0, World_Horisontal_Size, World_Horisontal_Size));
			Vegetation_object_loader->setHeightFunction(&terrain_height); // need a terrain height funtion
			Vegetation_object->setPageLoader(Vegetation_object_loader);
			Forests::ImpostorPage::setImpostorResolution(IntFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]],"Impostor_Resolution"));  
			//Forests::ImpostorPage::setImpostorPivot(Ogre::BillboardOrigin::BBO_BOTTOM_CENTER);
			//Vegetation_object_loader->setColorMap("Tex.png"); A voir si on veux implémenter

			//Load entities
			std::vector<Ogre::Entity*> Local_Entity_list; // On ajoute une sous liste au tableau des entités visuelles
				Entity_list.push_back(Local_Entity_list);
			std::vector<NxOgre::Mesh*> Local_PhysX_Mesh_list; // On ajoute une sous liste au tableau des maillages physiques
				PhysX_Mesh_list.push_back(Local_PhysX_Mesh_list);
			std::vector<std::pair<float, float>>	Local_RndScl_Lst;

			std::vector<int> Vegetation_object_entity_Indexes = SubPoolsIndexesFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]],"Entity");
			size_t Nb_Vegetation_object_entity = Vegetation_object_entity_Indexes.size();

			for (uint j=0;j<Nb_Vegetation_object_entity;j++)
			{
				// Ogre loading of mesh
				Entity *Object = sys0.mgr->createEntity("VegetationObj"+StringConverter::toString(i)+"Entity"+StringConverter::toString(j)+"_"+toOgStr(sys0.timer.getMicroseconds()),StringFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]].SubPool[Vegetation_object_entity_Indexes[j]],"Visual_FileName"));
				Entity_list[i].push_back(Object);
				// PhysX loading
				NxOgre::Mesh* Object_NxMesh = NxOgre::MeshManager::getSingleton()->load("ogre://General/"+StringFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]].SubPool[Vegetation_object_entity_Indexes[j]],"PhysX_FileName"));
				PhysX_Mesh_list[i].push_back(Object_NxMesh);
				// Wind parameter
				Vegetation_object->setCustomParam(Entity_list[i][j]->getName(), "windFactorX", FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]].SubPool[Vegetation_object_entity_Indexes[j]],"windFactorX"));
				Vegetation_object->setCustomParam(Entity_list[i][j]->getName(), "windFactorY", FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]].SubPool[Vegetation_object_entity_Indexes[j]],"windFactorY"));
				
				
				//  ==== Random scale parameter ====
				float	minScl = 0.00000001f, // min values for min and max scales
						maxScl = 0.00001f;
			
				// Strictly positive Scale 
				if (minScl < FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]].SubPool[Vegetation_object_entity_Indexes[j]],"RandomScale_MinValue"))
					minScl = FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]].SubPool[Vegetation_object_entity_Indexes[j]],"RandomScale_MinValue");					
				if (maxScl < FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]].SubPool[Vegetation_object_entity_Indexes[j]],"RandomScale_MaxValue"))
					maxScl = FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]].SubPool[Vegetation_object_entity_Indexes[j]],"RandomScale_MaxValue");
			
				// Default min and max to 1
				if (-1 == FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]].SubPool[Vegetation_object_entity_Indexes[j]],"RandomScale_MinValue"))
					minScl = 1;
				if (-1 == FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]].SubPool[Vegetation_object_entity_Indexes[j]],"RandomScale_MaxValue"))
					maxScl = 1;

				if (maxScl < minScl)	minScl = maxScl;

				if (minScl != maxScl || maxScl != 1)
					Local_RndScl_Lst.push_back(std::pair<float, float>(minScl, maxScl));
				else
					Local_RndScl_Lst.push_back(std::pair<float, float>(-1, -1));  // Do not scale

			}
			
			//Randomly place some objects on the cliffs or at the specified altitude
			size_t	Object_Count = IntFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]],"Object_Count");
			float	RandomPosition_Y_minDelta = 	FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]],"RandomPosition_Y_minDelta"),
					RandomPosition_Y_MaxDelta =		FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]],"RandomPosition_Y_MaxDelta"),
					RandomPosition_Y_LowerBound =	FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]],"RandomPosition_Y_LowerBound"),
					RandomPosition_Y_UpperBound  =	FloatFromName(Map.MapDataPool.SubPool[Vegetation_object_Indexes[i]],"RandomPosition_Y_UpperBound");

			// WARING : pas d'acces aux dataPools sur une boucle autant utilisée (acces aux datapools très lent)
			for (uint r = 0; r < Object_Count;)
			{
				float fyaw = Ogre::Math::RangeRandom(0, 6.28);
				yaw = Ogre::Radian(fyaw);

				position.x = Ogre::Math::RangeRandom(0, World_Horisontal_Size);
				position.z = Ogre::Math::RangeRandom(0, World_Horisontal_Size);
				position.y = terrain_height(position.x, position.z,0);
				float delta_y = pow((terrain_height(position.x-10, position.z+10,0) - terrain_height(position.x+10, position.z-10,0)),2)
								+pow((terrain_height(position.x+10, position.z-10,0) - terrain_height(position.x-10, position.z+10,0)),2);

				// on vérifie que delta_y appartient a [RandomPosition_Y_minDelta;RandomPosition_Y_MaxDelta]
				// et que position.y appartient a [RandomPosition_Y_LowerBound;RandomPosition_Y_UpperBound]
				if(	RandomPosition_Y_minDelta < delta_y 
					&& delta_y < RandomPosition_Y_MaxDelta
					&& RandomPosition_Y_LowerBound < position.y 
					&& position.y < RandomPosition_Y_UpperBound)
				{
					float rnd = Ogre::Math::RangeRandom(0,Nb_Vegetation_object_entity); // prend une entité au hasard
					for(uint j=0;j<Nb_Vegetation_object_entity;j++)  // sorte de switch automatisé
						if (j< rnd && rnd < j+1)
						{		
							NxOgre::Mesh* Object_NxMesh_scaled;
							float	fscale;

							if (Local_RndScl_Lst[j].first != -1)
							{	
								fscale = Ogre::Math::RangeRandom(Local_RndScl_Lst[j].first, Local_RndScl_Lst[j].second);	//pas inférieur a 1, ca bug =_=  (12/08/13 : Ca n'a pas l'air)
								scale = fscale;

								if(PhysX)
								{
									// mise a l'échelle du maillaige physique, attention, opération longue
									NxOgre::ManualMesh m = PhysX_Mesh_list[i][j]->getMeshData();
									for(unsigned int k=0;k < m.nbVertices();k++)
										m.scaleVertex(k, NxOgre::Vec3(fscale,fscale,fscale) );
									Object_NxMesh_scaled = m.end();
									m.clean();
								}
								else 
									Object_NxMesh_scaled = PhysX_Mesh_list[i][j];
								
							}
							else
							{
								fscale = scale = 1;
								Object_NxMesh_scaled = PhysX_Mesh_list[i][j];
							}
							Vegetation_object_loader->addTree(Entity_list[i][j], position, yaw, scale);		// Loads the ones of the menu !
							
							if(PhysX)
							{								
								NxOgre::Quat quat = NxOgre::Quat(NxOgre::Math::cos(fyaw*3.14/360), 0, NxOgre::Math::sin(fyaw*3.14/360), 0);
								quat.normalise();
								NxOgre::Matrix44 mat = NxOgre::Matrix44( NxOgre::Vec3(position.x, position.y, position.z) ,quat);
								physXCreatedGeometry.push_back(PhysX_man.mScene->createSceneGeometry( NxOgre::TriangleGeometryDescription(Object_NxMesh_scaled) ,mat));
							}
							
							r++;
						} 		
				}
			}
		}



		std::vector<int> Vegetation_image_Indexes = SubPoolsIndexesFromName(Map.MapDataPool,"Vegetation image pager");
		size_t Nb_Vegetation_image = Vegetation_image_Indexes.size();

		for(uint i=0;i<Nb_Vegetation_image;i++)
		{
			ldbg.send("(Paged Manager) Loading Vegetation image n°"+StringConverter::toString(i+1)+" of " +StringConverter::toString(Nb_Vegetation_image));
			PagedGeometry* Vegetation_image = new PagedGeometry(sys0.cam, FloatFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"Page_Size"));
			Geom_Pager.push_back(Vegetation_image);
			Vegetation_image->addDetailLevel<GrassPage>(FloatFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"Distance"),FloatFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"FadeDistance"));
			Vegetation_image_loader = new GrassLoader(Vegetation_image);
			Vegetation_image->setPageLoader(Vegetation_image_loader);	//Assign the "treeLoader" to be used to load geometry for the PagedGeometry instance
			Vegetation_image_loader->setHeightFunction(&terrain_height); // grass need a terrain height funtion

			imageLayerlst.push_back(Vegetation_image_loader->addLayer(StringFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"Material_Name")));
			imageLayerlst.back()->setMinimumSize(FloatFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"Size_min"), FloatFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"Size_min"));
			imageLayerlst.back()->setMaximumSize(FloatFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"Size_Max"), FloatFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"Size_Max"));
			imageLayerlst.back()->setAnimationEnabled(true);		//Enable animations
			imageLayerlst.back()->setSwayDistribution(FloatFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"Sway_Distribution"));		//Sway fairly unsynchronized
			imageLayerlst.back()->setSwayLength(FloatFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"Sway_Length"));				//Sway back and forth 0.5 units in length
			imageLayerlst.back()->setSwaySpeed(FloatFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"Sway_Speed"));				//Sway 1/2 a cycle every second
			imageLayerlst.back()->setDensity(FloatFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"Density"));				//Relatively dense grass 
			if(IntFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"RenderTechnique") == 0)
				imageLayerlst.back()->setRenderTechnique(Forests::GRASSTECH_QUAD);
			if(IntFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"RenderTechnique") == 1)
				imageLayerlst.back()->setRenderTechnique(Forests::GRASSTECH_CROSSQUADS);
			if(IntFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"RenderTechnique") == 2)
				imageLayerlst.back()->setRenderTechnique(Forests::GRASSTECH_SPRITE);
			imageLayerlst.back()->setFadeTechnique(FADETECH_GROW);	//Distant grass should slowly raise out of the ground when coming in range
			if(StringFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"ColorMap") != "None")
				imageLayerlst.back()->setColorMap(StringFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"ColorMap"));
			imageLayerlst.back()->setDensityMap(StringFromName(Map.MapDataPool.SubPool[Vegetation_image_Indexes[i]],"DensityMap"));
			imageLayerlst.back()->setMapBounds(TBounds(0, 0, FloatFromName(Map.MapDataPool,"World_Horisontal_Size"), FloatFromName(Map.MapDataPool,"World_Horisontal_Size"))); // to scale density and color texture to terrain size
			imageLayerlst.back()->setLightingEnabled(true);
		}
			
		ldbg.send("(Paged Manager) Updating Paged instances");
		for(uint i=0;i<(Nb_Vegetation_object+Nb_Vegetation_image);i++)
			Geom_Pager[i]->update();
		ldbg.send("(Paged Manager) Intialisation Complete"); 
	}
}

void Paged_geometry_mgr::update(void)		// REmove those acceses to the datapool  (they are slow and this is realtime function)
{

	if(BoolFromName(Map.MapDataPool,"Vegetation_Enabled"))
	{
		std::vector<int> Vegetation_object_Indexes = SubPoolsIndexesFromName(Map.MapDataPool,"Vegetation object pager");
		uint Nb_Vegetation_object = Vegetation_object_Indexes.size();
		std::vector<int> Vegetation_image_Indexes = SubPoolsIndexesFromName(Map.MapDataPool,"Vegetation image pager");
		uint Nb_Vegetation_image = Vegetation_image_Indexes.size();

		for(uint i=0;i<(Nb_Vegetation_object+Nb_Vegetation_image);i++)
			Geom_Pager[i]->update();
	}
	//rocks_paged->update();
}



void Paged_geometry_mgr::destroy(void)
{
	if(initialised)
	{
	
		Vegetation_object_loader->deleteTrees(Vector3::ZERO, 999999999.9f);
		/*
		while (imageLayerlst.size()-1)
		{
			Vegetation_image_loader->deleteLayer(imageLayerlst.back());
			imageLayerlst.pop_back();
		}

		std::vector<int> Vegetation_object_Indexes = SubPoolsIndexesFromName(Map.MapDataPool,"Vegetation object pager");
		uint Nb_Vegetation_object = Vegetation_object_Indexes.size();
		std::vector<int> Vegetation_image_Indexes = SubPoolsIndexesFromName(Map.MapDataPool,"Vegetation image pager");
		uint Nb_Vegetation_image = Vegetation_image_Indexes.size();

		*/

		ldbg.send(" Stating Paged geometry unloader ");
		while (Geom_Pager.size())
		{
			//Geom_Pager.back()->removeDetailLevels();
			//Geom_Pager[i]->
		//	delete Geom_Pager.back()->getPageLoader();
			delete Geom_Pager.back();
			Geom_Pager.pop_back();
		}
		
		while (physXCreatedGeometry.size())
		{
			PhysX_man.mScene->forceDestroySceneGeometry(physXCreatedGeometry.back());
			physXCreatedGeometry.pop_back();
		}
		//delete rocks_paged->getPageLoader();
		//delete rocks_paged;
		//ldbg.send(" rocks_paged unloaded ");
		
	
		initialised = false;
	}
	else
		ldbg.send("PagedGeometryManager : Tried to destroy a non initialised manager, nothing to do.");

}



















































