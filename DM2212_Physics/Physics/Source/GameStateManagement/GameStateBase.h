#pragma once

#include "../Application.h"

class CGameStateBase
{
public:
	// Constructor
	CGameStateBase(void) {}
	// Destructor
	virtual ~CGameStateBase(void) {}

	// These are pure virtual functions
	virtual bool Init() = 0;
	virtual bool Update(const double dElapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Destroy() = 0;
};
