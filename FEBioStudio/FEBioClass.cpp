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
#include "stdafx.h"
#include "FEBioClass.h"
#include <FECore/FECoreKernel.h>
#include <FECore/FECoreBase.h>
#include <FEBioLib/FEBioModel.h>
using namespace FEBio;

// dummy model used for allocating temporary FEBio classes.
FEBioModel febioModel;

std::vector<FEBio::FEBioClassInfo> FEBio::FindAllClasses(int mod, int superId)
{
	vector<FEBio::FEBioClassInfo> facs;

	FECoreKernel& fecore = FECoreKernel::GetInstance();

	for (int i = 0; i < fecore.FactoryClasses(); ++i)
	{
		const FECoreFactory* fac = fecore.GetFactoryClass(i);
		if (fac->GetSuperClassID() == superId)
		{
			FEBio::FEBioClassInfo febc = { fac->GetTypeStr(), i };
			facs.push_back(febc);
		}
	}

	return facs;
}

void FEBioClass::AddParameter(const std::string& paramName, int paramType, const QVariant& val)
{
	FEBioParam p;
	p.m_type = paramType;
	p.m_name = paramName;
	p.m_val  = val;
	m_Param.push_back(p);
}

QVariant vec3d_to_qvariant(const vec3d& v)
{
	QList<QVariant> val;
	val.push_back(v.x);
	val.push_back(v.y);
	val.push_back(v.z);
	return val;
}

QVariant mat3d_to_qvariant(const mat3d& m)
{
	QList<QVariant> val;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) val.push_back(m[i][j]);
	return val;
}

FEBioClass* FEBio::CreateFEBioClass(int classId)
{
	// Get the kernel
	FECoreKernel& fecore = FECoreKernel::GetInstance();

	// find the factory
	const FECoreFactory* fac = fecore.GetFactoryClass(classId); assert(fac);
	if (fac == nullptr) return nullptr;

	// try to create a temporary FEBio object
	unique_ptr<FECoreBase> pc(fac->Create(&febioModel)); assert(pc);
	if (pc == nullptr) return nullptr;

	const char* sztype = fac->GetTypeStr();

	// create the interface class
	FEBioClass* feb = new FEBioClass;
	feb->SetTypeString(sztype);

	// copy parameter info
	FEParameterList& pl = pc->GetParameterList();
	int params = pl.Parameters();
	FEParamIterator it = pl.first();
	for (int i = 0; i < params; ++i, ++it)
	{
		FEParam& p = *it;
		switch (p.type())
		{
		case FE_PARAM_INT   : feb->AddParameter(p.name(), p.type(), p.value<int>()); break;
		case FE_PARAM_BOOL  : feb->AddParameter(p.name(), p.type(), p.value<bool>()); break;
		case FE_PARAM_DOUBLE: feb->AddParameter(p.name(), p.type(), p.value<double>()); break;
		case FE_PARAM_VEC3D : feb->AddParameter(p.name(), p.type(), vec3d_to_qvariant(p.value<vec3d>())); break;
		case FE_PARAM_MAT3D : feb->AddParameter(p.name(), p.type(), mat3d_to_qvariant(p.value<mat3d>())); break;
		case FE_PARAM_DOUBLE_MAPPED: feb->AddParameter(p.name(), p.type(), 0.0); break;
		case FE_PARAM_VEC3D_MAPPED: 
		{
			vec3d v(0,0,0); // TODO: Grab const value from FEParamVec3d
			QVariant val = vec3d_to_qvariant(v);
			feb->AddParameter(p.name(), p.type(), val);
		}
		break;
		case FE_PARAM_MAT3D_MAPPED:
		{
			mat3d M; M.unit(); // TODO: Grab const value from FEParamMat3d
			QVariant val = mat3d_to_qvariant(M);
//			feb->AddParameter(p.name(), p.type(), val);
		}
		break;
		case FEBIO_PARAM_STD_VECTOR_INT:
		{
			// Don't know how to handle this.
		}
		break;
		default:
			assert(false);
		}
	}

	// all done!
	return feb;
}
