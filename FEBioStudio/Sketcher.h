#pragma once
#include <MeshTools/GSketch.h>
#include <FSCore/Object.h>

//-----------------------------------------------------------------------------
//! Helper class for creating sketches. This singleton manages the sketch that
//! the user is currently working on
class GSketcher
{
public:
	static GSketcher* GetInstance();

public:
	GSketch& GetCurrentSketch();
	CObject* FinalizeSketch();

private:
	GSketcher(){}

private:
	static GSketcher*	m_pThis;
	GSketch	m_sketch;	// current sketch
};
