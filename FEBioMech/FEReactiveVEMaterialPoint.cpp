/*This file is part of the FEBio source code and is licensed under the MIT license
listed below.

See Copyright-FEBio.txt for details.

Copyright (c) 2021 University of Utah, The Trustees of Columbia University in
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



#include "stdafx.h"
#include "FEReactiveVEMaterialPoint.h"
#include "FEElasticMaterial.h"

//-----------------------------------------------------------------------------
FEReactiveViscoelasticMaterialPoint::FEReactiveViscoelasticMaterialPoint() : FEMaterialPointArray(new FEElasticMaterialPoint)
{
}

//-----------------------------------------------------------------------------
FEMaterialPoint* FEReactiveViscoelasticMaterialPoint::Copy()
{
    FEReactiveViscoelasticMaterialPoint* pt = new FEReactiveViscoelasticMaterialPoint;
    pt->m_mp = m_mp;
    if (m_pNext) pt->m_pNext = m_pNext->Copy();
    return pt;
}

//-----------------------------------------------------------------------------
void FEReactiveViscoelasticMaterialPoint::Init()
{
    // don't forget to initialize the base class
    FEMaterialPointArray::Init();
}

//-----------------------------------------------------------------------------
void FEReactiveViscoelasticMaterialPoint::Serialize(DumpStream& ar)
{
    FEMaterialPointArray::Serialize(ar);
}


///////////////////////////////////////////////////////////////////////////////
//
// FEReactiveVEMaterialPoint
//
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
//! Create a shallow copy of the material point data
FEMaterialPoint* FEReactiveVEMaterialPoint::Copy()
{
    FEReactiveVEMaterialPoint* pt = new FEReactiveVEMaterialPoint(*this);
    if (m_pNext) pt->m_pNext = m_pNext->Copy();
    return pt;
}

//-----------------------------------------------------------------------------
//! Initializes material point data.
void FEReactiveVEMaterialPoint::Init()
{
	// initialize data to zero
	m_Uv.clear();
	m_Jv.clear();
	m_v.clear();
	m_f.clear();
    
    // don't forget to initialize the base class
    FEMaterialPoint::Init();
}

//-----------------------------------------------------------------------------
//! Serialize data to the archive
void FEReactiveVEMaterialPoint::Serialize(DumpStream& ar)
{
    FEMaterialPoint::Serialize(ar);
    
    if (ar.IsSaving())
    {
        int n = (int)m_Uv.size();
        ar << n;
        for (int i=0; i<n; ++i) ar << m_Uv[i] << m_Jv[i] << m_v[i] << m_f[i];
    }
    else
    {
        int n;
        ar >> n;
		m_Uv.resize(n);
		m_Jv.resize(n);
		m_v.resize(n);
		m_f.resize(n);
        for (int i=0; i<n; ++i) ar >> m_Uv[i] >> m_Jv[i] >> m_v[i] >> m_f[i];
    }
}
