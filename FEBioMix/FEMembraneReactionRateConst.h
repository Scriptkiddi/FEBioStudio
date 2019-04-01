#pragma once
#include "FEMultiphasic.h"

class FEBIOMIX_API FEMembraneReactionRateConst : public FEMembraneReactionRate
{
public:
    //! constructor
    FEMembraneReactionRateConst(FEModel* pfem) : FEMembraneReactionRate(pfem) { m_k = 0; }
    
    //! reaction rate at material point
    double ReactionRate(FEMaterialPoint& pt) override { return m_k; }
    
    //! tangent of reaction rate with strain at material point
    double Tangent_ReactionRate_Strain(FEMaterialPoint& pt) override { return 0; }
    
    //! tangent of reaction rate with effective fluid pressure at material point
    double Tangent_ReactionRate_Pressure(FEMaterialPoint& pt) override {return 0; }
    double Tangent_ReactionRate_Pe(FEMaterialPoint& pt) override { return 0; }
    double Tangent_ReactionRate_Pi(FEMaterialPoint& pt) override { return 0; }

    //! tangent of reaction rate with effective solute concentration at material point
    double Tangent_ReactionRate_Concentration(FEMaterialPoint& pt, const int isol) override {return 0; }
    double Tangent_ReactionRate_Ce(FEMaterialPoint& pt, const int isol) override { return 0; }
    double Tangent_ReactionRate_Ci(FEMaterialPoint& pt, const int isol) override { return 0; };
    
public:
    double    m_k;        //!< reaction rate
    
    DECLARE_FECORE_CLASS();
};
