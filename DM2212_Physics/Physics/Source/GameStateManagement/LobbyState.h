#pragma once
#include "GameStateBase.h"
#include "../SceneManager.h"
#include "../Input.h"
#include "../SceneLobby.h"
#include "../UIManager.h"
#include <vector>

class CLobbyState : public CGameStateBase
{
public:
	SceneManager* sceneManager;
	SceneLobby* sceneLobby;
	UIManager* uiManager;

	// Constructor
	CLobbyState(void);
	// Destructor
	~CLobbyState(void);

	// Init this class instance
	virtual bool Init(void);
	// Update this class instance
	virtual bool Update(const double dElapsedTime);
	// Render this class instance
	virtual void Render(void);
	// Destroy this class instance
	virtual void Destroy(void);

	//Input
	Input* input;

};
