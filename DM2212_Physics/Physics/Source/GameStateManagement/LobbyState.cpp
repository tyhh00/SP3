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

CLobbyState::CLobbyState(void)
{
	sceneLobby = new SceneLobby();

	// Scene Manager
	sceneManager = SceneManager::GetInstance();

	// Input
	input = Input::GetInstance();

}

CLobbyState::~CLobbyState(void)
{
	if (sceneLobby)
	{
		delete sceneLobby;
		sceneLobby = NULL;
	}
}

bool CLobbyState::Init(void)
{
	sceneLobby->Init();
	return true;
}

bool CLobbyState::Update(const double dElapsedTime)
{
	sceneLobby->Update(dElapsedTime);

	//shortcuts to different world states
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

	return true;
}

void CLobbyState::Render(void)
{
	// Clear the screen and buffer
	glClearColor(0.0f, 0.55f, 1.00f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sceneLobby->Render();
}

void CLobbyState::Destroy(void)
{
	sceneLobby->Exit();
	cout << "CLobbyState::Destroy()\n" << endl;
}