#include "Namuh_Main_H.h"

using namespace Ogre;

//extern Hydrax_mgr Hydrax_man;

PhysX_Waterfall PhysX_Waterfall_man;


PhysX_Waterfall:: PhysX_Waterfall(void){};
PhysX_Waterfall::~PhysX_Waterfall(void){};

void PhysX_Waterfall::make(void)
{

	sys1.logmsg("[PhysX Manager] Fluids Initialistation reached");
	std::vector<int> Fluid_object_Indexes = SubPoolsIndexesFromName(Map.MapDataPool,"Fluid");
	uint Nb_Fluid_object = Fluid_object_Indexes.size();

	for(uint i=0;i<Nb_Fluid_object;i++)
	{
		
		NxOgre::FluidDescription Fluid_desc;
		NxOgre::FluidEmitterDescription Emiter_desc;

		Fluid_desc.mMaxParticles =					IntFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"MaxParticles");
		Fluid_desc.mKernelRadiusMultiplier =		FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"KernelRadiusMultiplier");
		Fluid_desc.mRestParticlesPerMetre =			FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"RestParticlesPerMetre");
		Fluid_desc.mMotionLimitMultiplier =			FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"MotionLimitMultiplier");
		Fluid_desc.mPacketSizeMultiplier =			FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"PacketSizeMultiplier");
		Fluid_desc.mCollisionDistanceMultiplier =	FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"CollisionDistanceMultiplier");
		Fluid_desc.mStiffness =						FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"Stiffness");
		Fluid_desc.mViscosity =						FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"Viscosity");
		Fluid_desc.mRestDensity =					FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"RestDensity");
		Fluid_desc.mSimulationMethod = NxOgre::Enums::FluidSimulationMethod_NoParticleInteraction;
		//Fluid_desc.mFlags |= Enums::FluidFlags_Hardware;
		//Fluid_desc.mFlags |= NxOgre::Enums::FluidFlags_DisableGravity;

		NxOgre::Fluid* Fluid_Background = PhysX_man.mRenderSystem->createFluid(Fluid_desc,StringFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"Material"), Critter::Enums::FluidType_OgreParticle);
		((Critter::ParticleRenderable*)Fluid_Background->getRenderable())->getParticleSystem()->setDefaultDimensions(
																				FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"ParticleSize"),
																				FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"ParticleSize"));
		((Critter::ParticleRenderable*)Fluid_Background->getRenderable())->getParticleSystem()->setRenderQueueGroup(((Critter::ParticleRenderable*)Fluid_Background->getRenderable())->getParticleSystem()->getRenderQueueGroup()+IntFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"RenderQueueGroupOffset"));

		Emiter_desc.mPose.set(
			NxOgre::Matrix44(
			Vector3FromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"Position"),
			NxOgre::Quat(Ogre::Math::Cos(0.1f,false),
			NxOgre::Vec3(0.0f,Ogre::Math::Sin(0.1f,false),0.0f))));
		Emiter_desc.mShape = NxOgre::Enums::FluidEmitterShape_Rectangular;
		Emiter_desc.mParticleLifetime = FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"ParticleLifetime");
		Emiter_desc.mRate = FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"Rate");
		Emiter_desc.mFluidSpeed = FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"FluidSpeed");
		Emiter_desc.mType = NxOgre::Enums::FluidEmitterType_FlowRate;
		Emiter_desc.mReplusionCoefficient = FloatFromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"ReplusionCoefficient");
		Emiter_desc.mDimensionX = Vector3FromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"EmmiterSize").x;
		Emiter_desc.mDimensionY = Vector3FromName(Map.MapDataPool.SubPool[Fluid_object_Indexes[i]],"EmmiterSize").y;
		NxOgre::FluidEmitter*	Emitter_Background = Fluid_Background->createEmitter(Emiter_desc);
	}
};











































































