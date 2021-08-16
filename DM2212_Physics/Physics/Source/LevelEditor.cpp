#include "LevelEditor.h"
#include "GL\glew.h"
#include "Application.h"
#include <fstream>
#include <sstream>   

std::vector<std::string> splitStr(std::string str, char delimiter);

LevelEditor::LevelEditor()
{

}

LevelEditor::~LevelEditor()
{
	for (int i = 0; i < gridObjects.size(); i++)
	{
		if(gridObjects[i])
			delete gridObjects[i];
	}
}

void LevelEditor::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_gameWidth = m_worldWidth * 0.75;

	camera.Init(Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1), Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0), Vector3(0, 1, 0));
	//camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
	std::string mapToLoad = "LEVEL_1.txt";
	LoadMap(mapToLoad);
}

bool LevelEditor::LoadMap(std::string filename)
{
	std::string path = file_path + filename;
	std::ifstream fileStream(path.c_str(), std::ios::binary);
	if (!fileStream.is_open())
	{
		DEBUG_MSG("Impossible to open " << file_path << ". Are you in the right directory ?\n");
		return false;
	}
	int line = 0;
	bool loadingTiles = false;

	while (!fileStream.eof())
	{
		if (fileStream.fail())
		{
			DEBUG_MSG("File Stream failed at line: " << line);
			break;
		}
		char buf[1028];
		fileStream.getline(buf, 1028);

		line++;
		if (std::string(buf).empty()) continue;
		if (buf[0] == '#') continue;

		std::string key;
		
		key = "GridLength:";
		if (strncmp(key.c_str(), buf, key.size()) == 0)
		{
			char split[1028];
			strcpy_s(split, buf + (buf[key.size()] == ' ' ? key.size() + 1 : key.size()));
			gridLength = std::stoi(split);
		}

		key = "GridHeight:";
		if (strncmp(key.c_str(), buf, key.size()) == 0)
		{
			char split[1028];
			strcpy_s(split, buf + (buf[key.size()] == ' ' ? key.size() + 1 : key.size()));
			gridHeight = std::stoi(split);
		}

		key = "Tiles:";
		if (strncmp(key.c_str(), buf, key.size()) == 0)
		{
			loadingTiles = true;
			continue;
		}

		if (loadingTiles)
		{
			key = std::string(buf);
			std::vector<std::string> split = splitStr(key, ':');
			if (split.size() < 4) //GO_Type;Pos;Rot;Scale
			{
				DEBUG_MSG("Skipping " << line << " as it has a broken format in configuration");
				continue;
			}
			Vector3 pos, normal, scale;
			GEOMETRY_TYPE type = static_cast<GEOMETRY_TYPE>(std::stoi(split.at(0)));
			if (!(type > GEOMETRY_TYPE::GEO_TILES_START && type < GEOMETRY_TYPE::GEO_TILES_END))
			{
				DEBUG_MSG("Not a valid Tile! GEOMETRY_TYPE ID: " << type << " is out of range of tiles selection");
				continue;
			}

			//Find Position
			std::vector<std::string> posSplit = splitStr(split.at(1), ',');
			if (posSplit.size() < 3)
			{
				DEBUG_MSG("Position " << split.at(1) << " attribute has a broken format in configuration. Skipping...");
				continue;
			}
			pos = Vector3(std::stoi(posSplit.at(0)), std::stoi(posSplit.at(1)), std::stoi(posSplit.at(2)));

			//Find Rotation
			std::vector<std::string> rotSplit = splitStr(split.at(2), ',');
			if (rotSplit.size() < 3)
			{
				DEBUG_MSG("Normal " << split.at(2) << " attribute has a broken format in configuration. Skipping...");
				continue;
			}
			normal = Vector3(std::stoi(rotSplit.at(0)), std::stoi(rotSplit.at(1)), std::stoi(rotSplit.at(2)));

			//Find Scale
			std::vector<std::string> scaleSplit = splitStr(split.at(3), ',');
			if (scaleSplit.size() < 3)
			{
				DEBUG_MSG("Scale " << split.at(3) << " attribute has a broken format in configuration. Skipping...");
				continue;
			}
			scale = Vector3(std::stoi(scaleSplit.at(0)), std::stoi(scaleSplit.at(1)), std::stoi(scaleSplit.at(2)));
			if (scale.x <= 0 || scale.y <= 0 || scale.z <= 0)
			{
				DEBUG_MSG("Scale " << split.at(3) << " attribute cannot have values less than or equal to 0. Skipping...");
				continue;
			}

			GameObject* obj = new GameObject(GameObject::GO_TILE, meshList[type]);
			obj->pos = pos;
			obj->physics->SetNormal(normal);
			obj->scale = scale;
			gridObjects.push_back(obj);


		}
		

	}
	DEBUG_MSG("Loaded Level " << filename << " with " << gridObjects.size() << " Tiles");
	return true;
}




void LevelEditor::SaveMap()
{

}

void LevelEditor::ClearMap()
{

}

void LevelEditor::RemoveGO(GameObject* go)
{

}

void LevelEditor::Update(double dt)
{
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
		bLButtonState = true; //Down
	else if (bLButtonState && !Application::IsMousePressed(0))
		bLButtonState = false; //Up
	
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
		bRButtonState = true; //Down
	else if(bRButtonState && !Application::IsMousePressed(1))
		bRButtonState = false; //Up
	
	static bool bMButtonState = false;
	if (!bMButtonState && Application::IsMousePressed(2))
		bMButtonState = true; //Down
	else if (bMButtonState && !Application::IsMousePressed(2))
		bMButtonState = false; //Up
	
	
}

void LevelEditor::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	//	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projection.SetToPerspective(m_worldWidth - 1.0f, m_worldWidth / m_worldHeight, 0.1f, 1000.f);
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

	//RenderMesh(meshList[GEO_AXES], false);

	// all gos
	for (std::vector<GameObject*>::iterator it = gridObjects.begin(); it != gridObjects.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		float angle = Math::RadianToDegree(atan2(go->physics->GetNormal().y, go->physics->GetNormal().x));
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(angle, 0, 0, 1);

		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(go->mesh, false);
		modelStack.PopMatrix();
	}

	for (int x = camera.position.x - m_worldWidth * 0.5 - 2; x < camera.position.x + m_worldWidth * 0.5 + 2; ++x)
	{
		for (int y = camera.position.y - m_worldHeight * 0.5 - 2; y < camera.position.y + m_worldHeight * 0.5 + 2; ++y)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x, y, 1);

			modelStack.Scale(gridLength, gridHeight, 1);
			RenderMesh(meshList[GEO_TILEGRID], false);
			modelStack.PopMatrix();
		}
	}



	// fps tings

	std::ostringstream ss;
	ss.str("");
	ss.precision(4);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 3);


	double x,y;
	CursorPosition(x, y);
	ss.str("");
	ss.precision(3);
	ss << "Cursor Position: (" << x << ", " << y << ")";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 64, 97);

	RenderTextOnScreen(meshList[GEO_TEXT], mapName, Color(1, 1, 1), 3, 0, 0);
}

void LevelEditor::Exit()
{

}

//Utilities

void LevelEditor::CursorPosition(double& theX, double& theY)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	// convert to world space
	x /= (w / m_worldWidth);
	y = h - y;
	y /= (h / m_worldHeight);
	theX = x;
	theY = y;
}

std::vector<std::string> splitStr(std::string str, char delimiter) {
	std::vector<std::string> internal;
	std::stringstream ss(str); // Turn the string into a stream. 
	std::string tok;

	while (std::getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}