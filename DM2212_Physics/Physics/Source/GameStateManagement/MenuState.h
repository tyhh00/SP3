#pragma once

/**
 CMenuState
 @brief This class is derived from CGameState. It will introduce the game to the player.
 By: Toh Da Jun
 Date: July 2021
 */

#include "GameStateBase.h"
#include <string>

 
#include "GameStateBase.h"
#include "../SceneManager.h"
#include "../Input.h"
#include "../SceneMainMenu.h"


class CMenuState : public CGameStateBase
{
public:
	SceneManager* sceneManager;

	// Constructor
	CMenuState(void);
	// Destructor
	~CMenuState(void);

	// Init this class instance
	virtual bool Init(void);
	// Update this class instance
	virtual bool Update(const double dElapsedTime);
	// Render this class instance
	virtual void Render(void);
	// Destroy this class instance
	virtual void Destroy(void);

	Input* input;

protected:

	SceneMainMenu* menuScene;
};
