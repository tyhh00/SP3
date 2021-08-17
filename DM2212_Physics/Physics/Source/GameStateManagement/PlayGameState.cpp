// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLFW
#include <GLFW/glfw3.h>

#include "PlayGameState.h"

// Include CGameStateManager
#include "GameStateManager.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CPlayGameState::CPlayGameState(void)
{

}

/**
 @brief Destructor
 */
CPlayGameState::~CPlayGameState(void)
{

}

/**
 @brief Init this class instance
 */
bool CPlayGameState::Init(void)
{
	cout << "CPlayGameState::Init()\n" << endl;

	sceneManager = SceneManager::GetInstance();
	sceneManager->init();

	return true;
}

/**
 @brief Update this class instance
 */
bool CPlayGameState::Update(const double dElapsedTime)
{
	sceneManager->getScene()->Update(dElapsedTime);
	if (Application::IsKeyPressed(VK_ESCAPE))
	{
		// Load the menu state
		cout << "Loading MenuState" << endl;
		CGameStateManager::GetInstance()->SetActiveGameState("MenuState");
		return true;
	}
	return true;
}

/**
 @brief Render this class instance
 */
void CPlayGameState::Render(void)
{
	cout << "CPlayGameState::Render()\n" << endl;
	sceneManager->getScene()->Render();

}

/**
 @brief Destroy this class instance
 */
void CPlayGameState::Destroy(void)
{
	cout << "CPlayGameState::Destroy()\n" << endl;
	sceneManager->destroy();
	sceneManager->Destroy();
}