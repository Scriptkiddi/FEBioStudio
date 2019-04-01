#pragma once
#include <FECore/FESurfaceLoad.h>
#include <FECore/FESurfaceMap.h>
#include "febiomix_api.h"

//-----------------------------------------------------------------------------
//! This pseudo-surface load is used to calculate the pressure stabilization
//! time constant based on the properties of elements under that surface
//!
class FEBIOMIX_API FEPressureStabilization : public FESurfaceLoad
{
public:
    //! constructor
    FEPressureStabilization(FEModel* pfem);
    
    //! Set the surface to apply the load to
    void SetSurface(FESurface* ps) override;
    
    //! calculate pressure stiffness
    void StiffnessMatrix(const FETimeInfo& tp, FESolver* psolver) override {}
    
    //! calculate residual
    void Residual(const FETimeInfo& tp, FEGlobalVector& R) override {}
    
    //! initialize
    bool Init() override;
    
    //! activate
    void Activate() override;

protected:
    double TimeConstant(FESurfaceElement& el, FESurface& s);
    
protected:
    bool	m_bstab;		//!< flag for calculating stabilization constant
    
    DECLARE_FECORE_CLASS();
};
