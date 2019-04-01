#pragma once
#include "FEViscousFluid.h"

//-----------------------------------------------------------------------------
// This class evaluates the viscous stress in a Powell-Eyring fluid

class FEBIOFLUID_API FEPowellEyringFluid :	public FEViscousFluid
{
public:
    //! constructor
    FEPowellEyringFluid(FEModel* pfem);
    
    //! viscous stress
    mat3ds Stress(FEMaterialPoint& pt) override;
    
    //! tangent of stress with respect to strain J
    mat3ds Tangent_Strain(FEMaterialPoint& mp) override;
    
    //! tangent of stress with respect to rate of deformation tensor D
    tens4ds Tangent_RateOfDeformation(FEMaterialPoint& mp) override;
    
    //! dynamic viscosity
    double ShearViscosity(FEMaterialPoint& mp) override;
    
    //! bulk viscosity
    double BulkViscosity(FEMaterialPoint& mp) override;
    
public:
    double	m_mu0;		//!< shear viscosity at zero shear rate
    double	m_mui;		//!< shear viscosity at infinite shear rate
    double  m_lam;      //!< time constant
    
    // declare parameter list
    DECLARE_FECORE_CLASS();
};
