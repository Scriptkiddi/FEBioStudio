#pragma once
#include "FEDiagnostic.h"

//! Harwell-Boeing Matrix Print Diagnostic

//! Class to run a diagnostic to print the initial matrix in
//! Harwell-Boeing matrix format

class FEPrintHBMatrixDiagnostic :	public FEDiagnostic
{
public:
	FEPrintHBMatrixDiagnostic(FEModel& fem);
	~FEPrintHBMatrixDiagnostic(void);

	bool ParseSection(XMLTag& tag);

	bool Run();

};
