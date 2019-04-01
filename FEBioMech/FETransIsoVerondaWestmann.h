#pragma once
#include "FEUncoupledMaterial.h"
#include "FEUncoupledFiberExpLinear.h"
#include "FEActiveFiberContraction.h"
#include <FECore/FEModelParam.h>

//-----------------------------------------------------------------------------
//! Transversely Isotropic Veronda-Westmann material

//! This material has an isotopric Veronda-Westmann basis and single preferred
//! fiber direction.

class FETransIsoVerondaWestmann : public FEUncoupledMaterial
{
public:
	FETransIsoVerondaWestmann(FEModel* pfem);

public:
	double	m_c1;	//!< Veronda-Westmann coefficient C1
	double	m_c2;	//!< Veronda-Westmann coefficient C2

	FEParamVec3		m_fiber;	//!< local material fiber

public:
	//! calculate deviatoric stress at material point
	virtual mat3ds DevStress(FEMaterialPoint& pt) override;

	//! calculate deviatoric tangent stiffness at material point
	virtual tens4ds DevTangent(FEMaterialPoint& pt) override;

	//! calculate deviatoric strain energy density at material point
	virtual double DevStrainEnergyDensity(FEMaterialPoint& pt) override;

protected:
	FEUncoupledFiberExpLinear	m_fib;
	FEActiveFiberContraction*	m_ac;

	// declare parameter list
	DECLARE_FECORE_CLASS();
};
