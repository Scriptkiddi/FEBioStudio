/*This file is part of the FEBio source code and is licensed under the MIT license
listed below.

See Copyright-FEBio.txt for details.

Copyright (c) 2019 University of Utah, Columbia University, and others.

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
#include "FEDataGenerator.h"
#include "FEMesh.h"
#include "FENodeDataMap.h"
#include "FESurfaceMap.h"
#include "FEDomainMap.h"
#include "FEElementSet.h"
#include "log.h"

REGISTER_SUPER_CLASS(FEDataGenerator, FEDATAGENERATOR_ID);

FEDataGenerator::FEDataGenerator(FEModel* fem) : FECoreBase(fem)
{
}

FEDataGenerator::~FEDataGenerator()
{
}

bool FEDataGenerator::Init()
{
	return true;
}

// generate the data array for the given node set
bool FEDataGenerator::Generate(FENodeDataMap& map, const FENodeSet& set)
{
	int N = set.Size();
	map.Create(N);
	vector<double> p(3, 0.0);

	FEDataType dataType = map.DataType();
	for (int i = 0; i<N; ++i)
	{
		const FENode* ni = set.Node(i);
		vec3d ri = ni->m_r0;
		switch (dataType)
		{
		case FE_DOUBLE: { double d; value(ri, d); map.setValue(i, d); } break;
		case FE_VEC2D : { vec2d  d; value(ri, d); map.setValue(i, d); } break;
		case FE_VEC3D : { vec3d  d; value(ri, d); map.setValue(i, d); } break;
		case FE_MAT3D : { mat3d  d; value(ri, d); map.setValue(i, d); } break;
		}
	}

	return true;
}

// generate the data array for the given facet set
bool FEDataGenerator::Generate(FESurfaceMap& map, const FEFacetSet& surf)
{
	const FEMesh& mesh = *surf.GetMesh();

	FEDataType dataType = map.DataType();

	int N = surf.Faces();
	map.Create(&surf);
	for (int i = 0; i<N; ++i)
	{
		const FEFacetSet::FACET& face = surf.Face(i);

		int nf = face.ntype;
		for (int j=0; j<nf; ++j)
		{
			vec3d ri = mesh.Node(face.node[j]).m_r0;
			switch (dataType)
			{
			case FE_DOUBLE: { double d; value(ri, d); map.setValue(i, j, d); } break;
			case FE_VEC2D : { vec2d  d; value(ri, d); map.setValue(i, j, d); } break;
			case FE_VEC3D : { vec3d  d; value(ri, d); map.setValue(i, j, d); } break;
			case FE_MAT3D : { mat3d  d; value(ri, d); map.setValue(i, j, d); } break;
			}
		}
	}
	return true;
}

// generate the data array for the given element set
bool FEDataGenerator::Generate(FEDomainMap& map, FEElementSet& set)
{
	FEMesh& mesh = *set.GetMesh();

	FEDataType dataType = map.DataType();
	int N = set.Elements();
	map.Create(&set);
	for (int i = 0; i<N; ++i)
	{
		FEElement& el = *mesh.FindElementFromID(set[i]);

		switch (map.StorageFormat())
		{
		case FMT_MULT:
		{
			int ne = el.Nodes();
			for (int j = 0; j < ne; ++j)
			{
				vec3d ri = mesh.Node(el.m_node[j]).m_r0;
				switch (dataType)
				{
				case FE_DOUBLE: { double d; value(ri, d); map.setValue(i, j, d); } break;
				case FE_VEC2D: { vec2d  d; value(ri, d); map.setValue(i, j, d); } break;
				case FE_VEC3D: { vec3d  d; value(ri, d); map.setValue(i, j, d); } break;
				case FE_MAT3D: { mat3d  d; value(ri, d); map.setValue(i, j, d); } break;
				}
			}
		}
		break;
		case FMT_ITEM:
		{
			// calculate element center
			vec3d r(0, 0, 0);
			int ne = el.Nodes();
			for (int j = 0; j < ne; ++j) r += mesh.Node(el.m_node[j]).m_r0;
			r /= ne;

			// evaluate
			switch (dataType)
			{
			case FE_DOUBLE: { double d; value(r, d); map.setValue(i, d); } break;
			case FE_VEC2D: { vec2d  d; value(r, d); map.setValue(i, d); } break;
			case FE_VEC3D: { vec3d  d; value(r, d); map.setValue(i, d); } break;
			case FE_MAT3D: { mat3d  d; value(r, d); map.setValue(i, d); } break;
			}
		}
		break;
		};
	}
	return true;
}
