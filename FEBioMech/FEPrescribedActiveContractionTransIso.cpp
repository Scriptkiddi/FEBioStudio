#include "stdafx.h"
#include "FEPrescribedActiveContractionTransIso.h"

//-----------------------------------------------------------------------------
// define the material parameters
BEGIN_FECORE_CLASS(FEPrescribedActiveContractionTransIso, FEElasticMaterial)
	ADD_PARAMETER(m_T0 , "T0"   );
	ADD_PARAMETER(m_thd, "theta");
	ADD_PARAMETER(m_phd, "phi"  );
END_FECORE_CLASS();

//-----------------------------------------------------------------------------
FEPrescribedActiveContractionTransIso::FEPrescribedActiveContractionTransIso(FEModel* pfem) : FEElasticMaterial(pfem)
{
    m_thd = 0;
    m_phd = 90;
}

//-----------------------------------------------------------------------------
bool FEPrescribedActiveContractionTransIso::Validate()
{
	if (FEElasticMaterial::Validate() == false) return false;

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
void FEPrescribedActiveContractionTransIso::Serialize(DumpStream& ar)
{
	FEElasticMaterial::Serialize(ar);
	if (ar.IsShallow()) return;
	ar & m_n0;
}

//-----------------------------------------------------------------------------
mat3ds FEPrescribedActiveContractionTransIso::Stress(FEMaterialPoint &mp)
{
    FEElasticMaterialPoint& pt = *mp.ExtractData<FEElasticMaterialPoint>();
    
    // deformation gradient
    mat3d &F = pt.m_F;
    double J = pt.m_J;
    mat3ds b = pt.LeftCauchyGreen();
    
	// get the local coordinate systems
	mat3d Q = GetLocalCS(mp);

    // evaluate fiber direction in global coordinate system
    vec3d n0 = Q*m_n0;
    
    // evaluate the deformed fiber direction
    vec3d nt = F*n0;
    mat3ds N = dyad(nt);
    
    // evaluate the active stress
    mat3ds s = (b-N)*(m_T0/J);
    
    return s;
}

//-----------------------------------------------------------------------------
tens4ds FEPrescribedActiveContractionTransIso::Tangent(FEMaterialPoint &mp)
{
    tens4ds c;
    c.zero();
    
    return c;
}
