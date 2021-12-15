#pragma once
#include <FSCore/FSObject.h>


//-----------------------------------------------------------------------------
class FSCoreBase;

//-----------------------------------------------------------------------------
//! Class describing a material property
//! A material property is a component of a parent material and is represented by a (list of ) material(s) itself
class FSProperty
{
public:
	enum { NO_FIXED_SIZE = 0 };

	enum Flags {
		EDITABLE = 0x01,			// the property can be edited in the material editor
		NON_EXTENDABLE = 0x02,		// cannot be modified after created in material editor
		REQUIRED = 0x04				// property is required
	};

public:
	FSProperty();
	FSProperty(const std::string& name, int nClassID, FSCoreBase* parent, int nsize = 1, unsigned int flags = EDITABLE);
	~FSProperty();

	// clears the material list for this property
	void Clear();

	// set the name of the property
	void SetName(const std::string& name);

	// return the name of the property
	const std::string& GetName();

	// append a component to the component list of this property
	void AddComponent(FSCoreBase* pm);

	// set the component in the property's material list
	void SetComponent(FSCoreBase* pm, int i = 0);

	// remove a component from the list (returns false if pm is not part of the list)
	bool RemoveComponent(FSCoreBase* pm);

	// return a component from the property's material list
	FSCoreBase* GetComponent(int i = 0);

	// return the index of a component (or -1)
	int GetComponentIndex(FSCoreBase* mat);

	// return the class ID of this property
	int GetClassID() const { return m_nClassID; }

	// return the size of the component list
	int Size() const { return (int)m_cmp.size(); }

	// return the max size for this property
	int maxSize() const { return m_maxSize; }

	// return property flags
	unsigned int GetFlags() const { return m_flag; }

	void SetFlags(unsigned int flags) { m_flag = flags; }

	void SetSuperClassID(int superClassID) { m_nsuperClassID = superClassID; }
	int GetSuperClassID() const { return m_nsuperClassID; }

	const std::string& GetDefaultType() { return m_defaultType; }
	void SetDefaultType(const std::string& s) { m_defaultType = s; }

	bool IsRequired() const;

private:
	std::string			m_name;			// name of this property
	std::string			m_defaultType;	// default type string, when type attributed is ommitted.
	int					m_nClassID;		// the class ID for this property
	int					m_nsuperClassID;// super class ID
	int					m_maxSize;		// max number of properties (0 for no limit)
	unsigned int		m_flag;			// property flags
	FSCoreBase*			m_parent;		// parent object this class is a property off
	vector<FSCoreBase*>	m_cmp;			// list of materials
};

//-----------------------------------------------------------------------------
// Base class for components of an FSModel
// TODO: Should I rename this to something else, like FSAbstractModelComponent or something.
class FSCoreBase : public FSObject
{
public:
	FSCoreBase();

public:
	// get number of properties
	size_t Properties() const;

	// delete all properties
	void ClearProperties();

	// get a property
	FSProperty& GetProperty(int i);

	// find a property by name
	FSProperty* FindProperty(const std::string& name);

	// find the property by type
	FSProperty* FindProperty(int ntype);

	// find the property by the class
	FSProperty* FindProperty(FSCoreBase* pm);

	// add a property to the material
	FSProperty* AddProperty(const std::string& name, int nClassID, int maxSize = 1, unsigned int flags = FSProperty::EDITABLE);

	// add a component to property with index propID
	int AddProperty(int propID, FSCoreBase* pm);

	// add a material to the property with name
	void AddProperty(const std::string& name, FSCoreBase* pm);

	// replace a property of the class
	void ReplaceProperty(int propID, FSCoreBase* pmat, int matID = 0);

private:
	vector<FSProperty*>	m_prop;		//!< list of properties
};
