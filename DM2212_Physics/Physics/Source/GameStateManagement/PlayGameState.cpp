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
, backButtonMesh(NULL)
, menuBG(NULL)
, buttonHighlight(NULL)
, leftArrowMesh(NULL)
, rightArrowMesh(NULL)
{
	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	sceneManager = SceneManager::GetInstance();
	gameManager = GameManager::GetInstance();
	soundController = CSoundController::GetInstance();
	dialogueManager = DialogueManager::GetInstance();
	uiManager = UIManager::GetInstance();

	buttonManager = new ButtonManager(80, 60);
	buttonManager->SetZPriority(5);

	// ALL MESHES
	resumeButtonMesh = MeshBuilder::GenerateQuad("resume button", Color(1, 1, 1), 1.0f);
	resumeButtonMesh->textureID = LoadTGA("Image/ResumeButton.tga");
	lobbyButtonMesh = MeshBuilder::GenerateQuad("back to lobby button", Color(1, 1, 1), 1.0f);
	lobbyButtonMesh->textureID = LoadTGA("Image/LobbyButton.tga");
	retryButtonMesh = MeshBuilder::GenerateQuad("retry button", Color(1, 1, 1), 1.0f);
	retryButtonMesh->textureID = LoadTGA("Image/RetryButton.tga");
	optionsButtonMesh = MeshBuilder::GenerateQuad("options button", Color(1, 1, 1), 1.0f);
	optionsButtonMesh->textureID = LoadTGA("Image/OptionsButton.tga");
	backButtonMesh = MeshBuilder::GenerateQuad("back button", Color(1, 1, 1), 1.0f);
	backButtonMesh->textureID = LoadTGA("Image/BackButton.tga");
	menuBG = MeshBuilder::GenerateQuad("menu bg", Color(1, 1, 1), 1.0f);
	menuBG->textureID = LoadTGA("Image/MenuBG.tga");

	Mesh* highlight = MeshBuilder::GenerateQuad("Button Hover Highlight", Color(1, 1, 1), 1.0f);
	highlight->textureID = LoadTGA("Image/ButtonHighlightA.tga");

	volumeBar[0] = MeshBuilder::GenerateQuad("volume bar", Color(1, 1, 1), 1.0f);
	volumeBar[0]->textureID = LoadTGA("Image/VolumeBar_0.tga");
	volumeBar[1] = MeshBuilder::GenerateQuad("volume bar", Color(1, 1, 1), 1.0f);
	volumeBar[1]->textureID = LoadTGA("Image/VolumeBar_1.tga");
	volumeBar[2] = MeshBuilder::GenerateQuad("volume bar", Color(1, 1, 1), 1.0f);
	volumeBar[2]->textureID = LoadTGA("Image/VolumeBar_2.tga");
	volumeBar[3] = MeshBuilder::GenerateQuad("volume bar", Color(1, 1, 1), 1.0f);
	volumeBar[3]->textureID = LoadTGA("Image/VolumeBar_3.tga");
	volumeBar[4] = MeshBuilder::GenerateQuad("volume bar", Color(1, 1, 1), 1.0f);
	volumeBar[4]->textureID = LoadTGA("Image/VolumeBar_4.tga");
	volumeBar[5] = MeshBuilder::GenerateQuad("volume bar", Color(1, 1, 1), 1.0f);
	volumeBar[5]->textureID = LoadTGA("Image/VolumeBar_5.tga");
	leftArrowMesh = MeshBuilder::GenerateQuad("left arrow", Color(1, 1, 1), 1.0f);
	leftArrowMesh->textureID = LoadTGA("Image/LeftArrow.tga");
	rightArrowMesh = MeshBuilder::GenerateQuad("right arrow", Color(1, 1, 1), 1.0f);
	rightArrowMesh->textureID = LoadTGA("Image/RightArrow.tga");

	// ALL BUTTONS
	Button* menuBGButton = ButtonFactory::createNoTextButton("menuBG", 40, 30, 30, 40, menuBG);
	Button* resumeButton = ButtonFactory::createNoTextButton("resume", 40, 40, 15, 7, resumeButtonMesh);
	Button* optionsButton = ButtonFactory::createNoTextButton("options", 40, 30, 15, 7, optionsButtonMesh);
	Button* lobbyButton = ButtonFactory::createNoTextButton("lobby", 40, 20, 15, 7, lobbyButtonMesh);
	Button* retryButton = ButtonFactory::createNoTextButton("retry", 40, 30, 15, 7, retryButtonMesh);
	Button* backButton = ButtonFactory::createNoTextButton("back", 40, 20, 15, 7, backButtonMesh);
	buttonHighlight = ButtonFactory::createNoTextButton("highlight", 40, 30, 16, 8, highlight);

	Button* leftArrowButton = ButtonFactory::createNoTextButton("leftarrow", 35, 30, 4, 4, leftArrowMesh);
	Button* rightArrowButton = ButtonFactory::createNoTextButton("rightarrow", 45, 30, 4, 4, rightArrowMesh);
	Button* volumeBarButton = ButtonFactory::createNoTextButton("volumebar", 40, 35, 23, 4, 
		volumeBar[5]);

	// DISABLE AND ADD BUTTONS TO BUTTON MANAGER
	menuBGButton->disable();
	resumeButton->disable();
	lobbyButton->disable();
	retryButton->disable();
	optionsButton->disable();
	backButton->disable();
	leftArrowButton->disable();
	rightArrowButton->disable();
	volumeBarButton->disable();
	buttonHighlight->disable();
	buttonManager->addButton(resumeButton);
	buttonManager->addButton(lobbyButton);
	buttonManager->addButton(retryButton);
	buttonManager->addButton(optionsButton);
	buttonManager->addButton(backButton);
	buttonManager->addButton(leftArrowButton);
	buttonManager->addButton(rightArrowButton);
	buttonManager->addButton(volumeBarButton);

	buttonManager->addButton(buttonHighlight); // second last
	buttonManager->addButton(menuBGButton); // must be last

	// my own vectors, just to for loop
	pauseButtonsList.push_back(menuBGButton);
	pauseButtonsList.push_back(lobbyButton);
	pauseButtonsList.push_back(optionsButton);
	pauseButtonsList.push_back(resumeButton);
	optionsButtonsList.push_back(backButton);
	optionsButtonsList.push_back(menuBGButton);
	optionsButtonsList.push_back(leftArrowButton);
	optionsButtonsList.push_back(rightArrowButton);
	optionsButtonsList.push_back(volumeBarButton);
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
	if (leftArrowMesh)
	{
		delete leftArrowMesh;
		leftArrowMesh = NULL;
	}
	if (rightArrowMesh)
	{
		delete rightArrowMesh;
		rightArrowMesh = NULL;
	}
	for (int i = 0; i < 6; i++)
	{
		if (volumeBar[i])
		{
			delete volumeBar[i];
			volumeBar[i] = NULL;
		}
	}
	pauseButtonsList.clear();
	optionsButtonsList.clear();
}

/**
 @brief Init this class instance
 */
bool CPlayGameState::Init(void)
{

	uiManager->SetActive(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS, true);

	currentState = DEFAULT;

	UpdateVolumeBar();

	buttonManager->deactivateButton("highlight");
	buttonManager->deactivateButton("retry");
	for (int i = 0; i < pauseButtonsList.size(); i++)
	{
		buttonManager->deactivateButton(pauseButtonsList[i]->getName());
	}
	for (int i = 0; i < optionsButtonsList.size(); i++)
	{
		buttonManager->deactivateButton(optionsButtonsList[i]->getName());
	}

	lastClickedTime = 0;

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
		if (!dialogueManager->Update(dElapsedTime))
			sceneManager->update(dElapsedTime);
		// state change conditions
		if (currentState == DEFAULT && Input::GetInstance()->IsKeyPressed(VK_ESCAPE))
		{
			currentState = PAUSED;
			for (int i = 0; i < pauseButtonsList.size(); i++)
			{
				buttonManager->activateButton(pauseButtonsList[i]->getName());
			}
		}
		if (sceneManager->getScene()->gameLost)
		{
			currentState = GAMEOVER;
			buttonManager->activateButton("retry");
			buttonManager->activateButton("lobby");
			buttonManager->activateButton("menuBG");
		}

		if (sceneManager->getScene()->gameWin)
		{
			dialogueManager->AddDialogue(PLAYER, "I have the time machine part now, let's go back.");
			currentState = LEVELWON;
		}
		break;
	case PAUSED:
		break;
	case GAMEOVER:
		break;
	case LEVELWON:
		dialogueManager->Update(dElapsedTime);
		if (!dialogueManager->isDialogue())
		{

			CGameStateManager::GetInstance()->SetActiveGameState("LobbyState");
		}
		break;
	}

	UIManager::GetInstance()->Update(dElapsedTime);
	buttonManager->deactivateButton("highlight");
	buttonManager->Update(sceneManager->getScene(), dElapsedTime);
	lastClickedTime += dElapsedTime;
	for (auto button : buttonManager->getButtonsInteracted())
	{
		if (button->hovering 
			&& button->buttonClicked->getName() != "menuBG"
			&& button->buttonClicked->getName() != "leftarrow"
			&& button->buttonClicked->getName() != "rightarrow"
			&& button->buttonClicked->getName() != "volumebar"
			)
		{
			buttonManager->activateButton("highlight");
			buttonHighlight->setOrigin(button->buttonClicked->getOriginX(), button->buttonClicked->getOriginY());
		}
		if (button->justClicked && lastClickedTime > 0.5)
		{
			if (button->buttonClicked->getName() == "resume")
			{
				for (int i = 0; i < pauseButtonsList.size(); i++)
				{
					buttonManager->deactivateButton(pauseButtonsList[i]->getName());
				}
				currentState = DEFAULT;
				lastClickedTime = 0;
				break;
			}
			else if (button->buttonClicked->getName() == "retry")
			{
				buttonManager->deactivateButton("retry");
				buttonManager->deactivateButton("lobby");
				buttonManager->deactivateButton("menuBG");
				sceneManager->resetScene();
				currentState = DEFAULT;
				lastClickedTime = 0;
				break;
			}
			else if (button->buttonClicked->getName() == "options")
			{
				for (int i = 0; i < pauseButtonsList.size(); i++)
				{
					buttonManager->deactivateButton(pauseButtonsList[i]->getName());
				}
				for (int i = 0; i < optionsButtonsList.size(); i++)
				{
					buttonManager->activateButton(optionsButtonsList[i]->getName());
				}
				lastClickedTime = 0;
				break;
			}
			else if (button->buttonClicked->getName() == "back")
			{
				for (int i = 0; i < optionsButtonsList.size(); i++)
				{
					buttonManager->deactivateButton(optionsButtonsList[i]->getName());
				}
				for (int i = 0; i < pauseButtonsList.size(); i++)
				{
					buttonManager->activateButton(pauseButtonsList[i]->getName());
				}
				lastClickedTime = 0;
				break;
			}
			else if (button->buttonClicked->getName() == "lobby")
			{
				// Load the menu state
				CGameStateManager::GetInstance()->SetActiveGameState("LobbyState");
				lastClickedTime = 0;
				return true;
			}
			else if (button->buttonClicked->getName() == "rightarrow")
			{
				soundController->MasterVolumeIncrease();
				if (soundController->GetMasterVolume() > 0.2)
					soundController->MasterVolumeIncrease();
				UpdateVolumeBar();
				lastClickedTime = 0;
			}
			else if (button->buttonClicked->getName() == "leftarrow")
			{
				soundController->MasterVolumeDecrease();
				soundController->MasterVolumeDecrease();
				UpdateVolumeBar();
				lastClickedTime = 0;
			}
		}
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
	dialogueManager->Render(sceneManager->getScene());
	buttonManager->Render(sceneManager->getScene());
	UIManager::GetInstance()->Render(sceneManager->getScene());
}

/**
 @brief Destroy this class instance
 */
void CPlayGameState::Destroy(void)
{
	uiManager->SetActive(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS, false);
	if (gameManager->getCurrAbility(1) != nullptr)
		gameManager->getCurrAbility(1)->Reset();
	if (gameManager->getCurrAbility(1) != nullptr)
		gameManager->getCurrAbility(2)->Reset();
	sceneManager->destroy();
	dialogueManager->Exit();
}

void CPlayGameState::UpdateVolumeBar()
{
	int numBar;
	float volume = soundController->GetMasterVolume();
	if (volume >= 0.8f)
	{
		numBar = 5;
	}
	else if (volume >= 0.6f)
	{
		numBar = 4;
	}
	else if (volume >= 0.4f)
	{
		numBar = 3;
	}
	else if (volume >= 0.2f)
	{
		numBar = 2;
	}
	else if (volume > 0)
	{
		numBar = 1;
	}
	else 
	{
		numBar = 0;
	}
	//buttonManager->getButtonByName("volumebar")->setQuadImage(volumeBar[(int)(soundController->GetMasterVolume() * 5)]);
	buttonManager->getButtonByName("volumebar")->setQuadImage(volumeBar[numBar]);
}
