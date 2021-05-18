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

#pragma once

#include <MeshLib/FENode.h>
#include <MeshLib/FEElement.h>
#include <MeshLib/FEMesh.h>
#include "FEGroup.h"
#include <MeshLib/FENodeElementList.h>
#include <MeshLib/FENodeFaceList.h>
#include <FSCore/box.h>
#include <utility>
#include <vector>
//using namespace std;

namespace Post {

//-----------------------------------------------------------------------------
class FEPostMesh : public FEMesh
{
public:
	// --- M E M O R Y   M A N A G M E N T ---
	//! constructor
	FEPostMesh();

	//! destructor
	virtual ~FEPostMesh();

	//! allocate storage for mesh
	void Create(int nodes, int elems, int faces = 0, int edges = 0) override;

	//! Clean up all data
	void CleanUp();

	//! clean mesh and all data
	void ClearAll();

	const vector<NodeElemRef>& NodeElemList(int n) const { return m_NEL.ElementList(n); }
	const vector<NodeFaceRef>& NodeFaceList(int n) const { return m_NFL.FaceList(n); }

public:
	// --- G E O M E T R Y ---

	//! return domains
	int Domains() const { return (int) m_Dom.size(); }

	//! return a domain
	FEDomain& Domain(int i) { return *m_Dom[i]; }

	//! nr of parts
	int Parts() const { return (int) m_Part.size(); }

	//! add a part
	void AddPart(FEPart* pg) { m_Part.push_back(pg); }

	//! return a part
	FEPart& Part(int n) { return *m_Part[n]; }

	// number of surfaces
	int Surfaces() const { return (int) m_Surf.size(); }

	// return a surface
	FESurface& Surface(int n) { return *m_Surf[n]; }

	// Add a surface
	void AddSurface(FESurface* ps) { m_Surf.push_back(ps); }

	//! number of node sets
	int NodeSets() const { return (int) m_NSet.size(); }

	//! return a node set
	FENodeSet& NodeSet(int i) { return *m_NSet[i]; }

	//! Add a node set
	void AddNodeSet(FENodeSet* ps) { m_NSet.push_back(ps); }

	// --- D A T A   U P D A T E ---

	//! update mesh data
	void BuildMesh() override;

protected:
	void UpdateDomains();

	void ClearDomains();
	void ClearParts();
	void ClearSurfaces();
	void ClearNodeSets();

protected:
	// --- G E O M E T R Y ---
	vector<FEDomain*>	m_Dom;	// domains

	// user-defined partitions
	vector<FEPart*>		m_Part;	// parts
	vector<FESurface*>	m_Surf;	// surfaces
	vector<FENodeSet*>	m_NSet;	// node sets

	FENodeElementList	m_NEL;
	FENodeFaceList		m_NFL;
};

// find the element and the iso-parametric coordinates of a point inside the mesh
// the x coordinates is assumed to be in reference frame
bool FindElementInReferenceFrame(FECoreMesh& m, const vec3f& x, int& nelem, double r[3]);

class FEState;

double IntegrateNodes(Post::FEPostMesh& mesh, Post::FEState* ps);
double IntegrateEdges(Post::FEPostMesh& mesh, Post::FEState* ps);

// This function calculates the integral over a surface. Note that if the surface
// is triangular, then we calculate the integral from a degenerate quad.
double IntegrateFaces(Post::FEPostMesh& mesh, Post::FEState* ps);

// integrates the surface normal scaled by the data field
vec3d IntegrateSurfaceNormal(Post::FEPostMesh& mesh, Post::FEState* ps);

// This function calculates the integral over a volume. Note that if the volume
// is not hexahedral, then we calculate the integral from a degenerate hex.
double IntegrateElems(Post::FEPostMesh& mesh, Post::FEState* ps);

} // namespace Post
