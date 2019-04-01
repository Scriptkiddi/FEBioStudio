#pragma once
#include "FECore/FEMaterial.h"

//-----------------------------------------------------------------------------
// Virtual base class for damage criterion

class FEDamageCriterion : public FEMaterial
{
public:
	FEDamageCriterion(FEModel* pfem) : FEMaterial(pfem) {}
    
	//! damage
	virtual double DamageCriterion(FEMaterialPoint& pt) = 0;
    
};

//-----------------------------------------------------------------------------
// Simo's damage criterion

class FEDamageCriterionSimo : public FEDamageCriterion
{
public:
	FEDamageCriterionSimo(FEModel* pfem) : FEDamageCriterion(pfem) {}
    
	//! damage
	double DamageCriterion(FEMaterialPoint& pt);
};

//-----------------------------------------------------------------------------
// Strain energy density as damage criterion

class FEDamageCriterionSED : public FEDamageCriterion
{
public:
	FEDamageCriterionSED(FEModel* pfem) : FEDamageCriterion(pfem) {}
    
	//! damage
	double DamageCriterion(FEMaterialPoint& pt);
};

//-----------------------------------------------------------------------------
// Specific strain energy as damage criterion

class FEDamageCriterionSSE : public FEDamageCriterion
{
public:
    FEDamageCriterionSSE(FEModel* pfem) : FEDamageCriterion(pfem) {}
    
    //! damage
    double DamageCriterion(FEMaterialPoint& pt);
};

//-----------------------------------------------------------------------------
// von Mises stress as damage criterion

class FEDamageCriterionVMS : public FEDamageCriterion
{
public:
	FEDamageCriterionVMS(FEModel* pfem) : FEDamageCriterion(pfem) {}
    
	//! damage
	double DamageCriterion(FEMaterialPoint& pt);
};

//-----------------------------------------------------------------------------
// max shear stress as damage criterion

class FEDamageCriterionMSS : public FEDamageCriterion
{
public:
	FEDamageCriterionMSS(FEModel* pfem) : FEDamageCriterion(pfem) {}
    
	//! damage
	double DamageCriterion(FEMaterialPoint& pt);
};

//-----------------------------------------------------------------------------
// max normal stress as damage criterion

class FEDamageCriterionMNS : public FEDamageCriterion
{
public:
	FEDamageCriterionMNS(FEModel* pfem) : FEDamageCriterion(pfem) {}
    
	//! damage
	double DamageCriterion(FEMaterialPoint& pt);
};

//-----------------------------------------------------------------------------
// max normal Lagrange strain as damage criterion

class FEDamageCriterionMNLS : public FEDamageCriterion
{
public:
	FEDamageCriterionMNLS(FEModel* pfem) : FEDamageCriterion(pfem) {}
    
	//! damage
	double DamageCriterion(FEMaterialPoint& pt);
};
