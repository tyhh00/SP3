#include "LevelEditor.h"
#include "GL\glew.h"
#include "Application.h"
#include "Input.h"
#include <fstream>
#include <sstream>

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

	std::string mapToLoad = "JUNGLE_1_1";

	// Calculating aspect ratio
	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_worldHeight = m_screenHeight;
	m_worldWidth = m_screenWidth;

	stackOnGrid = true;

	snapPosToGrid = true;
	snapRotToGrid = true;

	canScrollIn = scrollingSpeed;
	scrolledGeo = static_cast<GEOMETRY_TYPE>(GEOMETRY_TYPE::GEO_TILES_START + 1);

	camera.Init(Vector3(m_screenWidth * 0.5, m_screenHeight * 0.5, 1), Vector3(m_screenWidth * 0.5, m_screenHeight * 0.5, 0), Vector3(0, 1, 0));
	camera.SetLimits(m_screenWidth, m_screenHeight, m_worldWidth, m_worldHeight);
	//camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	LoadMap(mapToLoad);
}

bool LevelEditor::LoadMap(std::string filename)
{
	mapName = filename;
	
	return (LevelLoader::GetInstance()->LoadTiles(filename, this->meshList, this->tileSize, gridObjects, gridLength, gridHeight));
}




void LevelEditor::SaveMap()
{
	if (unsavedChanges)
	{
		unsavedChanges = false;
		DEBUG_MSG("Saving " << mapName << ".txt ...");

		std::ofstream file("Levels\\" + mapName + ".txt");
		file << "# Format: \"TILE_ID Enum Val\":\"Position\":\"Rotation\":\"Scale\"" << std::endl;
		file << "GridLength: " << gridLength << std::endl;
		file << "GridHeight: " << gridHeight << std::endl;
		file << "Tiles: " << std::endl;

		for (auto& go : gridObjects)
		{
			file << go->geoTypeID << ":" <<
				go->pos.x << "," << go->pos.y << "," << go->pos.z << ":" <<
				go->physics->GetNormal().x << "," << go->physics->GetNormal().y << "," << go->physics->GetNormal().z << ":" <<
				go->scale.x / (tileSize[go->geoTypeID]->gridLength * gridLength) << "," << go->scale.y / (tileSize[go->geoTypeID]->gridHeight  * gridHeight) << "," << go->scale.z / gridLength << std::endl;
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
	SceneBase::Update(dt);
	camera.Update(dt);
	fps = 1 / dt;

	canScrollIn -= dt;
	if (canScrollIn < 0.0)
		canScrollIn = 0.0;
	canSnapRotateIn -= dt;
	if (canSnapRotateIn < 0.0)
		canSnapRotateIn = 0.0;

	bool LKeyRelease = false;
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
		bLButtonState = true; //Down
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false; //Up
		LKeyRelease = true;
	}

	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
		bRButtonState = true; //Down
	else if(bRButtonState && !Application::IsMousePressed(1))
		bRButtonState = false; //Up

	static bool bCTRLState = false;
	bool CTRLKeyRelease = false;
	if (!bCTRLState && Application::IsKeyPressed(VK_CONTROL))
		bCTRLState = true; //Down
	else if (bCTRLState && !Application::IsKeyPressed(VK_CONTROL))
	{
		bCTRLState = false; //Up
		CTRLKeyRelease = true;
	}

	scrollState = SCROLLER_GEOSWITCHER;
	if (Application::IsKeyPressed('S'))
		scrollState = SCROLLER_SCALE;
	else if (Application::IsKeyPressed('R'))
		scrollState = SCROLLER_ROTATE;
	else if (Application::IsKeyPressed('T'))
		scrollState = SCROLLER_TRANSLATE;

	if(Input::GetInstance()->IsKeyReleased('1'))
	{
		snapPosToGrid = !snapPosToGrid;
	}

	else if (Input::GetInstance()->IsKeyReleased('2'))
	{
		snapRotToGrid = !snapRotToGrid;
	}

	static bool cannotPasteYet = true; //after pressing Left-Click, you must let go of left click once before u can start placing blocks
	static bool pastedOnce = false;
	
	for (std::vector<GameObject*>::iterator it = gridObjects.begin(); it != gridObjects.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		go->Update(dt);
		go->physics->Update(dt);
		double curs_tw_X, curs_tw_Y;
		CursorToWorldPosition(curs_tw_X, curs_tw_Y);
		//DEBUG_MSG("curs_tw_X: " <<  curs_tw_X << " curs_tw_Y: " <<  curs_tw_Y << " || " << go->pos.x << "x " << go->pos.y << "y");
		//curs_tw_or Collided With Object
		if (PosCollidedWithGO(curs_tw_X, curs_tw_Y, go))
		{
			if (heldOnTo == nullptr && bLButtonState)
			{
				heldOnTo = go;
				cannotPasteYet = true;
			}
			else if (heldOnTo != nullptr && !bLButtonState && !bCTRLState)
			{
				std::vector<GameObject*> colls = GetCollidedGOs(heldOnTo->pos.x, heldOnTo->pos.y);
				bool dup = false;
				for (auto& go : colls)
				{
					if (go->geoTypeID == heldOnTo->geoTypeID && go != heldOnTo) {
						dup = true;
						break;
					}
					else if (heldOnTo != go && go->pos == heldOnTo->pos && go->scale == heldOnTo->scale)
					{
						dup = true;
						break;
					}
				}
				if (!dup)
				{
					heldOnTo = nullptr;
					unsavedChanges = true;
				}
			}

		}
	}
	double curs_tw_X, curs_tw_Y;
	CursorToWorldPosition(curs_tw_X, curs_tw_Y);
	
	//Enable Pasting
	if (bCTRLState && LKeyRelease && cannotPasteYet)
	{
		cannotPasteYet = false;
	}

	//Scroll System
	if (bCTRLState && Application::mouseScrollingUp > 0)
	{
		int scrollVal = 0;
		if (Application::mouseScrollingUp == 1)
			scrollVal -= 1;
		else if (Application::mouseScrollingUp == 2)
			scrollVal += 1;

		switch (scrollState)
		{
		case SCROLLER_GEOSWITCHER:
		{
			cannotPasteYet = true;
			if (canScrollIn <= 0.0)
			{
				canScrollIn = scrollingSpeed;
				scrolledGeo = static_cast<GEOMETRY_TYPE>(scrolledGeo + scrollVal);
				if (scrolledGeo >= GEOMETRY_TYPE::GEO_TILES_END)
					scrolledGeo = static_cast<GEOMETRY_TYPE>(GEOMETRY_TYPE::GEO_TILES_START + 2);
				if (scrolledGeo <= GEOMETRY_TYPE::GEO_TILES_START)
					scrolledGeo = static_cast<GEOMETRY_TYPE>(GEOMETRY_TYPE::GEO_TILES_END - 1);
			}
			//Delete heldOnTo if it exists & GEO Type != new scrolledGeo
			if (heldOnTo != nullptr)
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
			GameObject* obj = new GameObject(GameObject::GO_TILE, meshList[scrolledGeo], scrolledGeo);
			obj->pos = Vector3(curs_tw_X, curs_tw_Y);
			obj->physics->SetNormal(Vector3(1, 0, 0));
			obj->scale = Vector3(1, 1, 1) * gridLength;
			obj->scale.x *= tileSize[scrolledGeo]->gridLength;
			obj->scale.y *= tileSize[scrolledGeo]->gridHeight;
			heldOnTo = obj;
			gridObjects.push_back(heldOnTo);
			break;
		}

		case SCROLLER_TRANSLATE:
			if (heldOnTo != nullptr)
			{
				cannotPasteYet = true;
				heldOnTo->pos += heldOnTo->physics->GetNormal() * (scrollVal);
			}
			break;
		
		case SCROLLER_ROTATE:
			if (heldOnTo != nullptr)
			{
				cannotPasteYet = true;
				float angle = atan2(heldOnTo->physics->GetNormal().y, heldOnTo->physics->GetNormal().x);
				if (snapRotToGrid && canSnapRotateIn <= 0)
				{
					angle += Math::DegreeToRadian(scrollVal);
				}
				else
					angle += (scrollVal) * dt * 2.5;
				heldOnTo->physics->SetNormal(Vector3(cosf(angle), sinf(angle), 0));
			}
			break;

		case SCROLLER_SCALE:
			if (heldOnTo != nullptr)
			{
				heldOnTo->scale += heldOnTo->scale * (scrollVal)*dt;
				if (heldOnTo->scale.x < 0.01)
					heldOnTo->scale.x = 0.01;
				if (heldOnTo->scale.y < 0.01)
					heldOnTo->scale.y = 0.01;
				if (heldOnTo->scale.z < 0.01)
					heldOnTo->scale.z = 0.01;
			}
			break;


		}
		

	}

	if (heldOnTo != nullptr)
	{
		double snap_X, snap_Y;
		double gridDiameter_X = (gridLength) * 2;
		double gridDiameter_Y = (gridHeight) * 2;
		snap_X = floor((curs_tw_X) / gridDiameter_X) * gridDiameter_X;
		snap_Y = floor((curs_tw_Y) / gridDiameter_Y) * gridDiameter_Y;

		if (snapRotToGrid)
		{
			float angle = Math::RadianToDegree(atan2(heldOnTo->physics->GetNormal().y, heldOnTo->physics->GetNormal().x));
			DEBUG_MSG("PREANGLE" << angle);
			angle = round(angle);
			heldOnTo->physics->SetNormal(Vector3(cosf(Math::DegreeToRadian(angle)), sinf(Math::DegreeToRadian(angle))));
			DEBUG_MSG("POST" << angle);

		}

		if (heldOnTo->scale.y > gridHeight)
		{
			snap_Y += (heldOnTo->scale.y - gridHeight) * 0.5;
		}
		else if (heldOnTo->scale.x > gridLength)
		{
			snap_X += (heldOnTo->scale.x - gridLength) * 0.5;
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
			if (bCTRLState && heldOnTo != nullptr && bLButtonState) //Holding Down control
			{
				std::vector<GameObject*> colls = GetCollidedGOs(heldOnTo->pos.x, heldOnTo->pos.y);
				bool dup = false;
				int count = 0;
				heldOnTo->pos.z = 0;
				for (auto& go : colls)
				{
					if (go != heldOnTo)
					{
						if (go->geoTypeID == heldOnTo->geoTypeID) {
							dup = true;
							break;
						}
						else
						{
							heldOnTo->pos.z = count++;
						}
					}
				}

				if (!dup && !cannotPasteYet)
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

	if (bCTRLState && Input::GetInstance()->IsKeyPressed('S'))
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
	CursorPosition(x, y);
	ss.str("");
	ss.precision(3);
	ss << "Cursor Position: (" << x << ", " << y << ")";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 97);

	double wX, wY;
	CursorToWorldPosition(wX, wY);
	ss.str("");
	ss.precision(3);
	ss << "World Position: (" << wX << ", " << wY << ")";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 94);

	ss.str("");
	ss.precision(3);
	ss << "Editor: ";
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 91);

	ss.str("");
	ss.precision(3);
	ss << "Changes " << (unsavedChanges ? "UNSAVED":"SAVED");
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color((unsavedChanges ? 1 : 0), (unsavedChanges ? 0 : 1), 0), 3, 0,88);

	ss.str("");
	ss.precision(3);
	ss << "Snap to Grid " << (snapPosToGrid ? "ENABLED" : "Disabled");
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color((snapPosToGrid ? 0 : 1), (snapPosToGrid ? 1 : 0), 0), 3, 0, 85);

	ss.str("");
	ss.precision(3);
	ss << "Snap Rotation " << (snapRotToGrid? "ENABLED" : "Disabled");
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color((snapRotToGrid ? 0 : 1), (snapRotToGrid ? 1 : 0), 0), 3, 0, 82);

	//int line = 88;

	//line -= 3;
	//ss.str("");
	//ss.precision(3);
	//ss << "Left-Click to select block";
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, line);

	//line -= 3;
	//ss.str("");
	//ss.precision(3);
	//ss << "CTRL + Left-Click to paste selected block";
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, line);

	//line -= 3;
	//ss.str("");
	//ss.precision(3);
	//ss << "CTRL + Scroll to paste scroll through all types of blocks";
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, line);

	//line -= 3;
	//ss.str("");
	//ss.precision(3);
	//ss << "Let go of CTRL to unselect tile";
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, line);

	//line -= 3;
	//ss.str("");
	//ss.precision(3);
	//ss << "CTRL + S to save";
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, line);

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