#include "FEBioFluid.h"
#include "FEFluid.h"
#include "FEFluidV.h"
#include "FENewtonianFluid.h"
#include "FECarreauFluid.h"
#include "FECarreauYasudaFluid.h"
#include "FEPowellEyringFluid.h"
#include "FECrossFluid.h"
#include "FEFluidFSI.h"
#include "FEIdealGasIsentropic.h"

#include "FEFluidSolver.h"
#include "FEFluidDomain3D.h"
#include "FEFluidDomain2D.h"

#include "FEFluidTractionLoad.h"
#include "FEFluidNormalTraction.h"
#include "FEFluidNormalVelocity.h"
#include "FEFluidVelocity.h"
#include "FEFluidRotationalVelocity.h"
#include "FEFluidResistanceBC.h"
#include "FETangentialDamping.h"
#include "FETangentialFlowStabilization.h"
#include "FEBackFlowStabilization.h"

#include "FETiedFluidInterface.h"

#include "FEConstraintFrictionlessWall.h"
#include "FEConstraintNormalFlow.h"

#include "FEBioFluidPlot.h"
#include "FEBioFluidData.h"

#include "FEFluidDomainFactory.h"

//-----------------------------------------------------------------------------
//! Initialization of the FEBioFluid module. This function registers all the classes
//! in this module with the FEBio framework.
void FEBioFluid::InitModule()
{
	//-----------------------------------------------------------------------------
	// Domain factory
	FECoreKernel& febio = FECoreKernel::GetInstance();
	febio.RegisterDomain(new FEFluidDomainFactory);

	// define the fluid module
	febio.CreateModule("fluid");

//-----------------------------------------------------------------------------
// solver classes
REGISTER_FECORE_CLASS(FEFluidSolver, "fluid");

//-----------------------------------------------------------------------------
// Materials
REGISTER_FECORE_CLASS(FEFluid             , "fluid"         );
REGISTER_FECORE_CLASS(FEFluidV            , "fluidV"        );
REGISTER_FECORE_CLASS(FENewtonianFluid    , "Newtonian fluid");
REGISTER_FECORE_CLASS(FECarreauFluid      , "Carreau"       );
REGISTER_FECORE_CLASS(FECarreauYasudaFluid, "Carreau-Yasuda");
REGISTER_FECORE_CLASS(FEPowellEyringFluid , "Powell-Eyring" );
REGISTER_FECORE_CLASS(FECrossFluid        , "Cross"         );
REGISTER_FECORE_CLASS(FEIdealGasIsentropic, "ideal gas isentropic");

//-----------------------------------------------------------------------------
// Domain classes
REGISTER_FECORE_CLASS(FEFluidDomain3D, "fluid-3D");
REGISTER_FECORE_CLASS(FEFluidDomain2D, "fluid-2D");

//-----------------------------------------------------------------------------
// Surface loads
REGISTER_FECORE_CLASS(FEFluidTractionLoad          , "fluid viscous traction");
REGISTER_FECORE_CLASS(FEFluidNormalTraction        , "fluid normal traction");
REGISTER_FECORE_CLASS(FEFluidNormalVelocity        , "fluid normal velocity");
REGISTER_FECORE_CLASS(FEFluidVelocity              , "fluid velocity");
REGISTER_FECORE_CLASS(FEFluidRotationalVelocity    , "fluid rotational velocity");
REGISTER_FECORE_CLASS(FEFluidResistanceBC          , "fluid resistance");
REGISTER_FECORE_CLASS(FETangentialDamping          , "fluid tangential damping");
REGISTER_FECORE_CLASS(FETangentialFlowStabilization, "fluid tangential stabilization");
REGISTER_FECORE_CLASS(FEBackFlowStabilization      , "fluid backflow stabilization");
    
//-----------------------------------------------------------------------------
// Contact interfaces
REGISTER_FECORE_CLASS(FETiedFluidInterface, "tied-fluid");
   
//-----------------------------------------------------------------------------
// constraint classes
REGISTER_FECORE_CLASS(FEConstraintFrictionlessWall, "frictionless fluid wall");
REGISTER_FECORE_CLASS(FEConstraintNormalFlow      , "normal fluid flow"      );

//-----------------------------------------------------------------------------
// classes derived from FEPlotData
REGISTER_FECORE_CLASS(FEPlotDisplacement               , "displacement"             );
REGISTER_FECORE_CLASS(FEPlotNodalFluidVelocity         , "nodal fluid velocity"     );
REGISTER_FECORE_CLASS(FEPlotNodalRelativeFluidVelocity , "nodal relative fluid velocity"     );
REGISTER_FECORE_CLASS(FEPlotFluidDilatation            , "fluid dilatation"         );
REGISTER_FECORE_CLASS(FEPlotElasticFluidPressure	   , "elastic fluid pressure"   );
REGISTER_FECORE_CLASS(FEPlotFluidVolumeRatio		   , "fluid volume ratio"       );
REGISTER_FECORE_CLASS(FEPlotFluidDensity               , "fluid density"            );
REGISTER_FECORE_CLASS(FEPlotFluidDensityRate           , "fluid density rate"       );
REGISTER_FECORE_CLASS(FEPlotFluidVelocity              , "fluid velocity"           );
REGISTER_FECORE_CLASS(FEPlotFluidTemperature           , "fluid temperature"        );
REGISTER_FECORE_CLASS(FEPlotRelativeFluidVelocity      , "relative fluid velocity"  );
REGISTER_FECORE_CLASS(FEPlotFluidAcceleration          , "fluid acceleration"       );
REGISTER_FECORE_CLASS(FEPlotFluidVorticity             , "fluid vorticity"          );
REGISTER_FECORE_CLASS(FEPlotElementFluidStress         , "fluid stress"             );
REGISTER_FECORE_CLASS(FEPlotElementFluidRateOfDef      , "fluid rate of deformation");
REGISTER_FECORE_CLASS(FEPlotFluidStressPowerDensity    , "fluid stress power density");
REGISTER_FECORE_CLASS(FEPlotFluidHeatSupplyDensity     , "fluid heat supply density");
REGISTER_FECORE_CLASS(FEPlotFluidSurfaceForce          , "fluid surface force"      );
REGISTER_FECORE_CLASS(FEPlotFluidSurfaceTractionPower  , "fluid surface traction power");
REGISTER_FECORE_CLASS(FEPlotFluidSurfaceEnergyFlux     , "fluid surface energy flux");
REGISTER_FECORE_CLASS(FEPlotFluidShearViscosity        , "fluid shear viscosity"    );
REGISTER_FECORE_CLASS(FEPlotFluidMassFlowRate          , "fluid mass flow rate"     );
REGISTER_FECORE_CLASS(FEPlotFluidStrainEnergyDensity   , "fluid strain energy density");
REGISTER_FECORE_CLASS(FEPlotFluidKineticEnergyDensity  ,"fluid kinetic energy density");
REGISTER_FECORE_CLASS(FEPlotFluidEnergyDensity         , "fluid energy density"     );
REGISTER_FECORE_CLASS(FEPlotFluidElementStrainEnergy   , "fluid element strain energy");
REGISTER_FECORE_CLASS(FEPlotFluidElementKineticEnergy  , "fluid element kinetic energy");
REGISTER_FECORE_CLASS(FEPlotFluidElementLinearMomentum , "fluid element linear momentum");
REGISTER_FECORE_CLASS(FEPlotFluidElementAngularMomentum, "fluid element angular momentum");
REGISTER_FECORE_CLASS(FEPlotFluidElementCenterOfMass   , "fluid element center of mass");
REGISTER_FECORE_CLASS(FEPlotFluidFlowRate              , "fluid flow rate");
REGISTER_FECORE_CLASS(FEPlotFluidPressure              , "fluid pressure");

//-----------------------------------------------------------------------------
REGISTER_FECORE_CLASS(FENodeFluidXVel          , "nfvx");
REGISTER_FECORE_CLASS(FENodeFluidYVel          , "nfvy");
REGISTER_FECORE_CLASS(FENodeFluidZVel          , "nfvz");
REGISTER_FECORE_CLASS(FELogElemFluidPosX       , "fx");
REGISTER_FECORE_CLASS(FELogElemFluidPosY       , "fy");
REGISTER_FECORE_CLASS(FELogElemFluidPosZ       , "fz");
REGISTER_FECORE_CLASS(FELogElasticFluidPressure, "fp");
REGISTER_FECORE_CLASS(FELogFluidVolumeRatio    , "fJ");
REGISTER_FECORE_CLASS(FELogFluidDensity        , "fd");
REGISTER_FECORE_CLASS(FELogFluidStressPower    , "fsp");
REGISTER_FECORE_CLASS(FELogFluidVelocityX      , "fvx");
REGISTER_FECORE_CLASS(FELogFluidVelocityY      , "fvy");
REGISTER_FECORE_CLASS(FELogFluidVelocityZ      , "fvz");
REGISTER_FECORE_CLASS(FELogFluidAccelerationX  , "fax");
REGISTER_FECORE_CLASS(FELogFluidAccelerationY  , "fay");
REGISTER_FECORE_CLASS(FELogFluidAccelerationZ  , "faz");
REGISTER_FECORE_CLASS(FELogFluidVorticityX     , "fwx");
REGISTER_FECORE_CLASS(FELogFluidVorticityY     , "fwy");
REGISTER_FECORE_CLASS(FELogFluidVorticityZ     , "fwz");
REGISTER_FECORE_CLASS(FELogFluidStressXX       , "fsxx");
REGISTER_FECORE_CLASS(FELogFluidStressYY       , "fsyy");
REGISTER_FECORE_CLASS(FELogFluidStressZZ       , "fszz");
REGISTER_FECORE_CLASS(FELogFluidStressXY       , "fsxy");
REGISTER_FECORE_CLASS(FELogFluidStressYZ       , "fsyz");
REGISTER_FECORE_CLASS(FELogFluidStressXZ       , "fsxz");
REGISTER_FECORE_CLASS(FELogFluidRateOfDefXX    , "fdxx");
REGISTER_FECORE_CLASS(FELogFluidRateOfDefYY    , "fdyy");
REGISTER_FECORE_CLASS(FELogFluidRateOfDefZZ    , "fdzz");
REGISTER_FECORE_CLASS(FELogFluidRateOfDefXY    , "fdxy");
REGISTER_FECORE_CLASS(FELogFluidRateOfDefYZ    , "fdyz");
REGISTER_FECORE_CLASS(FELogFluidRateOfDefXZ    , "fdxz");

	febio.SetActiveModule(0);
}
