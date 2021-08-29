// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLFW
#include <GLFW/glfw3.h>

#include "LobbyState.h"

#include "../Input.h"

// Include CGameStateManager
#include "GameStateManager.h"

#include "../UIManager.h"
#include "../GameManager.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CLobbyState::CLobbyState(void)
{
	sceneLobby = new SceneLobby();
	sceneManager = SceneManager::GetInstance();

	// Input
	input = Input::GetInstance();

}

/**
 @brief Destructor
 */
CLobbyState::~CLobbyState(void)
{
	if (sceneLobby)
	{
		delete sceneLobby;
		sceneLobby = NULL;
	}
}

/**
 @brief Init this class instance
 */
bool CLobbyState::Init(void)
{
	cout << "CLobbyState::Init()\n" << endl;
	sceneLobby->Init();
	return true;
}

/**
 @brief Update this class instance
 */
bool CLobbyState::Update(const double dElapsedTime)
{
	sceneLobby->Update(dElapsedTime);

	if (input->IsKeyPressed('1'))
	{
		sceneManager->setScene(w_jungle);
		CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
		return true;
	}


	if (input->IsKeyPressed('2'))
	{
		sceneManager->setScene(w_graveyard); 
		CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
		return true;
	}

	if (input->IsKeyPressed('3'))
	{
		sceneManager->setScene(w_levelEditor);
		CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
		return true;
	}

	if (input->IsKeyPressed('4'))
	{
		sceneManager->setScene(w_robot);
		CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
		return true;
	}

	if (input->IsKeyPressed('5'))
	{
		sceneManager->setScene(w_ocean);
		CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
		return true;
	}

	if (input->IsKeyPressed(VK_ESCAPE))
	{
		CGameStateManager::GetInstance()->SetActiveGameState("MenuState");
	}
	if (GameManager::GetInstance()->getGameState() == GameManager::GS_CUTSCENE
		&& !DialogueManager::GetInstance()->isDialogue())
	{
		// switch to main menu
		CGameStateManager::GetInstance()->SetActiveGameState("MenuState");
	}
	return true;
}

/**
 @brief Render this class instance
 */
void CLobbyState::Render(void)
{
	// Clear the screen and buffer
	glClearColor(0.0f, 0.55f, 1.00f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sceneLobby->Render();
	// cout << "CLobbyState::Render()\n" << endl;
}

/**
 @brief Destroy this class instance
 */
void CLobbyState::Destroy(void)
{
	sceneLobby->Exit();
	cout << "CLobbyState::Destroy()\n" << endl;
}