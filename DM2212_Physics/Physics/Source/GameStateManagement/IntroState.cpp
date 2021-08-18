// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLFW
#include <GLFW/glfw3.h>

#include "IntroState.h"

// Include CGameStateManager
#include "GameStateManager.h"

#include "../filesystem.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CIntroState::CIntroState(void)
{

}

/**
 @brief Destructor
 */
CIntroState::~CIntroState(void)
{
}

/**
 @brief Init this class instance
 */
bool CIntroState::Init(void)
{
	cout << "CIntroState::Init()\n" << endl;

	// Load the sounds into CSoundController
	soundController = CSoundController::GetInstance();
	soundController->Init();
	soundController->LoadSound("Sounds\\Sound_BombExplosion.wav", SOUND_TYPE::BOMB_EXPLOSION, true);
	soundController->LoadSound("Sounds\\Sound_DirtLand.wav", SOUND_TYPE::LANDED_GRASS, true);
	soundController->LoadSound("Sounds\\Sound_DirtWalk.ogg", SOUND_TYPE::WALKING_GRASS, true);
	soundController->LoadSound("Sounds\\Sound_GameOver.wav", SOUND_TYPE::GAME_OVER_LOSE, true);
	soundController->LoadSound("Sounds\\Sound_GameWin.wav", SOUND_TYPE::GAME_OVER_WIN, true);
	soundController->LoadSound("Sounds\\Sound_ItemPickup.wav", SOUND_TYPE::ITEM_PICKUP, true);
	soundController->LoadSound("Sounds\\Sound_LevelRotation.wav", SOUND_TYPE::LEVEL_ROTATION, true);
	soundController->LoadSound("Sounds\\Sound_Warp.wav", SOUND_TYPE::WARP, true);
	soundController->LoadSound("Sounds\\Sound_Selector.wav", SOUND_TYPE::SELECTOR, true);
	soundController->LoadSound("Sounds\\Sound_Sizzle.wav", SOUND_TYPE::ACID_SIZZLE, true);
	//Loopables
	soundController->LoadSound("Sounds\\Sound_GameBG1.wav", SOUND_TYPE::BG_ARCADE, true, true);
	soundController->LoadSound("Sounds\\Sound_GameBG2.wav", SOUND_TYPE::BG_ARCADE2, true, true);
	soundController->LoadSound("Sounds\\Sound_MainMenu.wav", SOUND_TYPE::BG_MAINMENU, true, true);

	CSoundController::GetInstance()->PlaySoundByID(SOUND_TYPE::BG_MAINMENU);

	return true;
}

/**
 @brief Update this class instance
 */
bool CIntroState::Update(const double dElapsedTime)
{
	cout << "CIntroState::Update()\n" << endl;

	if (Application::IsKeyPressed(VK_SPACE))
	{
		// Reset the CKeyboardController
		cout << "space key pressed" << endl;
		// Load the menu state
		cout << "Loading PlayGameState" << endl;

		//Fading effect
		CSoundController::GetInstance()->StopPlayingSoundByID(SOUND_TYPE::BG_MAINMENU, 0.5, 0.1);

		CGameStateManager::GetInstance()->SetActiveGameState("MenuState");

		return true;
	}

	return true;
}

/**
 @brief Render this class instance
 */
void CIntroState::Render()
{
	// Clear the screen and buffer
	glClearColor(0.0f, 0.55f, 1.00f, 1.00f);
}

/**
 @brief Destroy this class instance
 */
void CIntroState::Destroy(void)
{
	cout << "CIntroState::Destroy()\n" << endl;
}