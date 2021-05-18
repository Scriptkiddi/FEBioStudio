/*This file is part of the FEBio Studio source code and is licensed under the MIT license
listed below.

See Copyright-FEBio-Studio.txt for details.

Copyright (c) 2020 University of Utah, The Trustees of Columbia University in 
the City of New York, and others.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

// LoadCurve.cpp: implementation of the CLoadCurve class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LoadCurve.h"
#include "BSpline.h"
#include <algorithm>

using std::min;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FELoadCurve::FELoadCurve()
{
	// add two points
	LOADPOINT pt0(0,0), pt1(1,1);
	m_Pt.resize(2);
	m_Pt[0] = pt0;
	m_Pt[1] = pt1;

	m_ntag = 0;

	m_nID = -1;

	m_ref = 0.0;

	m_szname[0] = 0;

	m_ntype = LC_SMOOTH;
	m_nextend = EXT_CONSTANT;
    
    m_spline = nullptr;
}

FELoadCurve::FELoadCurve(double r)
{
	m_ntag = 0;
	m_nID = -1;
	m_ref = r;
	m_szname[0] = 0;
	m_ntype = LC_SMOOTH;
	m_nextend = EXT_CONSTANT;
    m_spline = nullptr;
}

FELoadCurve::FELoadCurve(const FELoadCurve& lc)
{
	m_Pt.resize(lc.Size());
	for (int i=0; i< lc.Size(); ++i) m_Pt[i] = lc[i];

	m_ntag = lc.m_ntag;
	strcpy(m_szname, lc.m_szname);
	m_ntype   = lc.m_ntype;
	m_nextend = lc.m_nextend;
	m_ref = lc.m_ref;
    m_spline = lc.m_spline;
}

FELoadCurve::~FELoadCurve()
{

}

FELoadCurve& FELoadCurve::operator =(const FELoadCurve& lc)
{
	m_Pt.resize(lc.Size());
	for (int i=0; i< lc.Size(); ++i) m_Pt[i] = lc[i];

	m_ntag = lc.m_ntag;
	strcpy(m_szname, lc.m_szname);
	m_ntype   = lc.m_ntype;
	m_nextend = lc.m_nextend;
	m_ref = lc.m_ref;
    m_spline = lc.m_spline;

	return (*this);
}

void FELoadCurve::Add(double x, double y)
{
	LOADPOINT pt = { x, y };
	Add(pt);
}

int FELoadCurve::Add(const LOADPOINT& pt)
{
	int n = 0;
	vector<LOADPOINT>::iterator it = m_Pt.begin();
	// find where to insert this point
	while ((it != m_Pt.end()) && (it->time < pt.time)) { it++; n++; }

	// if the new point coincides with the current point, just replace the value
	if (it != m_Pt.end())
	{
		if (fabs(it->time - pt.time) < 1e-12)
		{
			it->load = pt.load;
			return n;
		}
	}

	// insert the point
	m_Pt.insert(it, pt);

	return n;
}

void FELoadCurve::SetName(const char* sz)
{ 
	strcpy(m_szname, sz); 
}

void FELoadCurve::Delete(int n)
{
	if ((n>=0) && (n<Size()) && (Size()> 2)) 
	{
		vector<LOADPOINT>::iterator it = m_Pt.begin();
		for (int i=0; i<n; ++i) ++it;
		m_Pt.erase(it);
	}
}

void FELoadCurve::Delete(const vector<int>& indexList)
{
	vector<int> tmp;
	int N = indexList.size();
	for (int i = 0; i < N; ++i)
	{
		int n = indexList[i];
		if ((n >= 0) && (n < Size())) tmp.push_back(n);
	}

	std::sort(tmp.begin(), tmp.end());

	for (int i = 0; i < N; ++i)
	{
		int n = tmp[i];
		m_Pt.erase(m_Pt.begin() + n);
		for (int j = i + 1; j < N; ++j) tmp[j]--;
	}
}

inline double lerp(double t, double t0, double f0, double t1, double f1)
{
	return f0 + (f1 - f0)*(t - t0)/(t1 - t0);
}

inline double qerp(double t, double t0, double f0, double t1, double f1, double t2, double f2)
{
	double q0 = ((t2 - t )*(t1 - t ))/((t2 - t0)*(t1 - t0));
	double q1 = ((t2 - t )*(t  - t0))/((t2 - t1)*(t1 - t0));
	double q2 = ((t  - t1)*(t  - t0))/((t2 - t1)*(t2 - t0));

	return f0*q0 + f1*q1 + f2*q2;
}

double FELoadCurve::Value(double time)
{
	int nsize = Size();
	if (nsize == 0) return m_ref;	
	if (nsize == 1) return Val(0);

	int N = nsize-1;

	if (time == m_Pt[0].time) return Val(0);
	if (time == m_Pt[N].time) return Val(N);

	if (time < m_Pt[0].time) return ExtendValue(time);
	if (time > m_Pt[N].time) return ExtendValue(time);

	if (m_ntype == LC_LINEAR)
	{
		int n = 0;
		while (m_Pt[n].time <= time) ++n;
	
		double t0 = m_Pt[n-1].time;
		double t1 = m_Pt[n  ].time;

		double f0 = Val(n-1);
		double f1 = Val(n);

		return lerp(time, t0, f0, t1, f1);
	}
	else if (m_ntype == LC_STEP)
	{
		int n=0;
		while (m_Pt[n].time < time) ++n;

		return Val(n);
	}
	else if (m_ntype == LC_SMOOTH)
	{
		if (nsize == 2)
		{
			double t0 = m_Pt[0].time;
			double t1 = m_Pt[1].time;

			double f0 = Val(0);
			double f1 = Val(1);

			return lerp(time, t0, f0, t1, f1);
		}
		else if (nsize == 3)
		{
			double t0 = m_Pt[0].time;
			double t1 = m_Pt[1].time;
			double t2 = m_Pt[2].time;

			double f0 = Val(0);
			double f1 = Val(1);
			double f2 = Val(2);

			return qerp(time, t0, f0, t1, f1, t2, f2);
		}
		else
		{
			int n = 0;
			while (m_Pt[n].time <= time) ++n;

			if (n == 1)
			{
				double t0 = m_Pt[0].time;
				double t1 = m_Pt[1].time;
				double t2 = m_Pt[2].time;

				double f0 = Val(0);
				double f1 = Val(1);
				double f2 = Val(2);

				return qerp(time, t0, f0, t1, f1, t2, f2);
			}
			else if (n == nsize-1)
			{
				double t0 = m_Pt[n-2].time;
				double t1 = m_Pt[n-1].time;
				double t2 = m_Pt[n  ].time;

				double f0 = Val(n-2);
				double f1 = Val(n-1);
				double f2 = Val(n);

				return qerp(time, t0, f0, t1, f1, t2, f2);
			}
			else
			{
				double t0 = m_Pt[n-2].time;
				double t1 = m_Pt[n-1].time;
				double t2 = m_Pt[n  ].time;
				double t3 = m_Pt[n+1].time;

				double f0 = Val(n-2);
				double f1 = Val(n-1);
				double f2 = Val(n);
				double f3 = Val(n+1);

				double q1 = qerp(time, t0, f0, t1, f1, t2, f2);
				double q2 = qerp(time, t1, f1, t2, f2, t3, f3);

				return lerp(time, t1, q1, t2, q2);
			}
		}
	}
    else if ((m_ntype == LC_CSPLINE) || (m_ntype == LC_CPOINTS) || (m_ntype == LC_APPROX))
    {
        if (m_spline) return m_spline->eval(time);
        else return 0;
    }

	return 0;
}

double FELoadCurve::ExtendValue(double t)
{
	int nsize = (int)m_Pt.size();
	int N = nsize - 1;

	if (nsize == 0) return m_ref;
	if (nsize == 1) return Val(0);

	double Dt = (m_Pt[N].time - m_Pt[0].time);
	double dt = 0.001*Dt;
	if (dt == 0) return Val(0);

	switch (m_nextend)
	{
	case EXT_CONSTANT:
		if (t < m_Pt[0].time) return Val(0);
		if (t > m_Pt[N].time) return Val(N);
		break;
	case EXT_EXTRAPOLATE:
		switch (m_ntype)
		{
		case LC_STEP:
			{
				if (t < m_Pt[0].time) return Val(0);
				if (t > m_Pt[N].time) return Val(N);
			}
			break;
		case LC_LINEAR:
			{
				if (t < m_Pt[0].time) return lerp(t, m_Pt[0].time, Val(0), m_Pt[1].time, Val(1));
				else return lerp(t, m_Pt[N-1].time, Val(N-1), m_Pt[N].time, Val(N));
			}
			break;
		case LC_SMOOTH:
        case LC_CSPLINE:
        case LC_CPOINTS:
        case LC_APPROX:
			{
				if (t < m_Pt[0].time) return lerp(t, m_Pt[0].time, Val(0), m_Pt[0].time + dt, Value(m_Pt[0].time+dt));
				else return lerp(t, m_Pt[N].time - dt, Value(m_Pt[N].time - dt), m_Pt[N].time, Val(N));
			}
			return 0;
		}
		break;
	case EXT_REPEAT:
		{
			if (t < m_Pt[0].time) while (t < m_Pt[0].time) t += Dt;
			else while (t > m_Pt[N].time) t -= Dt;
			return Value(t);
		}
		break;
	case EXT_REPEAT_OFFSET:
		{
			int n = 0;
			if (t < m_Pt[0].time) while (t < m_Pt[0].time) { t += Dt; --n; }
			else while (t > m_Pt[N].time) { t -= Dt; ++n; }
			double off = n*(Val(N) - Val(0));

			return Value(t)+off;
		}
		break;
	}

	return 0;
}

void FELoadCurve::Load(IArchive& ar)
{
	TRACE("FELoadCurve::Load");

	int n, N;
	bool bact;

	n = -1;

	m_Pt.clear();

	while (IArchive::IO_OK == ar.OpenChunk())
	{
		int nid = ar.GetChunkID();
		switch (nid)
		{
		case CID_LC_ACTIVE: 
			if (ar.Version() < 0x00010003)
			{
				// The active flag is obsolote, but we still
				// read it for backward compatibility
				ar.read(bact); 
				m_ntag = (bact?1:0);
			}
			else throw ReadError("error parsing CID_LC_ACTIVE (FELoadCurve::Load)");
			break;
		case CID_LC_REF   : ar.read(m_ref); break;
		case CID_LC_FLAGS : ar.read(m_ntag); break;
		case CID_LC_TYPE  : ar.read(m_ntype); break;
		case CID_LC_EXTEND: ar.read(m_nextend); break;
		case CID_LC_POINTS: ar.read(N); n = 0; m_Pt.resize(N); break;
		case CID_LC_POINT:
			{
				assert(n >= 0);
				LOADPOINT& p = m_Pt[n];
				while (IArchive::IO_OK == ar.OpenChunk())
				{
					int nid = ar.GetChunkID();
					switch (nid)
					{
					case CID_LC_TIME: ar.read(p.time); break;
					case CID_LC_LOAD: ar.read(p.load); break;
					}
					ar.CloseChunk();
				}
				++n;
				assert(n <= N);
			}
			break;
		}
		ar.CloseChunk();
	}
    
    Update();
}

void FELoadCurve::Save(OArchive& ar)
{
	int n, N;
	N = Size();
	
	ar.WriteChunk(CID_LC_FLAGS , m_ntag);
	ar.WriteChunk(CID_LC_TYPE  , m_ntype);
	ar.WriteChunk(CID_LC_EXTEND, m_nextend);
	ar.WriteChunk(CID_LC_REF   , m_ref);
	ar.WriteChunk(CID_LC_POINTS, N);

	for (n=0; n<N; ++n)
	{
		LOADPOINT& pt = m_Pt[n];
		ar.BeginChunk(CID_LC_POINT);
		{
			ar.WriteChunk(CID_LC_TIME, pt.time);
			ar.WriteChunk(CID_LC_LOAD, pt.load);
		}
		ar.EndChunk();
	}
}

///////////////////////////////////////////////////////////////////////////////
// FUNCTION: FELoadCurve::Scale
//  Scales the loads of the load curve.
//

void FELoadCurve::Scale(double s)
{
	for (int i=0; i<Size(); ++i) m_Pt[i].load *= s;
    Update();
}

///////////////////////////////////////////////////////////////////////////////

bool FELoadCurve::LoadData(const char* szfile)
{
	FILE* fp = fopen(szfile, "rt");
	if (fp == 0) return false;

	FELoadCurve lc;
	lc.Clear();

	char szline[256];
	fgets(szline, 255, fp);

	int n;
	LOADPOINT pt;
	while (!feof(fp))
	{
		n = sscanf(szline, "%lg%lg", &pt.time, &pt.load);
		if (n != 2) { fclose(fp); return false; }
		lc.Add(pt);
		fgets(szline, 255, fp);
	}

	fclose(fp);

	// copy data
	n = lc.Size();
	m_Pt.resize(n);
	for (int i=0; i<n; ++i) m_Pt[i] = lc[i];
    Update();

	return true;
}

bool FELoadCurve::WriteData(const char* szfile)
{
	FILE* fp = fopen(szfile, "wt");
	if (fp == 0) return false;

	for (int i = 0; i < m_Pt.size(); ++i)
	{
		LOADPOINT& pt = m_Pt[i];
		fprintf(fp, "%lg %lg\n", pt.time, pt.load);
	}
	fclose(fp);
	return true;
}

///////////////////////////////////////////////////////////////////////////////
void FELoadCurve::Update()
{
    // check if using spline
    bool valid = true;
    int N = (int)m_Pt.size();
    if (m_ntype == LC_CSPLINE) {
        int korder = min(N,4);
        if (m_spline) delete m_spline;
        m_spline = new BSpline();
        valid = m_spline->init_interpolation(korder, m_Pt);
    }
    else if (m_ntype == LC_CPOINTS) {
        int korder = min(N,4);
        if (m_spline) delete m_spline;
        m_spline = new BSpline();
        valid = m_spline->init(korder, m_Pt);
    }
    else if (m_ntype == LC_APPROX) {
        int korder = min(N/2+1,4);
        if (m_spline) delete m_spline;
        m_spline = new BSpline();
        valid = m_spline->init_approximation(korder, N/2+1, m_Pt);
    }
    if (!valid) m_spline = nullptr;
}
