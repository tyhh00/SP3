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

#include "../Buttons/ButtonFactory.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CPlayGameState::CPlayGameState(void) : buttonManager(NULL)
	, resumeButtonMesh(NULL)
	, lobbyButtonMesh(NULL)
{

}

/**
 @brief Destructor
 */
CPlayGameState::~CPlayGameState(void)
{
	if (buttonManager)
	{
		delete buttonManager;
		buttonManager = NULL;
	}
	if (resumeButtonMesh)
	{
		delete resumeButtonMesh;
		resumeButtonMesh = NULL;
	}
	if (lobbyButtonMesh)
	{
		delete lobbyButtonMesh;
		lobbyButtonMesh = NULL;
	}
}

/**
 @brief Init this class instance
 */
bool CPlayGameState::Init(void)
{
	cout << "CPlayGameState::Init()\n" << endl;

	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	sceneManager = SceneManager::GetInstance();

	buttonManager = new ButtonManager(80, 60);

	resumeButtonMesh = MeshBuilder::GenerateQuad("resume button", Color(1, 1, 1), 1.0f);
	resumeButtonMesh->textureID = LoadTGA("Image/button.tga");
	lobbyButtonMesh = MeshBuilder::GenerateQuad("back to lobby button", Color(1, 1, 1), 1.0f);
	lobbyButtonMesh->textureID = LoadTGA("Image/button.tga");

	Button* resumeButton = ButtonFactory::createNoTextButton("resume", 40, 30,
												10, 5, resumeButtonMesh);
	Button* lobbyButton = ButtonFactory::createNoTextButton("lobby", 40, 20,
												10, 5, lobbyButtonMesh);
	resumeButton->disable();
	lobbyButton->disable();
	buttonManager->addButton(resumeButton);
	buttonManager->addButton(lobbyButton);

	paused = false;

	return true;
}

/**
 @brief Update this class instance
 */
bool CPlayGameState::Update(const double dElapsedTime)
{
	if (!paused)
	{
		sceneManager->update(dElapsedTime);
	}
	buttonManager->Update(sceneManager->getScene(), dElapsedTime);
	for (auto button : buttonManager->getButtonsInteracted())
	{
		if (button->justClicked)
		{
			if (button->buttonClicked->getName() == "resume")
			{
				buttonManager->deactivateButton("resume");
				buttonManager->deactivateButton("lobby");
				paused = false;
			}
			else if (button->buttonClicked->getName() == "lobby")
			{
				// Load the menu state
				cout << "Loading LobbyState" << endl;
				CGameStateManager::GetInstance()->SetActiveGameState("LobbyState");
				return true;
			}
		}
	}
	
	if (!paused && Input::GetInstance()->IsKeyPressed(VK_ESCAPE))
	{
		paused = true;
		buttonManager->activateButton("resume");
		buttonManager->activateButton("lobby");
	}
	return true;
}

/**
 @brief Render this class instance
 */
void CPlayGameState::Render(void)
{
	glClearColor(0.0f, 0.55f, 1.00f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	sceneManager->render();
	buttonManager->Render(sceneManager->getScene());
}

/**
 @brief Destroy this class instance
 */
void CPlayGameState::Destroy(void)
{
	cout << "CPlayGameState::Destroy()\n" << endl;
	//sceneManager->destroy();
	//sceneManager->Destroy();
}