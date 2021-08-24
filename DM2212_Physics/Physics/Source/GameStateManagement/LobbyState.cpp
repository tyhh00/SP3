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

}

/**
 @brief Destructor
 */
CLobbyState::~CLobbyState(void)
{
	for (auto& mesh : meshGenerated)
{
	delete mesh;
	mesh = nullptr;
}
meshGenerated.clear();
}

/**
 @brief Init this class instance
 */
bool CLobbyState::Init(void)
{
	cout << "CLobbyState::Init()\n" << endl;

	//Init of UIManagement
	uiManager = UIManager::GetInstance();
	ButtonManager* bm_gameplayStat = uiManager->GetButtonManager(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS);

	if (meshGenerated.size() < 1)
	{
		//HealthBar
		Mesh* playerHealth = MeshBuilder::GenerateQuad("health", Color(0.8, 0.2, 0.2), 1.0f);
		meshGenerated.push_back(playerHealth);

		Mesh* healthIcon = MeshBuilder::GenerateQuad("health", Color(0.9, 0.2, 0.2), 1.0f);
		healthIcon->textureID = LoadTGA("Image/lives.tga");
		meshGenerated.push_back(healthIcon);

		Mesh* coinIcon = MeshBuilder::GenerateQuad("coin", Color(0.9, 0.2, 0.2), 1.0f);
		coinIcon->textureID = LoadTGA("Image/tiles/coin_tile.tga");
		meshGenerated.push_back(coinIcon);

		//Health Bar
		bm_gameplayStat->addButton(ButtonFactory::createProgressBar("playerhealth", 7, 59, 1, 7, HORIZONTAL, playerHealth));
		//Health Icon
		bm_gameplayStat->addButton(ButtonFactory::createNoTextButton("healthicon", 2.2, 59, 1.2, 1.2, healthIcon));

		//Coin Icon
		bm_gameplayStat->addButton(ButtonFactory::createNoTextButton("coinicon", 2.2, 57, 1.2, 1.2, coinIcon));
		bm_gameplayStat->addButton(ButtonFactory::createTextButton("coinvalue", 4.6, 55.1, 2, 2, 0, 0, Color(0.9, 0.9, 0.9), std::to_string(GameManager::GetInstance()->getCoins()), 1.4, SUPERMARIO));
	}

	sceneLobby = new SceneLobby();
	sceneLobby->Init();
	sceneManager = SceneManager::GetInstance();
	sceneManager->init();

	// Input
	input = Input::GetInstance();

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

	if (input->IsKeyPressed(VK_ESCAPE))
	{
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
	cout << "CLobbyState::Destroy()\n" << endl;
	//sceneManager->destroy();
}