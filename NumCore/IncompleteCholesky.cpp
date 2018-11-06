#include "stdafx.h"
#include "IncompleteCholesky.h"
#include <FECore/CompactSymmMatrix.h>

// We must undef PARDISO since it is defined as a function in mkl_solver.h
#ifdef MKL_ISS
#ifdef PARDISO
#undef PARDISO
#endif
#include "mkl_rci.h"
#include "mkl_blas.h"
#include "mkl_spblas.h"
#endif // MKL_ISS

IncompleteCholesky::IncompleteCholesky(FEModel* fem) : Preconditioner(fem)
{

}

CompactSymmMatrix* IncompleteCholesky::getMatrix()
{
	return m_L;
}

// create a preconditioner for a sparse matrix
bool IncompleteCholesky::Create(SparseMatrix* A)
{
	// make sure it's the correct format
	CompactSymmMatrix* K = dynamic_cast<CompactSymmMatrix*>(A);
	if (K == nullptr) return false;

	int N = K->Rows();
	int nnz = K->NonZeroes();

	z.resize(N, 0.0);

	// create the preconditioner
	m_L = new CompactSymmMatrix(K->Offset());
	double* val = new double[nnz];
	int* row = new int[nnz];
	int* col = new int[N + 1];
	m_L->alloc(N, N, nnz, val, row, col);

	// copy the matrix
	double* aval = K->Values();
	int* arow = K->Indices();
	int* acol = K->Pointers();
	for (int i = 0; i < nnz; ++i)
	{
		val[i] = aval[i];
		row[i] = arow[i];
	}
	for (int i = 0; i <= N; ++i) col[i] = acol[i];

	CompactSymmMatrix& L = *m_L;

	vector<double> tmp(N, 0.0);

	// fill in the values
	int offset = m_L->Offset();
	for (int k = 0; k < N; ++k)
	{
		// get the values for column k
		double* ak = val + (col[k] - offset);
		int* rowk = row + (col[k] - offset);
		int Lk = col[k + 1] - col[k];

		// make sure the diagonal element is not zero
		if (ak[0] <= 0.0) return false;

		// set the diagonal element
		double akk = sqrt(ak[0]);
		ak[0] = akk;
		tmp[rowk[0] - offset] = akk;

		// divide column by akk
		for (int j = 1; j < Lk; ++j)
		{
			ak[j] /= akk;
			tmp[rowk[j] - offset] = ak[j];
		}

		// loop over all other columns
		for (int _j = 1; _j < Lk; ++_j)
		{
			int j = rowk[_j] - offset;
			double tjk = tmp[j];
			if (tjk != 0.0)
			{
				double* aj = val + col[j] - offset;
				int Lj = col[j + 1] - col[j];
				int* rowj = row + col[j] - offset;

				for (int i = 0; i < Lj; i ++) aj[i] -= tmp[rowj[i] - offset] * tjk;
			}
		}

		// reset temp buffer
		for (int j = 0; j < Lk; ++j) tmp[rowk[j] - offset] = 0.0;
	}

	for (int i = 0; i < N; ++i)
	{
		double Lii = L.diag(i);
		assert(Lii != 0.0);
	}

	return true;
}

bool IncompleteCholesky::mult_vector(double* x, double* y)
{
	int ivar = m_L->Rows();
	double* pa = m_L->Values();
	int* ia = m_L->Pointers();
	int* ja = m_L->Indices();

	char cvar1 = 'U';
	char cvar = 'T';
	char cvar2 = 'N';
	mkl_dcsrtrsv(&cvar1, &cvar, &cvar2, &ivar, pa, ia, ja, &x[0], &z[0]);
	cvar1 = 'U';
	cvar = 'N';
	cvar2 = 'N';
	mkl_dcsrtrsv(&cvar1, &cvar, &cvar2, &ivar, pa, ia, ja, &z[0], &y[0]);

	return true;
}
