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
	, retryButtonMesh(NULL)
	, menuBG(NULL)
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

	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	sceneManager = SceneManager::GetInstance();
	//sceneManager->Init();

	buttonManager = new ButtonManager(80, 60);

	resumeButtonMesh = MeshBuilder::GenerateQuad("resume button", Color(1, 1, 1), 1.0f);
	resumeButtonMesh->textureID = LoadTGA("Image/ResumeButton.tga");
	lobbyButtonMesh = MeshBuilder::GenerateQuad("back to lobby button", Color(1, 1, 1), 1.0f);
	lobbyButtonMesh->textureID = LoadTGA("Image/LobbyButton.tga");
	retryButtonMesh = MeshBuilder::GenerateQuad("retry button", Color(1, 1, 1), 1.0f);
	retryButtonMesh->textureID = LoadTGA("Image/RetryButton.tga");
	menuBG = MeshBuilder::GenerateQuad("menu bg", Color(1, 1, 1), 1.0f);
	menuBG->textureID = LoadTGA("Image/MenuBG.tga");

	Button* menuBGButton = ButtonFactory::createNoTextButton("menuBG", 40, 30,
												30, 40, menuBG);
	Button* resumeButton = ButtonFactory::createNoTextButton("resume", 40, 30,
												15, 7, resumeButtonMesh);
	Button* lobbyButton = ButtonFactory::createNoTextButton("lobby", 40, 20,
												15, 7, lobbyButtonMesh);
	Button* retryButton = ButtonFactory::createNoTextButton("retry", 40, 30,
												15, 7, retryButtonMesh);
	menuBGButton->disable();
	resumeButton->disable();
	lobbyButton->disable();
	retryButton->disable();
	buttonManager->addButton(resumeButton);
	buttonManager->addButton(lobbyButton);
	buttonManager->addButton(retryButton);
	buttonManager->addButton(menuBGButton);

	currentState = DEFAULT;

	return true;
}

/**
 @brief Update this class instance
 */
bool CPlayGameState::Update(const double dElapsedTime)
{
	switch (currentState)
	{
	case DEFAULT:
		sceneManager->update(dElapsedTime);
		break;
	case PAUSED:
		break;
	case GAMEOVER:
		break;
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
				buttonManager->deactivateButton("menuBG");
				currentState = DEFAULT;
			}
			else if (button->buttonClicked->getName() == "retry")
			{
				buttonManager->deactivateButton("retry");
				buttonManager->deactivateButton("lobby");
				buttonManager->deactivateButton("menuBG");
				sceneManager->resetScene();
				currentState = DEFAULT;
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
	
	// state change conditions
	if (currentState == DEFAULT && Input::GetInstance()->IsKeyPressed(VK_ESCAPE))
	{
		currentState = PAUSED;
		buttonManager->activateButton("resume");
		buttonManager->activateButton("lobby");
		buttonManager->activateButton("menuBG");
	}
	if (sceneManager->getScene()->gameLost)
	{
		currentState = GAMEOVER;
		buttonManager->activateButton("retry");
		buttonManager->activateButton("lobby");
		buttonManager->activateButton("menuBG");
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
	sceneManager->destroy();
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
	if (retryButtonMesh)
	{
		delete retryButtonMesh;
		retryButtonMesh = NULL;
	}
	if (menuBG)
	{
		delete menuBG;
		menuBG = NULL;
	}
}