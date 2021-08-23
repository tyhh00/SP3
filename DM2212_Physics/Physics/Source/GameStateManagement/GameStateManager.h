#pragma once

#include "SingletonTemplate.h"
#include <map>
#include <string>

#include "GameStateBase.h"

//class CGameState;

class CGameStateManager : public CSingletonTemplate<CGameStateManager>
{
	friend CSingletonTemplate<CGameStateManager>;
public:
	// Destroy this CGameStateManager
	void Destroy();

	// System Interface
	// Update this class instance
	bool Update(const double dElapsedTime);
	// Render this class instance
	void Render(void);

	// User Interface
	bool AddGameState(const std::string& _name, CGameStateBase* _scene);
	bool RemoveGameState(const std::string& _name);
	bool SetActiveGameState(const std::string& _name);
	bool CheckGameStateExist(const std::string& _name);
	bool SetOverlayGameState(const std::string& _name);
	bool RemoveOverlayGameState();

protected:
	// Constructor
	CGameStateManager();
	// Destructor
	~CGameStateManager();


	bool overlayState;
	// The map of CGameStates
	std::map<std::string, CGameStateBase*> GameStateMap;
	// The handlers containing the active and next CGameState
	CGameStateBase*activeGameState, *nextGameState, *prevGameState;
	CGameStateBase* overlayGameState;
};
