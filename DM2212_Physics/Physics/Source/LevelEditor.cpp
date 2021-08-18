#include "LevelEditor.h"
#include "GL\glew.h"
#include "Application.h"
#include <fstream>
#include <sstream>   

std::vector<std::string> splitStr(std::string str, char delimiter);

LevelEditor::LevelEditor() : heldOnTo(nullptr)
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
	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_worldHeight = m_screenHeight * 3;
	m_worldWidth = m_screenWidth * 5;

	camera.Init(Vector3(0,0, 1), Vector3(0,0, 0), Vector3(0, 1, 0));
	camera.SetLimits(m_screenWidth, m_screenHeight, m_worldWidth, m_worldHeight);
	//camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
	std::string mapToLoad = "LEVEL_1.txt";
	LoadMap(mapToLoad);
}

bool LevelEditor::LoadMap(std::string filename)
{
	std::string path = file_path + filename;
	mapName = filename;
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
			std::vector<std::string> split = splitStr(std::string(buf), ' ');
			gridLength = std::stod(split.at(1));
		}

		key = "GridHeight:";
		if (strncmp(key.c_str(), buf, key.size()) == 0)
		{
			std::vector<std::string> split = splitStr(std::string(buf), ' ');
			gridHeight = std::stod(split.at(1));
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

			GameObject* obj = new GameObject(GameObject::GO_TILE, meshList[type], type);
			obj->pos = pos;
			obj->physics->SetNormal(normal);
			obj->scale = scale * gridLength;
			obj->scale.x *= tileSize[type]->gridLength;
			obj->scale.y *= tileSize[type]->gridHeight;
			gridObjects.push_back(obj);


		}
		

	}
	DEBUG_MSG("Loaded Level " << filename << " with " << gridObjects.size() << " Tiles");
	return true;
}




void LevelEditor::SaveMap()
{
	if (unsavedChanges)
	{
		unsavedChanges = false;
		DEBUG_MSG("Saving " << mapName << ".txt ...");

		std::ofstream file("Levels\\" + mapName);
		file << "# Format: \"TILE_ID Enum Val\":\"Position\":\"Rotation\":\"Scale\"" << std::endl;
		file << "GridLength: " << gridLength << std::endl;
		file << "GridHeight: " << gridHeight << std::endl;
		file << "Tiles: " << std::endl;

		for (auto& go : gridObjects)
		{
			file << go->geoTypeID << ":" <<
				go->pos.x << "," << go->pos.y << "," << go->pos.z << ":" <<
				go->physics->GetNormal().x << "," << go->physics->GetNormal().y << "," << go->physics->GetNormal().z << ":" <<
				go->scale.x / gridLength << "," << go->scale.y / gridLength << "," << go->scale.z / gridLength << std::endl;
		}

		DEBUG_MSG("Saved " << mapName);
	}
}

void LevelEditor::ClearMap()
{

}

void LevelEditor::RemoveGO(GameObject* go)
{

}

void LevelEditor::Update(double dt)
{
	camera.Update(camera.position, dt);
	fps = 1 / dt;
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

	static bool bCTRLState = false;
	bool CTRLKeyRelease = false;
	if (!bCTRLState && Application::IsKeyPressed(VK_CONTROL))
		bCTRLState = true; //Down
	else if (bCTRLState && !Application::IsKeyPressed(VK_CONTROL))
	{
		bCTRLState = false; //Up
		CTRLKeyRelease = true;
	}
	
	for (std::vector<GameObject*>::iterator it = gridObjects.begin(); it != gridObjects.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		double curs_tw_X, curs_tw_Y;
		CursorToWorldPosition(curs_tw_X, curs_tw_Y);
		//DEBUG_MSG("curs_tw_X: " <<  curs_tw_X << " curs_tw_Y: " <<  curs_tw_Y << " || " << go->pos.x << "x " << go->pos.y << "y");
		//curs_tw_or Collided With Object
		if (PosCollidedWithGO(curs_tw_X, curs_tw_Y, go))
		{
			unsavedChanges = true;
			if (heldOnTo == nullptr && bLButtonState)
			{
				heldOnTo = go;
			}
			else if (heldOnTo != nullptr && !bLButtonState)
			{
				if(GetCollidedGOs(heldOnTo->pos.x, heldOnTo->pos.y).size() == 1)
					heldOnTo = nullptr;
			}

		}
	}
	double curs_tw_X, curs_tw_Y;
	CursorToWorldPosition(curs_tw_X, curs_tw_Y);


	if (heldOnTo != nullptr)
	{
		double snap_X, snap_Y;
		double gridDiameter_X = (gridLength) * 2;
		double gridDiameter_Y = (gridHeight) * 2;
		snap_X = floor((curs_tw_X) / gridDiameter_X) * gridDiameter_X;
		snap_Y = floor((curs_tw_Y) / gridDiameter_Y) * gridDiameter_Y;
		if (heldOnTo->scale.y > 1.0)
		{
			snap_Y += (heldOnTo->scale.y - 1) * 0.5;
		}
		else if (heldOnTo->scale.x > 1.0)
		{
			snap_X += (heldOnTo->scale.x - 1) * 0.5;
		}

		unsavedChanges = true;
		if (!CursorWithinScreen())
		{
			heldOnTo = nullptr;
		}
		else 
		{
			if (snapPosToGrid)
			{
				heldOnTo->pos.x = snap_X;
				heldOnTo->pos.y = snap_Y;
			}
			else
			{
				heldOnTo->pos.x = curs_tw_X;
				heldOnTo->pos.y = curs_tw_Y;
			}
			static bool pastedOnce = false;
			if (bCTRLState && heldOnTo != nullptr) //Holding Down control
			{
				if (GetCollidedGOs(heldOnTo->pos.x, heldOnTo->pos.y).size() == 1)
				{
					gridObjects.push_back(heldOnTo->Clone());
					pastedOnce = true;
				}
			}
			else if (!bCTRLState && !bLButtonState && pastedOnce)
			{
				pastedOnce = false;
				//Delete HeldOnTo
				for (auto& go : gridObjects)
				{
					if (go == heldOnTo)
					{
						delete go;
						go = nullptr;
					}
				}
				heldOnTo = nullptr;
				gridObjects.erase(std::remove(gridObjects.begin(), gridObjects.end(), nullptr), gridObjects.end());
			}
		}
		if (bRButtonState)
		{
			for(auto& go : gridObjects)
			{
				if (go != heldOnTo && PosCollidedWithGO(heldOnTo->pos.x, heldOnTo->pos.y, go))
				{
					delete go;
					go = nullptr;
				}
			}
			gridObjects.erase(std::remove(gridObjects.begin(), gridObjects.end(), nullptr), gridObjects.end());
			
		}
	}
	else
	{
		if (bRButtonState)
		{
			for (auto& go : gridObjects)
			{
				double curs_tw_X, curs_tw_Y;
				CursorToWorldPosition(curs_tw_X, curs_tw_Y);
				if (PosCollidedWithGO(curs_tw_X, curs_tw_Y, go))
				{
					unsavedChanges = true;
					delete go;
					go = nullptr;
				}
			}
			gridObjects.erase(std::remove(gridObjects.begin(), gridObjects.end(), nullptr), gridObjects.end());

		}
	}

	if (bCTRLState && Application::IsKeyPressed('S'))
	{
		if(unsavedChanges)
			SaveMap();
	}

	//DEBUG_MSG("Cursor within screen? " << CursorWithinScreen());
}

std::vector<GameObject*> LevelEditor::GetCollidedGOs(double worldPos_X, double worldPos_Y)
{
	std::vector<GameObject*> coll;
	for (std::vector<GameObject*>::iterator it = gridObjects.begin(); it != gridObjects.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (PosCollidedWithGO(worldPos_X, worldPos_Y, go))
			coll.push_back(go);
	}
	return coll;
}

bool LevelEditor::PosCollidedWithGO(double worldPos_X, double worldPos_Y, GameObject* go)
{
	return (worldPos_X > go->pos.x - go->scale.x && worldPos_X <  go->pos.x + go->scale.x
		&& worldPos_Y > go->pos.y - go->scale.y && worldPos_Y < go->pos.y + go->scale.y);
}

void LevelEditor::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(-1 * m_screenWidth * 0.5f, m_screenWidth * 0.5f, -1 * m_screenHeight * 0.5f, m_screenHeight * 0.5f, -10, 10);

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

	int loops = 0;
	double xAdd = gridLength * 2.0;
	double yAdd = gridHeight * 2.0;
	int startX = floor((camera.position.x - (m_screenWidth * 0.5 - 2)) / xAdd) * xAdd;
	int startY = floor((camera.position.y - (m_screenWidth * 0.5 - 2)) / yAdd) * yAdd;
	for (int x = startX; x < camera.position.x + (m_screenWidth * 0.5 + 2); x += xAdd)
	{
		for (int y = startY; y < camera.position.y + (m_screenHeight * 0.5 + 2); y += yAdd)
		{
			loops++;
			modelStack.PushMatrix();
			modelStack.Translate(x, y, 0);
			modelStack.Rotate(0, 0, 0, 1);
			modelStack.Scale(gridLength, gridHeight, 1);
			RenderMesh(meshList[GEO_TILEGRID], false);
			modelStack.PopMatrix();
		}
	}
	//DEBUG_MSG("Looped " << loops << " to cover all grids in viewable scene");



	// fps tings

	std::ostringstream ss;
	ss.str("");
	ss.precision(4);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 3);


	double x,y;
	CursorToWorldPosition(x, y);
	ss.str("");
	ss.precision(3);
	ss << "Cursor Position: (" << x << ", " << y << ")";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 1), 3, 64, 97);

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

void LevelEditor::CursorToWorldPosition(double& theX, double& theY)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	// convert to world space
	x /= (w / m_screenWidth);
	y = h - y;
	y /= (h / m_screenHeight);
	x -= m_screenWidth * 0.5 - camera.position.x;
	y -= m_screenHeight * 0.5 - camera.position.y;

	theX = x;
	theY = y;
}

bool LevelEditor::CursorWithinScreen()
{
	double x, y;
	Application::GetCursorPos(&x, &y);

	if (x < 0 || x > Application::GetWindowWidth()
		|| y < 0 || y > Application::GetWindowHeight())
		return false;
	return true;

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