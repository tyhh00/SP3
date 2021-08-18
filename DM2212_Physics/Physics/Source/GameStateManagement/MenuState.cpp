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

#include "MenuState.h"

// Include CGameStateManager
#include "GameStateManager.h"

// Include Mesh Builder
#include "../MeshBuilder.h"
// Include ImageLoader
//#include "System\ImageLoader.h"
// Include Shader Manager
//#include "RenderControl\ShaderManager.h"

 // Include shader
//#include "RenderControl\shader.h"

// Include CSettings
//#include "GameControl/Settings.h"

// Include CKeyboardController
//#include "Inputs/KeyboardController.h"
//#include "../Application.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CMenuState::CMenuState(void)
	//: background(NULL)
{

}

/**
 @brief Destructor
 */
CMenuState::~CMenuState(void)
{

}

/**
 @brief Init this class instance
 */
bool CMenuState::Init(void)
{
	cout << "CMenuState::Init()\n" << endl;

	//CShaderManager::GetInstance()->Use("2DShader");
	//CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);

	////Create Background Entity
	//background = new CBackgroundEntity("Image/MenuBackground.png");
	//background->SetShader("2DShader");
	//background->Init();

	// Setup Dear ImGui context
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;

	//// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	/*ImGui_ImplGlfw_InitForOpenGL(CSettings::GetInstance()->pWindow, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);*/

	// Load the images for buttons
	//CImageLoader* il = CImageLoader::GetInstance();
	//startButtonData.fileName = "Image\\GUI\\PlayButton.png";
	//startButtonData.textureID = il->LoadTextureGetID(startButtonData.fileName.c_str(), false);
	//optionsButtonData.fileName = "Image\\GUI\\OptionsButton.png";
	//optionsButtonData.textureID = il->LoadTextureGetID(optionsButtonData.fileName.c_str(), false);
	//exitButtonData.fileName = "Image\\GUI\\ExitButton.png";
	//exitButtonData.textureID = il->LoadTextureGetID(exitButtonData.fileName.c_str(), false);

	return true;
}

/**
 @brief Update this class instance
 */
bool CMenuState::Update(const double dElapsedTime)
{
	
	if (Application::IsKeyPressed(VK_SPACE))
	{
		// Reset the CKeyboardController
		cout << "space key pressed" << endl;
		// Load the menu state
		cout << "Loading PlayGameState" << endl;
		CGameStateManager::GetInstance()->SetActiveGameState("LobbyState");
		return true;
	}
	return true;
}

/**
 @brief Render this class instance
 */
void CMenuState::Render(void)
{
	// Clear the screen and buffer
	glClearColor(0.0f, 0.55f, 1.00f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cout << "CMenuState::Render()\n" << endl;
}

/**
 @brief Destroy this class instance
 */
void CMenuState::Destroy(void)
{

	cout << "CMenuState::Destroy()\n" << endl;
}
