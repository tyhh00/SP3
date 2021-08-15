#include "SceneLevelSelection.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "MeshBuilder.h"
#include "LoadTGA.h"

SceneLevelSelection::SceneLevelSelection()
{
}

SceneLevelSelection::~SceneLevelSelection()
{
}

void SceneLevelSelection::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Setting of vars 
	mousePressed = false;
	mouseClicked = false;

	Math::InitRNG();

	// Meshes
	meshList[GEO_BG] = MeshBuilder::GenerateQuad("space background", Color(1, 1, 1), 1.0f);
	meshList[GEO_BG]->textureID = LoadTGA("Image/spacebackground3.tga");

	// Adding + Setting of Buttons
	Button* button;
	button = new Button("Level 1", Vector3(m_worldWidth / 2, 70, 0), 30, 10);
	buttonList.push_back(button);
	button = new Button("Level 2", Vector3(m_worldWidth / 2, 55, 0), 30, 10);
	buttonList.push_back(button);
	button = new Button("Level 3", Vector3(m_worldWidth / 2, 40, 0), 30, 10);
	buttonList.push_back(button);
	button = new Button("Back", Vector3(m_worldWidth / 2, 25, 0), 30, 10);
	buttonList.push_back(button);
	
}


void SceneLevelSelection::Update(double dt)
{
	SceneBase::Update(dt);

	//playButton.Set("Play", Vector3(Application::GetWindowWidth))

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
	// Mouse Click
	mouseClicked = false;
	if (Application::IsMousePressed(0))
	{
		mousePressed = true;
	}
	else
	{
		if (mousePressed)
		{
			mouseClicked = true;
		}
		mousePressed = false;
	}

	// Check for button click
	for (auto button : buttonList)
	{
		if (CheckButtonClick(*button))
		{
			if (button->GetName() == "Level 1")
			{
				Application::SwitchScene(Application::S_LEVEL1);
			}
			else if (button->GetName() == "Level 2")
			{
				Application::SwitchScene(Application::S_LEVEL2);
			}
			else if (button->GetName() == "Level 3")
			{
				Application::SwitchScene(Application::S_LEVEL3);
			}
			else if (button->GetName() == "Back")
			{
				Application::SwitchScene(Application::S_MAINMENU);
			}
		}
	}

}



void SceneLevelSelection::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
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
	
	RenderMesh(meshList[GEO_AXES], false);

	
	// Background
	modelStack.PushMatrix();
	modelStack.Translate(m_worldWidth * 0.5, m_worldHeight * 0.5, -1);
	modelStack.Rotate(180, 0, 0, 1);
	modelStack.Scale(m_worldWidth, m_worldHeight, 1);
	RenderMesh(meshList[GEO_BG], false);
	modelStack.PopMatrix();

	// Buttons
	for (auto button : buttonList)
	{
		RenderButton(button);
	}

	//On screen text

	// FPS
	std::ostringstream ss;
	ss.precision(3);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 4, 0, 4);

	// game name
	RenderTextOnScreen(meshList[GEO_TEXT], "Asteroid Shooter", Color(1, 1, 1), 4, 0, 0);
}

bool SceneLevelSelection::CheckButtonClick(Button button)
{
	// Convert mouse pos to game space pos
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	x /= (w / m_worldWidth);
	y = h - y; 
	y /= (h / m_worldHeight); 

	// Check if mouse clicked and within button area
	if (mouseClicked && button.CheckWithinButton(x, y))
	{
		return true;
	}
	return false;
}

void SceneLevelSelection::RenderButton(Button* button)
{
	// button
	modelStack.PushMatrix();
	modelStack.Translate(button->GetMid().x, button->GetMid().y, 0);
	modelStack.Scale(button->GetLength(), button->GetHeight(), 1);
	RenderMesh(meshList[GEO_BUTTON], false);
	modelStack.PopMatrix();

	// button's text
	int textSize = 5;
	float textposx = button->GetMid().x - (button->GetName().length() * 0.5f * textSize * 0.4f);
	float textposy = button->GetMid().y - (textSize * 0.4f);
	RenderTextOnScreen(meshList[GEO_TEXT], button->GetName(), Color(0, 0, 0), textSize, textposx, textposy);
}

void SceneLevelSelection::Exit()
{
	SceneBase::Exit();
	
}
