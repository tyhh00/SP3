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