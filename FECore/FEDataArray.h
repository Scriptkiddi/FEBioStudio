#pragma once
#include <vector>
#include <assert.h>
#include <string>
#include "vec3d.h"
#include "vec2d.h"
#include "mat3d.h"
#include "fecore_api.h"
#include "fecore_enum.h"
#include "fecore_type.h"

//-----------------------------------------------------------------------------
class DumpStream;

//-----------------------------------------------------------------------------
class FECORE_API FEDataArray
{
public:
	//! default constructor
	FEDataArray(FEDataMapType mapType, FEDataType dataType);

	virtual ~FEDataArray();

public:
	virtual void setValue(int n, double v) = 0;
	virtual void setValue(int n, const vec2d& v) = 0;
	virtual void setValue(int n, const vec3d& v) = 0;
	virtual void setValue(int n, const mat3d& v) = 0;

	virtual void fillValue(double v) = 0;
	virtual void fillValue(const vec2d& v) = 0;
	virtual void fillValue(const vec3d& v) = 0;
	virtual void fillValue(const mat3d& v) = 0;

public:
	//! get the value for a given facet index
	template <class T> T get(int n) const;

	//! set the value
	template <class T> bool set(const T& v);

	//! set the value
	template <class T> bool set(int n, const T& v);

	//! add a value
	template <class T> void push_back(const T& v);

	//! allocate data
	bool resize(int nsize, double val = 0.0);

	//! set the data sized
	void SetDataSize(int dataSize);

public:
	//! get the data type
	FEDataType DataType() const { return m_dataType; }

	//! get the map type
	FEDataMapType DataMapType() const { return m_mapType; }

	//! get data size
	int DataSize() const { return m_dataSize; }

	// number of data items
	int DataCount() const { return m_dataCount; }

	//! return the buffer size (actual number of doubles)
	int BufferSize() const { return (int) m_val.size(); }

public:
	//! serialization
	virtual void Serialize(DumpStream& ar);

	static void SaveClass(DumpStream& ar, FEDataArray* p);
	static FEDataArray* LoadClass(DumpStream& ar, FEDataArray* p);

protected:
	//! copy constructor
	FEDataArray(const FEDataArray& map);

	//! assignment operator
	FEDataArray& operator = (const FEDataArray& map);

private:
	FEDataMapType	m_mapType;	//!< the map type
	FEDataType		m_dataType;	//!< the data type
	int	m_dataSize;				//!< size of each data item
	int	m_dataCount;			//!< number of data items

	std::vector<double>	m_val;	//!< data values
};

template <> inline double FEDataArray::get<double>(int n) const
{
	assert(m_dataSize == fecoreType<double>::size());
	return	m_val[n];
}

template <> inline vec2d FEDataArray::get<vec2d>(int n) const
{
	assert(m_dataSize == fecoreType<vec2d>::size());
	return	vec2d(m_val[2*n], m_val[2*n+1]);
}

template <> inline vec3d FEDataArray::get<vec3d>(int n) const
{
	assert(m_dataSize == fecoreType<vec3d>::size());
	return	vec3d(m_val[3*n], m_val[3*n + 1], m_val[3*n+2]);
}

template <> inline mat3d FEDataArray::get<mat3d>(int n) const
{
	assert(m_dataSize == fecoreType<mat3d>::size());
	const double* v = &(m_val[9*n]);
	return	mat3d(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8]);
}

template <> inline void FEDataArray::push_back<double>(const double& v)
{
	assert(m_dataSize == fecoreType<double>::size());
	m_val.push_back(v);
	m_dataCount++;
}

template <> inline void FEDataArray::push_back<vec2d>(const vec2d& v)
{
	assert(m_dataSize == fecoreType<vec2d>::size());
	m_val.push_back(v.x());
	m_val.push_back(v.y());
	m_dataCount++;
}

template <> inline void FEDataArray::push_back<vec3d>(const vec3d& v)
{
	assert(m_dataSize == fecoreType<vec3d>::size());
	m_val.push_back(v.x);
	m_val.push_back(v.y);
	m_val.push_back(v.z);
	m_dataCount++;
}

template <> inline void FEDataArray::push_back<mat3d>(const mat3d& v)
{
	assert(m_dataSize == fecoreType<vec3d>::size());
	m_val.push_back(v[0][0]); m_val.push_back(v[0][1]); m_val.push_back(v[0][2]);
	m_val.push_back(v[1][0]); m_val.push_back(v[1][1]); m_val.push_back(v[1][2]);
	m_val.push_back(v[2][0]); m_val.push_back(v[2][1]); m_val.push_back(v[2][2]);
	m_dataCount++;
}

//-----------------------------------------------------------------------------
template <> inline bool FEDataArray::set<double>(int n, const double& v)
{
	assert(m_dataSize == fecoreType<double>::size());
	m_val[n] = v;
	return true;
}

//-----------------------------------------------------------------------------
template <> inline bool FEDataArray::set<vec2d>(int n, const vec2d& v)
{
	assert(m_dataSize == fecoreType<vec2d>::size());
	m_val[2 * n] = v.x();
	m_val[2 * n + 1] = v.y();
	return true;
}

//-----------------------------------------------------------------------------
template <> inline bool FEDataArray::set<vec3d>(int n, const vec3d& v)
{
	assert(m_dataSize == fecoreType<vec3d>::size());
	m_val[3 * n] = v.x;
	m_val[3 * n + 1] = v.y;
	m_val[3 * n + 2] = v.z;
	return true;
}

//-----------------------------------------------------------------------------
template <> inline bool FEDataArray::set<mat3d>(int n, const mat3d& v)
{
	assert(m_dataSize == fecoreType<mat3d>::size());
	double* d = &(m_val[9 * n]);
	d[0] = v[0][0]; d[1] = v[0][1]; d[2] = v[0][2];
	d[3] = v[1][0]; d[4] = v[1][1]; d[5] = v[1][2];
	d[6] = v[2][0]; d[7] = v[2][1]; d[8] = v[2][2];
	return true;
}

//-----------------------------------------------------------------------------
template <> inline bool FEDataArray::set<double>(const double& v)
{
	assert(m_dataSize == fecoreType<double>::size());
	for (int i = 0; i<(int)m_val.size(); ++i) m_val[i] = v;
	return true;
}

//-----------------------------------------------------------------------------
template <> inline bool FEDataArray::set<vec2d>(const vec2d& v)
{
	assert(m_dataSize == fecoreType<vec2d>::size());
	for (int i = 0; i<(int)m_val.size(); i += 2)
	{
		m_val[i] = v.x();
		m_val[i + 1] = v.y();
	}
	return true;
}

//-----------------------------------------------------------------------------
template <> inline bool FEDataArray::set<vec3d>(const vec3d& v)
{
	assert(m_dataSize == fecoreType<vec3d>::size());
	for (int i = 0; i<(int)m_val.size(); i += 3)
	{
		m_val[i] = v.x;
		m_val[i + 1] = v.y;
		m_val[i + 2] = v.z;
	}
	return true;
}

//-----------------------------------------------------------------------------
template <> inline bool FEDataArray::set<mat3d>(const mat3d& v)
{
	assert(m_dataSize == fecoreType<mat3d>::size());
	for (int i = 0; i<(int)m_val.size(); i += 9)
	{
		double* d = &m_val[i];
		d[0] = v[0][0]; d[1] = v[0][1]; d[2] = v[0][2];
		d[3] = v[1][0]; d[4] = v[1][1]; d[5] = v[1][2];
		d[6] = v[2][0]; d[7] = v[2][1]; d[8] = v[2][2];
	}
	return true;
}
