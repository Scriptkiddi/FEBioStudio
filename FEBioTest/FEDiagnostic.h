#pragma once
#include "FECore/FEModel.h"
#include "FEBioXML/FEBioImport.h"

class FEDiagnostic;

//-----------------------------------------------------------------------------
class FEDiagnosticScenario : public FEParamContainer
{
public:
	FEDiagnosticScenario(FEDiagnostic* pdia) : m_pdia(pdia) {};

	FEDiagnostic* GetDiagnostic() { return m_pdia; }

	virtual bool Init() { return true; }

private:
	FEDiagnostic* m_pdia;
};

//-----------------------------------------------------------------------------
//! The FEDiagnostic class is a base class that can be used to create
//! diagnostic classes to test FEBio's performance.

class FEDiagnostic
{
public:
	//! constructor
	FEDiagnostic(FEModel& fem);

	//! destructor
	virtual ~FEDiagnostic();

	//! initialization
	virtual bool Init() { return true; }

	//! run the diagnostic. Returns true on pass, false on failure
	virtual bool Run() = 0;

	//! load data from file
	virtual bool ParseSection(XMLTag& tag) { return false; }

	//! create a scenario class
	virtual FEDiagnosticScenario* CreateScenario(const std::string& sname) { return 0; }

	FEModel* GetFEModel() { return &m_fem; }

private:
	FEModel&	m_fem;	//!< the FEModel object the diagnostic is performed on
};

//-----------------------------------------------------------------------------
// Control Section
class FEDiagnosticControlSection : public FEFileSection
{
public:
	FEDiagnosticControlSection(FEFileImport* pim) : FEFileSection(pim) {}
	void Parse(XMLTag& tag);
};

//-----------------------------------------------------------------------------
// Scenario Section parser
class FEDiagnosticScenarioSection : public FEFileSection
{
public:
	FEDiagnosticScenarioSection(FEFileImport* pim) : FEFileSection(pim){}
	void Parse(XMLTag& tag);
};

//-----------------------------------------------------------------------------
//! The FEDiagnosticImport class creates a specific diagnostic test. Currently
//! the only way to create a diagnostic is to load a diagnostic from file

class FEDiagnosticImport : public FEFileImport
{
public:
	FEDiagnostic* LoadFile(FEModel& fem, const char* szfile);

protected:
	FEDiagnostic* m_pdia;

	friend class FEDiagnosticScenarioSection;
};
