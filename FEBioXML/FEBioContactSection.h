#pragma once
#include "FEBioImport.h"
#include "FECore/FESurfacePairInteraction.h"

class FEFacetSet;

//-----------------------------------------------------------------------------
// Contact section (new in version 2.0)
class FEBioContactSection : public FEBioFileSection
{
public:
	FEBioContactSection(FEBioImport* pim) : FEBioFileSection(pim){}
	void Parse(XMLTag& tag);

protected:
	void ParseRigidWall            (XMLTag& tag);
	void ParseRigidWall25          (XMLTag& tag);
	void ParseRigidSliding         (XMLTag& tag); // new in 2.5
	void ParseRigidInterface       (XMLTag& tag);
	void ParseLinearConstraint     (XMLTag& tag);

protected:
	void ParseContactInterface(XMLTag& tag, FESurfacePairInteraction* pci);
	void ParseContactInterface25(XMLTag& tag, FESurfacePairInteraction* pci);
	bool ParseSurfaceSection  (XMLTag& tag, FESurface& s, int nfmt, bool bnodal);

protected:
	bool BuildSurface(FESurface& s, FEFacetSet& f, bool bnodal);
};
