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
	//soundController = CSoundController::GetInstance();
	//soundController->Init();
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
	soundController = CSoundController::GetInstance();
	//soundController->Init();
	// Load the sounds into CSoundController
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
	soundController->LoadSound("Sounds\\Sound_Light_Bullet_Shoot.wav", SOUND_TYPE::LIGHTBULLET_SHOOT, true);
	soundController->LoadSound("Sounds\\Sound_Light_Bullet_Collided.wav", SOUND_TYPE::LIGHTBULLET_COLLIDED, true);
	soundController->LoadSound("Sounds\\Sound_Dash.wav", SOUND_TYPE::DASH, true);
	soundController->LoadSound("Sounds\\Sound_Grappler.wav", SOUND_TYPE::GRAPPLER, true);
	soundController->LoadSound("Sounds\\Sound_Campfire.wav", SOUND_TYPE::CAMPFIRE, true);
	soundController->LoadSound("Sounds\\Sound_MonkeyBanana.wav", SOUND_TYPE::MONKEY_BANANA, true);
	soundController->LoadSound("Sounds\\Sound_ProwlerAttack.wav", SOUND_TYPE::PROWLER_ATTACK, true);
	soundController->LoadSound("Sounds\\Sound_ProwlerFootsteps.wav", SOUND_TYPE::PROWLER_FOOTSTEPS, true);
	soundController->LoadSound("Sounds\\Sound_SpawnMinion.wav", SOUND_TYPE::PROWLER_SPAWN_MINION, true);
	soundController->LoadSound("Sounds\\Sound_Eating.wav", SOUND_TYPE::EATING, true);
	soundController->LoadSound("Sounds\\Sound_slowTime.wav", SOUND_TYPE::SLOWTIME, true);
	soundController->LoadSound("Sounds\\Sound_Thunder.wav", SOUND_TYPE::THUNDER, true);
	soundController->LoadSound("Sounds\\Sound_CrabAttack.wav", SOUND_TYPE::CRABATTACK, true);
	soundController->LoadSound("Sounds\\Sound_Sand.wav", SOUND_TYPE::CRABWALK, true);
	soundController->LoadSound("Sounds\\Sound_Trident.wav", SOUND_TYPE::TRIDENT, true);
	soundController->LoadSound("Sounds\\Sound_Coin.wav", SOUND_TYPE::COIN, true);
	soundController->LoadSound("Sounds\\Sound_Jump.wav", SOUND_TYPE::JUMP, true);
	//Loopables
	soundController->LoadSound("Sounds\\Sound_GameBG1.wav", SOUND_TYPE::BG_ARCADE, true, true);
	soundController->LoadSound("Sounds\\Sound_GameBG2.wav", SOUND_TYPE::BG_ARCADE2, true, true);
	soundController->LoadSound("Sounds\\Sound_MainMenu.wav", SOUND_TYPE::BG_MAINMENU, true, true);
	soundController->LoadSound("Sounds\\Sound_GraveyardBGM.wav", SOUND_TYPE::BG_GRAVEYARD, true, true);
	soundController->LoadSound("Sounds\\Sound_ChurchBG.wav", SOUND_TYPE::BG_CHURCH, true, true);
	soundController->LoadSound("Sounds\\Sound_OceanBG.wav", SOUND_TYPE::BG_OCEAN, true, true);
	soundController->LoadSound("Sounds\\Sound_JungleBG.wav", SOUND_TYPE::BG_JUNGLE, true, true);
	soundController->LoadSound("Sounds\\Sound_LobbyBG.wav", SOUND_TYPE::BG_LOBBY, true, true);


	sceneSplash = new SceneSplashScreen();
	sceneSplash->Init();
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

		CGameStateManager::GetInstance()->SetActiveGameState("MenuState");

		return true;
	}

	sceneSplash->Update(dElapsedTime);

	return true;
}

/**
 @brief Render this class instance
 */
void CIntroState::Render()
{
	// Clear the screen and buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.00f);
	sceneSplash->Render();
}

/**
 @brief Destroy this class instance
 */
void CIntroState::Destroy(void)
{
	cout << "CIntroState::Destroy()\n" << endl;

	if (sceneSplash)
	{
		sceneSplash->Exit();
		delete sceneSplash;
		sceneSplash = NULL;
	}
}