//
//  FEFiberExpPowSBM.cpp
//  FEBioMix
//
//  Created by Gerard Ateshian on 5/24/15.
//  Copyright (c) 2015 febio.org. All rights reserved.
//

#include "FEFiberExpPowSBM.h"
#include "FEMultiphasic.h"
#include <FECore/log.h>

// define the material parameters
BEGIN_FECORE_CLASS(FEFiberExpPowSBM, FEElasticMaterial)
	ADD_PARAMETER(m_alpha, FE_RANGE_GREATER_OR_EQUAL(0.0), "alpha");
	ADD_PARAMETER(m_beta , FE_RANGE_GREATER_OR_EQUAL(2.0), "beta" );
	ADD_PARAMETER(m_ksi0 , FE_RANGE_GREATER_OR_EQUAL(0.0), "ksi0" );
	ADD_PARAMETER(m_rho0 , FE_RANGE_GREATER_OR_EQUAL(0.0), "rho0" );
	ADD_PARAMETER(m_g    , FE_RANGE_GREATER_OR_EQUAL(0.0), "gamma");
	ADD_PARAMETER(m_sbm  , "sbm"  );
	ADD_PARAMETER(m_thd  , "theta");
	ADD_PARAMETER(m_phd  , "phi"  );
END_FECORE_CLASS();

//-----------------------------------------------------------------------------
// FEFiberExpPow
//-----------------------------------------------------------------------------

bool FEFiberExpPowSBM::Init()
{
	if (FEElasticMaterial::Init() == false) return false;

    // get the parent material which must be a multiphasic material
    FEMultiphasic* pMP = dynamic_cast<FEMultiphasic*> (GetAncestor());
	if (pMP == 0) {
		feLogError("Parent material must be multiphasic");
		return false;
	}
    
    // extract the local id of the SBM whose density controls Young's modulus from the global id
    m_lsbm = pMP->FindLocalSBMID(m_sbm);
	if (m_lsbm == -1) {
		feLogError("Invalid value for sbm");
		return false;
	}
    
    // convert angles from degrees to radians
    double pi = 4*atan(1.0);
    double the = m_thd*pi/180.;
    double phi = m_phd*pi/180.;
    // fiber direction in local coordinate system (reference configuration)
    m_n0.x = cos(the)*sin(phi);
    m_n0.y = sin(the)*sin(phi);
    m_n0.z = cos(phi);

	return true;
}

//-----------------------------------------------------------------------------
mat3ds FEFiberExpPowSBM::Stress(FEMaterialPoint& mp)
{
    FEElasticMaterialPoint& pt = *mp.ExtractData<FEElasticMaterialPoint>();
    FESolutesMaterialPoint& spt = *mp.ExtractData<FESolutesMaterialPoint>();
    
    // initialize material constants
    double rhor = spt.m_sbmr[m_lsbm];
    double ksi = FiberModulus(rhor);
    
    // deformation gradient
    mat3d &F = pt.m_F;
    double J = pt.m_J;
    
    // loop over all integration points
    vec3d n0, nt;
    double In_1, Wl;
    const double eps = 0;
    mat3ds C = pt.RightCauchyGreen();
    mat3ds s;
    
	// get the local coordinate systems
	mat3d Q = GetLocalCS(mp);

    // evaluate fiber direction in global coordinate system
    n0 = Q*m_n0;
    
    // Calculate In = n0*C*n0
    In_1 = n0*(C*n0) - 1.0;
    
    // only take fibers in tension into consideration
    if (In_1 >= eps)
    {
        // get the global spatial fiber direction in current configuration
        nt = F*n0;
        
        // calculate the outer product of nt
        mat3ds N = dyad(nt);
        
        // calculate strain energy derivative
        Wl = ksi*pow(In_1, m_beta-1.0)*exp(m_alpha*pow(In_1, m_beta));
        
        // calculate the fiber stress
        s = N*(2.0*Wl/J);
    }
    else
    {
        s.zero();
    }
    
    return s;
}

//-----------------------------------------------------------------------------
tens4ds FEFiberExpPowSBM::Tangent(FEMaterialPoint& mp)
{
    FEElasticMaterialPoint& pt = *mp.ExtractData<FEElasticMaterialPoint>();
    FESolutesMaterialPoint& spt = *mp.ExtractData<FESolutesMaterialPoint>();
    
    // initialize material constants
    double rhor = spt.m_sbmr[m_lsbm];
    double ksi = FiberModulus(rhor);
    
    // deformation gradient
    mat3d &F = pt.m_F;
    double J = pt.m_J;
    
    // loop over all integration points
    vec3d n0, nt;
    double In_1, Wll;
    const double eps = 0;
    mat3ds C = pt.RightCauchyGreen();
    tens4ds c;
    
	// get the local coordinate systems
	mat3d Q = GetLocalCS(mp);

    // evaluate fiber direction in global coordinate system
    n0 = Q*m_n0;
    
    // Calculate In = n0*C*n0
    In_1 = n0*(C*n0) - 1.0;
    
    // only take fibers in tension into consideration
    if (In_1 >= eps)
    {
        // get the global spatial fiber direction in current configuration
        nt = F*n0;
        
        // calculate the outer product of nt
        mat3ds N = dyad(nt);
        tens4ds NxN = dyad1s(N);
        
        // calculate strain energy 2nd derivative
        double tmp = m_alpha*pow(In_1, m_beta);
        Wll = ksi*pow(In_1, m_beta-2.0)*((tmp+1)*m_beta-1.0)*exp(tmp);
        
        // calculate the fiber tangent
        c = NxN*(4.0*Wll/J);
    }
    else
    {
        c.zero();
    }
    
    return c;
}

//-----------------------------------------------------------------------------
double FEFiberExpPowSBM::StrainEnergyDensity(FEMaterialPoint& mp)
{
    double sed = 0.0;
    
    FEElasticMaterialPoint& pt = *mp.ExtractData<FEElasticMaterialPoint>();
    FESolutesMaterialPoint& spt = *mp.ExtractData<FESolutesMaterialPoint>();
    
    // initialize material constants
    double rhor = spt.m_sbmr[m_lsbm];
    double ksi = FiberModulus(rhor);
    
    // loop over all integration points
    vec3d n0;
    double In_1;
    const double eps = 0;
    mat3ds C = pt.RightCauchyGreen();
    
	// get the local coordinate systems
	mat3d Q = GetLocalCS(mp);

    // evaluate fiber direction in global coordinate system
    n0 = Q*m_n0;
    
    // Calculate In = n0*C*n0
    In_1 = n0*(C*n0) - 1.0;
    
    // only take fibers in tension into consideration
    if (In_1 >= eps)
    {
        // calculate strain energy derivative
        if (m_alpha > 0) {
            sed = ksi/(m_alpha*m_beta)*(exp(m_alpha*pow(In_1, m_beta))-1);
        }
        else
            sed = ksi/m_beta*pow(In_1, m_beta);
    }
    
    return sed;
}
