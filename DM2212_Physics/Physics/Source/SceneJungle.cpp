#include "SceneJungle.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"
#include "../Source/SoundController/SoundController.h"
#include <sstream>
#include "LevelLoader.h"
#include "Utility.h"

#include "Debug.h"

//Entity Includes
#include "Player.h"

#include "Coin.h"

//...

SceneJungle::SceneJungle()
{
	goManager = new GameObjectManager();
	inventory = new Inventory();
}

SceneJungle::~SceneJungle()
{
	input = NULL;
}

void SceneJungle::Init()
{
	SceneBase::Init();
	goManager->Init();

	// Calculating aspect ratio
	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_worldHeight = m_screenHeight * 2;
	m_worldWidth = m_screenWidth * 15;

	dialogueManager = DialogueManager::GetInstance();

	//Inventory init
	inventory->Init(this);

	//Physics code here
	m_speed = 1.f;
	Math::InitRNG();

	//Store keyboard instance
	input = Input::GetInstance();

	// Game Manager
	gameManager = GameManager::GetInstance();

	// Unique Meshes
	meshList[GEO_BG] = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	meshList[GEO_BG]->textureID = LoadTGA("Image/bg_jungle2.tga");

	//Level Loading
	std::vector<GameObject*> tiles;
	if(LevelLoader::GetInstance()->LoadTiles("JUNGLE_1_1", this->meshList, this->tileSize, tiles, gridLength, gridHeight))
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
			player->Init(&camera, Player::PLATFORMER, goManager, inventory);

			player->AddBottomSprite();
			player->bottomSprite->mesh = meshList[GEO_WALL];
			goManager->AddGO(player);


			DEBUG_MSG("From Phy Editor: " << player->scale);
			

			//Delete Grid Player
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_JUNGLE_MONKEY)
		{
			Monkey* monkey = new Monkey();

			monkey->active = true;
			monkey->scale = go->scale;
			monkey->pos = go->pos;
			monkey->physics = go->physics->Clone();
			monkey->physics->SetInelasticity(0.99f);
			monkey->physics->SetIsBouncable(false);
			monkey->physics->SetGravity(Vector3(0, 0, 0));
			monkey->Init(this, goManager, inventory, player->pos, new BulletSpawner(goManager, new BananaBullet(Vector3(2, 2, 2), monkey, 30)));

			monkey->AddBottomSprite();
			monkey->bottomSprite->mesh = meshList[GEO_WALL];
			goManager->AddGO(monkey);

			//Delete Grid monkey
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_COIN)
		{
			Coin* coin = new Coin(1);
			coin->active = true;
			coin->scale = go->scale * 0.85;
			coin->pos = go->pos;
			coin->physics = go->physics->Clone();
			coin->Init();
			goManager->AddGO(coin);

			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_JUNGLE_CAMPFIRE)
		{
			Campfire* campfire = new Campfire();

			campfire->active = true;
			campfire->scale = go->scale;
			campfire->pos = go->pos;
			campfire->geoTypeID = GEOMETRY_TYPE::GEO_JUNGLE_CAMPFIRE;
			campfire->physics = go->physics->Clone();
			campfire->physics->SetInelasticity(0.99f);
			campfire->physics->SetIsBouncable(false);
			campfire->physics->SetGravity(Vector3(0, 0, 0));
			campfire->Init(this, inventory, player->pos);

			goManager->AddGO(campfire);

			//Delete Grid campfire
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_JUNGLE_PROWLER)
		{
			Prowler* prowler = new Prowler();

			prowler->active = true;
			prowler->scale = go->scale;
			prowler->pos = go->pos;
			prowler->physics = go->physics->Clone();
			prowler->physics->SetInelasticity(0.99f);
			prowler->physics->SetIsBouncable(false);
			prowler->physics->SetGravity(Vector3(0, 0, 0));
			prowler->Init(this, inventory, player, goManager);

			goManager->AddGO(prowler);

			//Delete Grid campfire
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_COIN)
		{
			Coin* coin = new Coin(1);
			coin->active = true;
			coin->scale = go->scale * 0.85;
			coin->pos = go->pos;
			coin->physics = go->physics->Clone();
			coin->Init();
			goManager->AddGO(coin);
			delete go;
			go = nullptr;
		}
	}
	tiles.erase(std::remove(tiles.begin(), tiles.end(), nullptr), tiles.end());
	
	//Add all remainding tiles
	goManager->AddAllGO(tiles);
	
	//Camera init
	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.SetLimits(m_screenWidth, m_screenHeight, m_worldWidth, m_worldHeight);
	camera.SetFocusTarget(player->pos);
	camera.SetMode(Camera::CENTER);

	// ABILITIES
	/*DashAbility* ability = new DashAbility(meshList[GEO_ABILITYICON_DASH]);
	ability->SetCamera(&camera);
	ability->SetScenePointer(this);

	GrapplingAbility* ability2 = new GrapplingAbility(inventory, meshList[GEO_ABILITYICON_GRAPPLINGHOOK]);
	ability2->SetCamera(&camera);
	ability2->SetScenePointer(this);
	ability2->SetGOManager(this->goManager);*/

	gameManager->initAbilities(this, &camera, goManager, player);
	player->SetAbilities(gameManager->getCurrAbility(1), gameManager->getCurrAbility(2));
	CSoundController::GetInstance()->PlaySoundByID(SOUND_TYPE::BG_JUNGLE, 1, 0.5);
}

void SceneJungle::Update(double dt)
{
	SceneBase::Update(dt);
	inventory->Update(dt);
	camera.Update(player->pos, dt);

	//play dialogue
	if (!playedDialogue)
	{
		player->physics->SetEnableUpdate(false);
		dialogueManager->AddDialogue(PLAYER, "Wow, what a change in environment!", LEFT, 3.0f);
		dialogueManager->AddDialogue(PLAYER, "Let's explore this jungle :D", LEFT, 3.0f);
		playedDialogue = true;
	}
	else
		player->physics->SetEnableUpdate(true);

	goManager->Update(dt, &this->camera);

	if (player->currentHP <= 0)
	{
		gameLost = true;
	}
	if (gameManager->getMachineStatus(2))
	{
		gameWin = true;
	}
}

void SceneJungle::Render()
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

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
		//std::cout << "// DIRECTIONAL" << std::endl;
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
		//std::cout << "// POINT" << std::endl;
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

	if (inventory->GetCurrentItem())
	{
		std::stringstream ss;
		ss << "curr item: " << inventory->GetCurrentItemType();

		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.0f, 1.0f, 1.0f), 4, 10, 10);
	}

	// BG
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, -0.01);
	modelStack.Scale(m_screenWidth, m_screenHeight, 1);
	RenderMesh(meshList[GEO_BG], true);
	modelStack.PopMatrix();

	inventory->Render();
	goManager->Render(this);
}

void SceneJungle::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	goManager->Exit();
	inventory->Clear();
	CSoundController::GetInstance()->StopPlayingSoundByID(SOUND_TYPE::BG_JUNGLE);
}

void SceneJungle::InitLights()
{
	lights[0].type = Light::LIGHT_POINT;
	lights[0].position.Set(player->pos.x, player->pos.y, player->pos.z + 10);
	lights[0].color.Set(0.702, 0.529, 1);
	lights[0].power = 2;
	lights[0].defaultPower = lights[0].power;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 0.f, 1.f);

	lights[1].type = Light::LIGHT_SPOT;
	lights[1].position.Set(0, 0, 1);
	lights[1].color.Set(0.8, 0.8, 1);
	lights[1].power = 0;
	lights[1].defaultPower = 2;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 0.f, 1.f);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	bLightEnabled = false;
}
