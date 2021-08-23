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

#include "PauseGameState.h"

// Include CGameStateManager
#include "GameStateManager.h"

// Include Mesh Builder
#include "../MeshBuilder.h"
#include "../Debug.h"


#include "../SoundController/SoundController.h"

#include <iostream>
#include <sstream>
using namespace std;

/**
 @brief Constructor
 */
CPauseGameState::CPauseGameState(void)
	//: background(NULL)
{

}

/**
 @brief Destructor
 */
CPauseGameState::~CPauseGameState(void)
{

}

/**
 @brief Init this class instance
 */
bool CPauseGameState::Init(void)
{
	cout << "CPauseGameState::Init()\n" << endl;

	sceneManager = SceneManager::GetInstance();
	sceneManager->init();
	
	// Input
	input = Input::GetInstance();

	bgMesh = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	bgMesh->textureID = LoadTGA("Image/GraveyardBG.tga");

	return true;
}

/**
 @brief Update this class instance
 */
bool CPauseGameState::Update(const double dElapsedTime)
{
	
	
	return true;
}

/**
 @brief Render this class instance
 */
void CPauseGameState::Render(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

}

/**
 @brief Destroy this class instance
 */
void CPauseGameState::Destroy(void)
{

	cout << "CPauseGameState::Destroy()\n" << endl;
}
