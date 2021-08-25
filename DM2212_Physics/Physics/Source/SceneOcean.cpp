#include "SceneOcean.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"
#include <sstream>
#include "LevelLoader.h"

#include "Debug.h"

//Entity Includes
#include "Player.h"

//...

SceneOcean::SceneOcean()
{
	goManager = new GameObjectManager();
}

SceneOcean::~SceneOcean()
{
	input = NULL;
}

void SceneOcean::Init()
{
	SceneBase::Init();
	goManager->Init();

	// Calculating aspect ratio
	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_worldHeight = m_screenHeight * 3;
	m_worldWidth = m_screenWidth * 5;

	//Camera init
	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.SetLimits(m_screenWidth, m_screenHeight, m_worldWidth, m_worldHeight);

	//Inventory init
	inventory = new Inventory();
	inventory->Init(this);

	//Physics code here
	m_speed = 1.f;
	Math::InitRNG();

	//Store keyboard instance
	input = Input::GetInstance();

	// Unique Meshes
	meshList[GEO_BG] = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	meshList[GEO_BG]->textureID = LoadTGA("Image/bg_aboveOcean.tga");

	//Level Loading
	std::vector<GameObject*> tiles;
	if(LevelLoader::GetInstance()->LoadTiles("OCEAN_1_1", this->meshList, this->tileSize, tiles, gridLength, gridHeight))
		DEBUG_MSG("Level Did not load successfully");
	for (auto& go : tiles)
	{
		if (go->geoTypeID == GEOMETRY_TYPE::GEO_PLAYER_GIRL1)
		{
			player = new Player();
			player->active = true;
			player->scale = go->scale;
			player->pos = go->pos;
			player->physics = go->physics->Clone();
			player->physics->SetInelasticity(0.99f);
			player->physics->SetIsBouncable(false);
			player->Init(Player::PLATFORMER, goManager, inventory);

			player->AddBottomSprite();
			player->bottomSprite->mesh = meshList[GEO_WALL];
			goManager->AddGO(player);


			DEBUG_MSG("From Phy Editor: " << player->scale);
			

			//Delete Grid Player
			delete go;
			go = nullptr;
		}
	}
	tiles.erase(std::remove(tiles.begin(), tiles.end(), nullptr), tiles.end());
	
	//Add all remainding tiles
	goManager->AddAllGO(tiles);
	
	// ABILITIES

	player->SetAbilities(nullptr, nullptr); 
	Trident* trident = new Trident;
	trident->Init(&camera, goManager, player->pos);
	inventory->AddItem(trident);
}

void SceneOcean::Update(double dt)
{
	SceneBase::Update(dt);
	inventory->Update(dt);
	camera.Update(player->pos, dt);


	if(input->IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(input->IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	goManager->Update(dt);

	if (player->currentHP <= 0)
	{
		gameLost = true;
	}
}

void SceneOcean::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Projection matrix : Orthographic Projection
	Mtx44 projection;
//	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
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

	if (inventory->GetCurrentItem())
	{
		std::stringstream ss;
		ss << "curr item: " << inventory->GetCurrentItemType();

		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.0f, 1.0f, 1.0f), 4, 10, 10);
	}

	inventory->Render();

	// BG
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, -0.01);
	modelStack.Scale(m_screenWidth, m_screenHeight, 1);
	RenderMesh(meshList[GEO_BG], true);
	modelStack.PopMatrix();

	goManager->Render(this);
}

void SceneOcean::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	goManager->Exit();
}
