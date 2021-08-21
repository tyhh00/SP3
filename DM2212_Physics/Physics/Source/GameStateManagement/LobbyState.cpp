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

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CLobbyState::CLobbyState(void)
{

}

/**
 @brief Destructor
 */
CLobbyState::~CLobbyState(void)
{

}

/**
 @brief Init this class instance
 */
bool CLobbyState::Init(void)
{
	cout << "CLobbyState::Init()\n" << endl;

	sceneManager = SceneManager::GetInstance();
	sceneManager->init();

	return true;
}

/**
 @brief Update this class instance
 */
bool CLobbyState::Update(const double dElapsedTime)
{
	if (oneKeyReleased && Application::IsKeyPressed('1'))
	{
		oneKeyReleased = false;
		sceneManager->setScene(w_jungle);
		CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
		return true;
	}
	else if (!oneKeyReleased && !Application::IsKeyPressed('1'))
	{
		oneKeyReleased = true;
	}

	if (twoKeyReleased && Application::IsKeyPressed('2'))
	{
		twoKeyReleased = false;
		sceneManager->setScene(w_graveyard); 
		CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
		return true;
	}
	else if (!twoKeyReleased && !Application::IsKeyPressed('2'))
	{
		twoKeyReleased = true;
	}

	if (threeKeyReleased && Application::IsKeyPressed('3'))
	{
		threeKeyReleased = false;
		sceneManager->setScene(w_levelEditor);
		CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
		return true;
	}
	else if (!threeKeyReleased && !Application::IsKeyPressed('3'))
	{
		threeKeyReleased = true;
	}

	if (fourKeyReleased && Application::IsKeyPressed('4'))
	{
		fourKeyReleased = false;
		sceneManager->setScene(w_robot);
		CGameStateManager::GetInstance()->SetActiveGameState("PlayGameState");
		return true;
	}
	else if (!fourKeyReleased && !Application::IsKeyPressed('4'))
	{
		fourKeyReleased = true;
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
	//cout << "CLobbyState::Render()\n" << endl;
}

/**
 @brief Destroy this class instance
 */
void CLobbyState::Destroy(void)
{
	cout << "CLobbyState::Destroy()\n" << endl;
}