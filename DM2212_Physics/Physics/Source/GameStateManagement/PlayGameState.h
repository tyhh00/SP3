#pragma once

/**
 CPlayGameState
 @brief This class is derived from CGameState. It will introduce the game to the player.
 By: Toh Da Jun
 Date: July 2021
 */

#include "GameStateBase.h"
#include "../SceneManager.h"
#include "../Buttons/ButtonManager.h"

class CPlayGameState : public CGameStateBase
{
public:
	SceneManager* sceneManager;

	// Constructor
	CPlayGameState(void);
	// Destructor
	~CPlayGameState(void);

	// Init this class instance
	virtual bool Init(void);
	// Update this class instance
	virtual bool Update(const double dElapsedTime);
	// Render this class instance
	virtual void Render(void);
	// Destroy this class instance
	virtual void Destroy(void);

protected:

	float m_screenWidth, m_screenHeight;

	bool paused;
	Mesh* resumeButtonMesh, *lobbyButtonMesh;
	Mesh* menuBG;
	ButtonManager* buttonManager;

};
