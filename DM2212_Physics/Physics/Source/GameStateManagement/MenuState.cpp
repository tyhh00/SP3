// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
//#include <includes/glm.hpp>
//#include <includes/gtc/matrix_transform.hpp>
//#include <includes/gtc/type_ptr.hpp>

#include "MenuState.h"

// Include CGameStateManager
#include "GameStateManager.h"

// Include Mesh Builder
#include "../MeshBuilder.h"
#include "../Debug.h"


#include "../SoundController/SoundController.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CMenuState::CMenuState(void)
{
	sceneManager = SceneManager::GetInstance();

	menuScene = new SceneMainMenu();

	// Input
	input = Input::GetInstance();
}

/**
 @brief Destructor
 */
CMenuState::~CMenuState(void)
{
	if (menuScene)
	{
		delete menuScene;
		menuScene = NULL;
	}
}

/**
 @brief Init this class instance
 */
bool CMenuState::Init(void)
{
	cout << "CMenuState::Init()\n" << endl;

	menuScene->Init();
	CSoundController::GetInstance()->PlaySoundByID(SOUND_TYPE::BG_MAINMENU);

	return true;
}

/**
 @brief Update this class instance
 */
bool CMenuState::Update(const double dElapsedTime)
{
	menuScene->Update(dElapsedTime);

	if (Application::IsKeyPressed(VK_SPACE))
	{
		//Setting state and switching the scene to lobby
		CGameStateManager::GetInstance()->SetActiveGameState("LobbyState");
		
		return true;
	}
	
	return true;
}

/**
 @brief Render this class instance
 */
void CMenuState::Render(void)
{
	// Clear the screen and buffer
	glClearColor(0.0f, 0.55f, 1.00f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	menuScene->Render();
	//cout << "CMenuState::Render()\n" << endl;
}

/**
 @brief Destroy this class instance
 */
void CMenuState::Destroy(void)
{
	cout << "CMenuState::Destroy()\n" << endl;
	menuScene->Exit();
}
