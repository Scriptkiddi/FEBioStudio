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
#include "FEMeshData.h"
#include <typeinfo>
#include <string>

using std::string;

//-----------------------------------------------------------------------------
namespace Post {

// forward declarations
class FEPostModel;

//-----------------------------------------------------------------------------
// data field flags
enum DataFieldFlags {
	EXPORT_DATA = 1			// data field can be exported
};

//-----------------------------------------------------------------------------
// Base class describing a data field
class FEDataField
{
public:
	FEDataField(FEPostModel* glm, Data_Type ntype, Data_Format nfmt, Data_Class ncls, unsigned int flag);

	virtual ~FEDataField();

	//! get the name of the field
	const std::string& GetName() const;

	//! set the name of the field
	void SetName(const std::string& newName);

	//! Create a copy
	virtual FEDataField* Clone() const = 0;

	//! FEMeshData constructor
	virtual FEMeshData* CreateData(FEState* pstate) = 0;

	//! type identifier
	Data_Type Type() { return m_ntype; }

	// Format identifier
	Data_Format Format() const { return m_nfmt; }

	// Class Identifier
	Data_Class DataClass() const { return m_nclass; }

	//! Set the field ID
	void SetFieldID(int nid) { m_nfield = nid; }

	//! get the field ID
	int GetFieldID() const { return m_nfield; }

	//! type string
	const char* TypeStr() const;

	//! number of components
	int components(Data_Tensor_Type ntype);

	//! number of actual data components
	int dataComponents(Data_Tensor_Type ntype);

	//! return the name of a component
	std::string componentName(int ncomp, Data_Tensor_Type ntype);

	virtual const std::type_info& TypeInfo() { return typeid(FEDataField); }

	unsigned int Flags() const { return m_flag; }

	void SetArraySize(int n) { m_arraySize = n; }
	int GetArraySize() const { return m_arraySize; }

	void SetArrayNames(vector<string>& n);
	vector<string> GetArrayNames() const;

	FEPostModel* GetModel() { return m_fem; }

protected:
	int				m_nfield;	//!< field ID
	Data_Type		m_ntype;	//!< data type
	Data_Format		m_nfmt;		//!< data format
	Data_Class		m_nclass;	//!< data class
	unsigned int	m_flag;		//!< flags
	std::string		m_name;		//!< data field name

	int				m_arraySize;	//!< data size for arrays
	vector<string>	m_arrayNames;	//!< (optional) names of array components

	FEPostModel*	m_fem;

public:
	// TODO: Add properties list for data fields (e.g. strains and curvature could use this)
	// strain parameters
	int		m_nref;	// reference state
};

//-----------------------------------------------------------------------------
template<typename T> class FEDataField_T : public FEDataField
{
public:
	FEDataField_T(FEPostModel* fem, unsigned int flag = 0) : FEDataField(fem, T::Type(), T::Format(), T::Class(), flag) {}
	FEMeshData* CreateData(FEState* pstate) { return new T(pstate, this); }

	virtual FEDataField* Clone() const
	{
		FEDataField_T<T>* newData = new FEDataField_T<T>(m_fem);
		newData->SetName(GetName());
		return newData;
	}

	const std::type_info& TypeInfo() { return typeid(T); }
};

//-----------------------------------------------------------------------------
typedef vector<FEDataField*>::iterator FEDataFieldPtr;


//-----------------------------------------------------------------------------
class FEArrayDataField : public FEDataField
{
public:
	FEArrayDataField(FEPostModel* fem, Data_Class c, Data_Format f, unsigned int flag = 0);

	FEDataField* Clone() const override;

	FEMeshData* CreateData(FEState* pstate) override;
};

//-----------------------------------------------------------------------------
class FEArrayVec3DataField : public FEDataField
{
public:
	FEArrayVec3DataField(FEPostModel* fem, Data_Class c, unsigned int flag = 0);

	FEDataField* Clone() const override;

	FEMeshData* CreateData(FEState* pstate) override;
};

//-------------------------------------------------------------------------------
bool ExportDataField(FEPostModel& fem, const FEDataField& df, const char* szfile);
bool ExportNodeDataField(FEPostModel& fem, const FEDataField& df, FILE* fp);
bool ExportFaceDataField(FEPostModel& fem, const FEDataField& df, FILE* fp);
bool ExportElementDataField(FEPostModel& fem, const FEDataField& df, FILE* fp);

//-----------------------------------------------------------------------------
void InitStandardDataFields();
int StandardDataFields(); 
std::string GetStandarDataFieldName(int i);
bool AddStandardDataField(FEPostModel& fem, const std::string& dataField);
bool AddStandardDataField(FEPostModel& fem, const std::string& dataField, std::vector<int> selectionList);

//-----------------------------------------------------------------------------
bool AddNodeDataFromFile(FEPostModel& fem, const char* szfile, const char* szname, int ntype);
bool AddFaceDataFromFile(FEPostModel& fem, const char* szfile, const char* szname, int ntype);
bool AddElemDataFromFile(FEPostModel& fem, const char* szfile, const char* szname, int ntype);

class FEPlotObjectData : public FEDataField
{
public:
	FEPlotObjectData(FEPostModel* fem, Data_Type ntype) : FEDataField(fem, ntype, DATA_ITEM, CLASS_OBJECT, 0) {}

	FEDataField* Clone() const override { assert(false); return nullptr; };
	FEMeshData* CreateData(FEState* pstate) override { assert(false); return nullptr; }
};
}
