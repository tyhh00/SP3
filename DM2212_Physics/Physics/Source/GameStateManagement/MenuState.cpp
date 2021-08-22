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
// Include ImageLoader
//#include "System\ImageLoader.h"
// Include Shader Manager
//#include "RenderControl\ShaderManager.h"

 // Include shader
//#include "RenderControl\shader.h"

// Include CSettings
//#include "GameControl/Settings.h"

// Include CKeyboardController
//#include "Inputs/KeyboardController.h"
//#include "../Application.h"

#include "../SoundController/SoundController.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CMenuState::CMenuState(void)
	//: background(NULL)
{

}

/**
 @brief Destructor
 */
CMenuState::~CMenuState(void)
{

}

/**
 @brief Init this class instance
 */
bool CMenuState::Init(void)
{
	cout << "CMenuState::Init()\n" << endl;

	sceneManager = SceneManager::GetInstance();
	sceneManager->init();
	
	// Input
	input = Input::GetInstance();

	CSoundController::GetInstance()->PlaySoundByID(SOUND_TYPE::BG_MAINMENU);

	return true;
}

/**
 @brief Update this class instance
 */
bool CMenuState::Update(const double dElapsedTime)
{
	
	if (Application::IsKeyPressed(VK_SPACE))
	{
		//Setting state and switching the scene to lobby
		sceneManager->setScene(w_lobby);
		CGameStateManager::GetInstance()->SetActiveGameState("LobbyState");
		
		//Fading effect for sound
		DEBUG_MSG("Fading out");
		CSoundController::GetInstance()->StopPlayingSoundByID(SOUND_TYPE::BG_MAINMENU, 3, 0.5);
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
	//cout << "CMenuState::Render()\n" << endl;
}

/**
 @brief Destroy this class instance
 */
void CMenuState::Destroy(void)
{

	cout << "CMenuState::Destroy()\n" << endl;
}
