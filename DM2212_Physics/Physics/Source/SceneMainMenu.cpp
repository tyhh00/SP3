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
#include "GameManager.h"

SceneMainMenu::SceneMainMenu() : buttonManager(NULL)
	, PlayButtonMesh(NULL)
	, QuitButtonMesh(NULL)
	, title(NULL)
	, background(NULL)
	, buttonHighlight(NULL)
	
{
}

SceneMainMenu::~SceneMainMenu()
{
	
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

	GameManager::GetInstance()->resetAll();

	background = MeshBuilder::GenerateSpriteAnimation(5, 5, 1.0f, 1.0f);
	background->AddAnimation("idle", 0, 24);
	title = MeshBuilder::GenerateQuad("title", Color(1, 1, 1), 1.0f);
	title->textureID = LoadTGA("Image/Title.tga");

	background->PlayAnimation("idle", -1, 2.5f);
	background->textureID = LoadTGA("Image/MainMenuBG.tga");

	buttonManager = new ButtonManager(80, 60);

	Mesh* highlight = MeshBuilder::GenerateQuad("Button Hover Highlight", Color(1, 1, 1), 1.0f);
	highlight->textureID = LoadTGA("Image/ButtonHighlightB.tga");

	PlayButtonMesh = MeshBuilder::GenerateQuad("Play button", Color(1, 1, 1), 1.0f);
	PlayButtonMesh->textureID = LoadTGA("Image/PlayButton.tga");
	QuitButtonMesh = MeshBuilder::GenerateQuad("Quit button", Color(1, 1, 1), 1.0f);
	QuitButtonMesh->textureID = LoadTGA("Image/QuitButton.tga");

	Button* playButton = ButtonFactory::createNoTextButton("play", 40, 20, 12, 6, PlayButtonMesh);
	buttonManager->addButton(playButton);
	Button* quitButton = ButtonFactory::createNoTextButton("quit", 40, 12, 12, 6, QuitButtonMesh);
	buttonManager->addButton(quitButton);
	buttonHighlight = ButtonFactory::createNoTextButton("highlight", 40, 30,
		13, 7, highlight);
	buttonManager->addButton(buttonHighlight);

	buttonManager->deactivateButton("highlight");

	camera.Init(Vector3(m_worldWidth * 0.5, m_worldHeight * 0.5, 1), Vector3(m_worldWidth * 0.5, m_worldHeight * 0.5, 0), Vector3(0, 1, 0));
	camera.SetLimits(m_screenWidth, m_screenHeight, m_worldWidth, m_worldHeight);
	
	bLightEnabled = false;
}


void SceneMainMenu::Update(double dt)
{
	SceneBase::Update(dt);
	background->Update(dt);
	
	buttonManager->Update(this, dt);
	
	buttonManager->deactivateButton("highlight");
	for (auto button : buttonManager->getButtonsInteracted())
	{
		if (button->hovering)
		{
			buttonManager->activateButton("highlight");
			buttonHighlight->setOrigin(button->buttonClicked->getOriginX(), button->buttonClicked->getOriginY());
		}
		if (button->justClicked)
		{
			if (button->buttonClicked->getName() == "play")
			{
				CGameStateManager::GetInstance()->SetActiveGameState("LobbyState");
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

	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
	}

	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
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
	RenderMesh(background, false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 0.5, m_worldHeight * 0.6, 0.1);
	modelStack.Scale(83.579, 30, 1);
	RenderMesh(title, false);
	modelStack.PopMatrix();

	buttonManager->Render(this);
}

void SceneMainMenu::Exit()
{
	SceneBase::Exit();
	if (PlayButtonMesh)
	{
		delete PlayButtonMesh;
		PlayButtonMesh = NULL;
	}
	if (QuitButtonMesh)
	{
		delete QuitButtonMesh;
		QuitButtonMesh = NULL;
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
