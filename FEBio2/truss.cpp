#include "stdafx.h"
#include "FESolidSolver.h"
#include "FETrussMaterial.h"
#include "FEElasticTrussDomain.h"

//-----------------------------------------------------------------------------
// FEElasticTrussDomain
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void FEElasticTrussDomain::Reset()
{
	for (int i=0; i<(int) m_Elem.size(); ++i) m_Elem[i].Init(true);
}

//-----------------------------------------------------------------------------
void FEElasticTrussDomain::UnpackElement(FEElement &el, unsigned int nflag)
{
	int i, n;

	vec3d* rt = el.rt();
	vec3d* r0 = el.r0();
	vec3d* vt = el.vt();
	double* pt = el.pt();

	int N = el.Nodes();
	vector<int>& lm = el.LM();

	for (i=0; i<N; ++i)
	{
		n = el.m_node[i];
		FENode& node = m_pMesh->Node(n);

		int* id = node.m_ID;

		// first the displacement dofs
		lm[3*i  ] = id[0];
		lm[3*i+1] = id[1];
		lm[3*i+2] = id[2];

		// now the pressure dofs
		lm[3*N+i] = id[6];

		// rigid rotational dofs
		lm[4*N + 3*i  ] = id[7];
		lm[4*N + 3*i+1] = id[8];
		lm[4*N + 3*i+2] = id[9];

		// fill the rest with -1
		lm[7*N + 3*i  ] = -1;
		lm[7*N + 3*i+1] = -1;
		lm[7*N + 3*i+2] = -1;

		lm[10*N + i] = id[10];
	}

	// copy nodal data to element arrays
	for (i=0; i<N; ++i)
	{
		n = el.m_node[i];

		FENode& node = m_pMesh->Node(n);

		// initial coordinates (= material coordinates)
		r0[i] = node.m_r0;

		// current coordinates (= spatial coordinates)
		rt[i] = node.m_rt;

		// current nodal pressures
		pt[i] = node.m_pt;

		// current nodal velocities
		vt[i] = node.m_vt;
	}

	// unpack the traits data
	el.UnpackTraitsData(nflag);
}

//-----------------------------------------------------------------------------
void FEElasticTrussDomain::InitElements()
{
	for (size_t i=0; i<m_Elem.size(); ++i)
	{
		FETrussElement& el = m_Elem[i];
		el.m_State[0]->Init(false);
	}
}

//-----------------------------------------------------------------------------

void FEElasticTrussDomain::StiffnessMatrix(FESolidSolver* psolver)
{
	FEM& fem = psolver->m_fem;
	matrix ke;
	int NT = m_Elem.size();
	for (int iel =0; iel<NT; ++iel)
	{
		FETrussElement& el = m_Elem[iel];
		UnpackElement(el);
		ElementStiffness(fem, el, ke);
		psolver->AssembleStiffness(el.m_node, el.LM(), ke);
	}
}

//-----------------------------------------------------------------------------
void FEElasticTrussDomain::ElementStiffness(FEM& fem, FETrussElement& el, matrix& ke)
{
	// get the material
	FETrussMaterial* pm = dynamic_cast<FETrussMaterial*>(fem.GetMaterial(el.GetMatID()));
	assert(pm);

	// intial length
	double L = el.Length0();

	// current length
	double l = el.Length();

	// get the elastic tangent
	FEMaterialPoint& mp = *el.m_State[0];
	FETrussMaterialPoint& pt = *mp.ExtractData<FETrussMaterialPoint>();
	double E = pm->Tangent(pt);

	// element initial volume
	double V = el.Volume0();

	// Kirchhoff Stress
	double tau = pt.m_tau;

	// scalar stiffness
	double k = V / (l*l)*( E - 2*tau);

	// axial force T = s*a = t*V/l
	double T = tau*V/l;

	// element normal
	vec3d n = el.Normal();

	// calculate the tangent matrix
	ke.Create(6, 6);

	ke[0][0] = ke[3][3] = k*n.x*n.x + T/l;
	ke[1][1] = ke[4][4] = k*n.y*n.y + T/l;
	ke[2][2] = ke[5][5] = k*n.z*n.z + T/l;

	ke[0][1] = ke[1][0] = ke[3][4] = ke[4][3] = k*n.x*n.y;
	ke[1][2] = ke[2][1] = ke[4][5] = ke[5][4] = k*n.y*n.z;
	ke[0][2] = ke[2][0] = ke[3][5] = ke[5][3] = k*n.x*n.z;

	ke[0][3] = ke[3][0] = -ke[0][0]; ke[0][4] = ke[4][0] = -ke[0][1]; ke[0][5] = ke[5][0] = -ke[0][2];
	ke[1][3] = ke[3][1] = -ke[1][0]; ke[1][4] = ke[4][1] = -ke[1][1]; ke[1][5] = ke[5][1] = -ke[1][2];
	ke[2][3] = ke[3][2] = -ke[2][0]; ke[2][4] = ke[4][2] = -ke[2][1]; ke[2][5] = ke[5][2] = -ke[2][2];
}

//-----------------------------------------------------------------------------

void FEElasticTrussDomain::Residual(FESolidSolver* psolver, vector<double>& R)
{
	// element force vector
	vector<double> fe;
	int NT = m_Elem.size();
	for (int i=0; i<NT; ++i)
	{
		FETrussElement& el = m_Elem[i];
		UnpackElement(el);
		InternalForces(el, fe);
		psolver->AssembleResidual(el.m_node, el.LM(), fe, R);
	}
}

//-----------------------------------------------------------------------------
void FEElasticTrussDomain::InternalForces(FETrussElement& el, vector<double>& fe)
{
	FEMaterialPoint& mp = *el.m_State[0];
	FETrussMaterialPoint& pt = *(mp.ExtractData<FETrussMaterialPoint>());

	// get the element's normal
	vec3d n = el.Normal();

	// get the element's Kirchhoff stress
	double tau = pt.m_tau;

	// elements initial volume
	double V = el.Volume0();

	// current length
	double l = el.Length();

	// calculate nodal forces
	fe.resize(6);
	fe[0] = tau*V/l*n.x;
	fe[1] = tau*V/l*n.y;
	fe[2] = tau*V/l*n.z;
	fe[3] = -fe[0];
	fe[4] = -fe[1];
	fe[5] = -fe[2];
}

//-----------------------------------------------------------------------------

void FEElasticTrussDomain::UpdateStresses(FEM &fem)
{
	for (int i=0; i<(int) m_Elem.size(); ++i)
	{
		// unpack the element
		FETrussElement& el = m_Elem[i];
		UnpackElement(el);

		// get the material
		FEMaterial* pmat = fem.GetMaterial(el.GetMatID());
		FETrussMaterial* pm = dynamic_cast<FETrussMaterial*>(pmat);

		// setup the material point
		FEMaterialPoint& mp = *(el.m_State[0]);
		FETrussMaterialPoint& pt = *(mp.ExtractData<FETrussMaterialPoint>());

		double l = el.Length();
		double L = el.Length0();
		pt.m_l = l / L;

		pt.m_tau = pm->Stress(pt);
	}
}
