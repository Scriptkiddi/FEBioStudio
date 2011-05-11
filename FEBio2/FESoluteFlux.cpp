#include "stdafx.h"
#include "FESoluteFlux.h"
#include "FESolidSolver.h"

//-----------------------------------------------------------------------------
//! calculates the stiffness contribution due to solute flux
//!
void FESoluteFlux::FluxStiffness(FESurfaceElement& el, matrix& ke, vector<double>& wn)
{
	int i, j, n;
	
	int nint = el.GaussPoints();
	int neln = el.Nodes();
	
	// normal solute flux at integration point
	double wr;
	
	vec3d dxr, dxs;
	
	// gauss weights
	double* w = el.GaussWeights();
	
	// nodal coordinates
	vec3d* rt = el.rt();
	
	vec3d kab, t1, t2;
	
	ke.zero();
	
	double* N, *Gr, *Gs;
	
	// repeat over integration points
	for (n=0; n<nint; ++n)
	{
		N = el.H(n);
		Gr = el.Gr(n);
		Gs = el.Gs(n);
		
		// calculate velocities and covariant basis vectors at integration point
		wr = 0;
		dxr = dxs = vec3d(0,0,0);
		for (i=0; i<neln; ++i)
		{
			wr += N[i]*wn[i];
			dxr += rt[i]*Gr[i];
			dxs += rt[i]*Gs[i];
		}
		
		// calculate surface normal
		vec3d dxt = dxr ^ dxs;
		
		// calculate stiffness component
		for (i=0; i<neln; ++i)
			for (j=0; j<neln; ++j)
			{
				t1 = dxt/dxt.norm()*wr;
				t2 = dxs*Gr[j] - dxr*Gs[j];
				kab = (t1 ^ t2)*(N[i]*w[n]);
				
				ke[4*i+3][4*j  ] += kab.x;
				ke[4*i+3][4*j+1] += kab.y;
				ke[4*i+3][4*j+2] += kab.z;
			}
	}
}

//-----------------------------------------------------------------------------
//! calculates the equivalent nodal volumetric flow rates due to solute flux
//!
bool FESoluteFlux::FlowRate(FESurfaceElement& el, vector<double>& fe, vector<double>& wn)
{
	int i, n;
	
	// nr integration points
	int nint = el.GaussPoints();
	
	// nr of element nodes
	int neln = el.Nodes();
	
	// nodal coordinates
	vec3d *rt = el.rt();
	
	double* Gr, *Gs;
	double* N;
	double* w  = el.GaussWeights();
	
	// normal solute flux at integration points
	double wr;
	
	vec3d dxr, dxs, dxt;
	
	// volumetric flow rate
	double f;
	
	// repeat over integration points
	zero(fe);
	for (n=0; n<nint; ++n)
	{
		N  = el.H(n);
		Gr = el.Gr(n);
		Gs = el.Gs(n);
		
		wr = 0;
		dxr = dxs = vec3d(0,0,0);
		for (i=0; i<neln; ++i) 
		{
			wr += N[i]*wn[i];
			dxr += rt[i]*Gr[i];
			dxs += rt[i]*Gs[i];
		}
		dxt = dxr ^ dxs;
		
		f = dxt.norm()*wr*w[n];
		
		for (i=0; i<neln; ++i)
		{
			fe[i] += N[i]*f;
		}
	}
	
	return true;
}

//-----------------------------------------------------------------------------
//! calculates the equivalent nodal volumetric flow rates due to solute flux
//!
bool FESoluteFlux::LinearFlowRate(FESurfaceElement& el, vector<double>& fe, vector<double>& wn)
{
	int i, n;
	
	// nr integration points
	int nint = el.GaussPoints();
	
	// nr of element nodes
	int neln = el.Nodes();
	
	// nodal coordinates
	vec3d *r0 = el.r0();
	
	double* Gr, *Gs;
	double* N;
	double* w  = el.GaussWeights();
	
	// normal solute flux at integration points
	double wr;
	
	vec3d dxr, dxs, dxt, vs;
	
	// volumetric flow rate
	double f;
	
	// repeat over integration points
	zero(fe);
	for (n=0; n<nint; ++n)
	{
		N  = el.H(n);
		Gr = el.Gr(n);
		Gs = el.Gs(n);
		
		wr = 0;
		dxr = dxs = vec3d(0,0,0);
		for (i=0; i<neln; ++i) 
		{
			wr += N[i]*wn[i];
			dxr += r0[i]*Gr[i];
			dxs += r0[i]*Gs[i];
		}
		dxt = dxr ^ dxs;
		
		f = dxt.norm()*wr*w[n];
		
		for (i=0; i<neln; ++i)
		{
			fe[i] += N[i]*f;
		}
	}
	
	return true;
}

//-----------------------------------------------------------------------------
//!
void FESoluteFlux::Serialize(DumpFile& ar)
{
	if (ar.IsSaving())
	{
		// solute fluxes
		ar << m_blinear;
		ar << (int) m_PC.size();
		for (int i=0; i<(int) m_PC.size(); ++i)
		{
			LOAD& fc = m_PC[i];
			ar << fc.lc;
			ar << fc.s[0] << fc.s[1] << fc.s[2] << fc.s[3];
			ar << fc.bc;
		}
	}
	else
	{
		// solute fluxes
		int n;
		ar >> m_blinear;
		ar >> n;
		m_PC.resize(n);
		for (int i=0; i<(int) m_PC.size(); ++i)
		{
			LOAD& fc = m_PC[i];
			ar >> fc.lc;
			ar >> fc.s[0] >> fc.s[1] >> fc.s[2] >> fc.s[3];
			ar >> fc.bc;
		}
	}
}

//-----------------------------------------------------------------------------
void FESoluteFlux::StiffnessMatrix(FESolver* psolver)
{
	FESolidSolver& solver = dynamic_cast<FESolidSolver&>(*psolver);
	FEM& fem = solver.m_fem;
	
	matrix ke;
	
	int nfr = m_PC.size();
	for (int m=0; m<nfr; ++m)
	{
		LOAD& fc = m_PC[m];
		if (fc.bc == 0)
		{
			// get the surface element
			FESurfaceElement& el = m_psurf->Element(m);
			
			// skip rigid surface elements
			// TODO: do we really need to skip rigid elements?
			if (!el.IsRigid())
			{
				m_psurf->UnpackElement(el);
				
				// calculate nodal normal solute flux
				int neln = el.Nodes();
				vector<double> wn(neln);
				
				if (m_blinear == false)
				{
					double g = fem.GetLoadCurve(fc.lc)->Value();
					
					for (int j=0; j<neln; ++j) wn[j] = g*fc.s[j];
					
					// get the element stiffness matrix
					int ndof = neln*4;
					ke.Create(ndof, ndof);
					
					// calculate pressure stiffness
					FluxStiffness(el, ke, wn);
					
					// TODO: the problem here is that the LM array that is returned by the UnpackElement
					// function does not give the equation numbers in the right order. For this reason we
					// have to create a new lm array and place the equation numbers in the right order.
					// What we really ought to do is fix the UnpackElement function so that it returns
					// the LM vector in the right order for solute-solid elements.
					vector<int> lm(ndof);
					for (int i=0; i<neln; ++i)
					{
						lm[4*i  ] = el.LM()[3*i];
						lm[4*i+1] = el.LM()[3*i+1];
						lm[4*i+2] = el.LM()[3*i+2];
						lm[4*i+3] = el.LM()[11*neln+i];
					}
					
					// assemble element matrix in global stiffness matrix
					solver.AssembleStiffness(el.m_node, lm, ke);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void FESoluteFlux::Residual(FESolver* psolver, vector<double>& R)
{
	FESolidSolver& solver = dynamic_cast<FESolidSolver&>(*psolver);
	FEM& fem = solver.m_fem;
	
	vector<double> fe;
	
	int nfr = m_PC.size();
	for (int i=0; i<nfr; ++i)
	{
		LOAD& fc = m_PC[i];
		if (fc.bc == 0)
		{
			FESurfaceElement& el = m_psurf->Element(i);
			m_psurf->UnpackElement(el);
			
			// calculate nodal normal solute flux
			int neln = el.Nodes();
			vector<double> wn(neln);
			
			double g = fem.GetLoadCurve(fc.lc)->Value();
			
			for (int j=0; j<neln; ++j) wn[j] = g*fc.s[j];
			
			int ndof = neln;
			fe.resize(ndof);
			
			if (m_blinear) LinearFlowRate(el, fe, wn); else FlowRate(el, fe, wn);
			
			// TODO: the problem here is that the LM array that is returned by the UnpackElement
			// function does not give the equation numbers in the right order. For this reason we
			// have to create a new lm array and place the equation numbers in the right order.
			// What we really ought to do is fix the UnpackElement function so that it returns
			// the LM vector in the right order for solute-solid elements.
			vector<int> lm(ndof);
			for (int i=0; i<neln; ++i)
				lm[i] = el.LM()[11*neln+i];
			
			// add element force vector to global force vector
			solver.AssembleResidual(el.m_node, lm, fe, R);
		}
	}
}
