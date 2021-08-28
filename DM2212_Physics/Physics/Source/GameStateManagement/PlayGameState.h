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
#include "../UIManager.h"
#include "../Buttons/DialogueManager.h"
#include "../GameManager.h"
#include "../SoundController/SoundController.h"

class CPlayGameState : public CGameStateBase
{
public:
	SceneManager* sceneManager;
	UIManager* uiManager;

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

	enum GAME_STATE
	{
		DEFAULT,
		PAUSED,
		GAMEOVER,
		LEVELWON,

		GS_TOTAL
	};

	float m_screenWidth, m_screenHeight;

	GAME_STATE currentState;
	Mesh* resumeButtonMesh, *lobbyButtonMesh, *retryButtonMesh, *optionsButtonMesh, *backButtonMesh;
	Mesh* menuBG;
	Mesh* volumeBar[6];
	Mesh* leftArrowMesh, *rightArrowMesh;

	Button* buttonHighlight;
	std::vector<Button*> pauseButtonsList;
	std::vector<Button*> optionsButtonsList;

	ButtonManager* buttonManager;
	DialogueManager* dialogueManager;
	GameManager* gameManager;
	CSoundController* soundController;

	void UpdateVolumeBar();
};
