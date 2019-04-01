#pragma once
#include "FEElasticMaterial.h"
#include "FEDamageMaterialPoint.h"
#include "FEDamageCriterion.h"
#include "FEDamageCDF.h"

//-----------------------------------------------------------------------------
// This material models damage in any hyper-elastic materials.

class FEDamageMaterial : public FEElasticMaterial
{
public:
	FEDamageMaterial(FEModel* pfem);
    
public:
	//! calculate stress at material point
	mat3ds Stress(FEMaterialPoint& pt) override;
    
	//! calculate tangent stiffness at material point
	tens4ds Tangent(FEMaterialPoint& pt) override;
    
	//! calculate strain energy density at material point
	double StrainEnergyDensity(FEMaterialPoint& pt) override;
    
    //! damage
    double Damage(FEMaterialPoint& pt);
    
	//! data initialization and checking
	bool Init() override;
    
	// returns a pointer to a new material point object
	FEMaterialPoint* CreateMaterialPointData() override
	{
		return new FEDamageMaterialPoint(m_pBase->CreateMaterialPointData());
	}
    
    // get the elastic material
    FEElasticMaterial* GetElasticMaterial() { return m_pBase; }
    
public:
    FEElasticMaterial*  m_pBase;    // base elastic material
	FEDamageCDF*        m_pDamg;    // damage model
	FEDamageCriterion*  m_pCrit;    // damage criterion

	DECLARE_FECORE_CLASS();
};
