#include "stdafx.h"
#include "FEOptimize.h"
#include "FEOptimizer.h"
#include "FECore/FECoreKernel.h"
#include "FECore/log.h"

//-----------------------------------------------------------------------------
FEOptimize::FEOptimize(FEModel* pfem) : FECoreTask(pfem), m_opt(*m_pfem)
{

}

//-----------------------------------------------------------------------------
bool FEOptimize::Init(const char* szfile)
{
	// read the data from the xml input file
	if (m_opt.Input(szfile) == false) return false;

	// do initialization
	felog.SetMode(Logfile::LOG_NEVER);
	if (m_opt.Init() == false) return false;

	felog.SetMode(Logfile::LOG_FILE_AND_SCREEN);
	felog.printbox("P A R A M E T E R   O P T I M I Z A T I O N   M O D U L E", "version 0.1");

	return true;
}

//-----------------------------------------------------------------------------
bool FEOptimize::Run()
{
	// solve the problem
	bool bret = m_opt.Solve();

	if (bret)
		felog.printf(" N O R M A L   T E R M I N A T I O N\n\n");
	else 
		felog.printf(" E R R O R   T E R M I N A T I O N\n\n");

	return bret;
}
