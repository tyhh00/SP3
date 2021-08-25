#include "SceneMainMenu.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"
#include "Utility.h"
#include <sstream>
#include "GameStateManagement/GameStateManager.h"
#include "Debug.h"
#include "SoundController/SoundController.h"
#include "Buttons/ButtonFactory.h"

SceneMainMenu::SceneMainMenu() : buttonManager(NULL)
		, buttonMesh(NULL)
	, background(NULL)
	
{
}

SceneMainMenu::~SceneMainMenu()
{
	if (buttonMesh)
	{
		delete buttonMesh;
		buttonMesh = NULL;
	}
	if (buttonManager)
	{
		delete buttonManager;
		buttonManager = NULL;
	}
	if (background)
	{
		delete background;
		background = NULL;
	}
}

void SceneMainMenu::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_worldHeight = m_screenHeight;
	m_worldWidth = m_screenWidth;

	Math::InitRNG();


	background = MeshBuilder::GenerateSpriteAnimation(5, 5, 1.0f, 1.0f);
	background->AddAnimation("idle", 0, 24);

	background->PlayAnimation("idle", -1, 2.5f);
	background->textureID = LoadTGA("Image/MainMenuBG.tga");

	buttonManager = new ButtonManager(80, 60);

	buttonMesh = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	buttonMesh->textureID = LoadTGA("Image/button.tga");

	Button* playButton = ButtonFactory::createNoTextButton("play", 40, 30, 10, 5, buttonMesh);
	buttonManager->addButton(playButton);
	Button* creditsButton = ButtonFactory::createNoTextButton("credits", 40, 20, 10, 5, buttonMesh);
	buttonManager->addButton(creditsButton);
	Button* quitButton = ButtonFactory::createNoTextButton("quit", 40, 10, 10, 5, buttonMesh);
	buttonManager->addButton(quitButton);

	camera.Init(Vector3(m_worldWidth * 0.5, m_worldHeight * 0.5, 1), Vector3(m_worldWidth * 0.5, m_worldHeight * 0.5, 0), Vector3(0, 1, 0));
	camera.SetLimits(m_screenWidth, m_screenHeight, m_worldWidth, m_worldHeight);
	
	bLightEnabled = false;
}


void SceneMainMenu::Update(double dt)
{
	SceneBase::Update(dt);
	buttonManager->Update(this, dt);
	background->Update(dt);

	for (auto button : buttonManager->getButtonsInteracted())
	{
		if (button->justClicked)
		{
			if (button->buttonClicked->getName() == "play")
			{
				CGameStateManager::GetInstance()->SetActiveGameState("LobbyState");
				//Fading effect for sound
				DEBUG_MSG("Fading out");
				CSoundController::GetInstance()->StopPlayingSoundByID(SOUND_TYPE::BG_MAINMENU, 3, 0.5);
			}
			else if (button->buttonClicked->getName() == "credits")
			{
				//...
			}
			else if (button->buttonClicked->getName() == "quit")
			{
				Application::quit = true;
			}
		}
	}

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}

	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}


}

void SceneMainMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(-1 * m_screenWidth * 0.5f, m_screenWidth * 0.5f, -1 * m_screenHeight * 0.5f, m_screenHeight * 0.5f, -10, 10);
//	projection.SetToPerspective(45.f, m_screenWidth / m_screenHeight, 0.1f, 1000.f);

	projectionStack.LoadMatrix(projection);


	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 0.5, m_worldHeight * 0.5, 0);
	modelStack.Scale(m_worldWidth, m_worldHeight, 1);
	RenderMesh(background, true);
	modelStack.PopMatrix();


	buttonManager->Render(this);

	std::ostringstream ss;

	// fps tings
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 0, 3);

	RenderTextOnScreen(meshList[GEO_TEXT], "Main Menu", Color(1, 1, 1), 2, 0, 0);
}

void SceneMainMenu::Exit()
{
	SceneBase::Exit();
}
