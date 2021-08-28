#include "SceneGraveyard.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"
#include <sstream>
#include "LevelLoader.h"
#include "Utility.h"
#include "Debug.h"
#include "SoundController/SoundController.h"

//Entity Includes
#include "Player.h"
#include "Ghost.h"
#include "Tumbleweed.h"
#include "GrimReaper.h"
#include "Tombstone.h"

//...

SceneGraveyard::SceneGraveyard()
{
	goManager = new GameObjectManager();
	inventory = new Inventory();
}

SceneGraveyard::~SceneGraveyard()
{
	input = NULL;
	if (goManager)
	{
		delete goManager;
		goManager = NULL;
	}
	if (inventory)
	{
		delete inventory;
		inventory = NULL;
	}
}

void SceneGraveyard::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_worldHeight = 143;
	m_worldWidth = 1000;

	//Physics code here
	m_speed = 1.f;
	Math::InitRNG();

	// GO Manager
	goManager->Init();
	// Inventory 
	inventory->Init(this);
	// Dialogue Manager
	dialogueManager = DialogueManager::GetInstance();
	//Store keyboard instance
	input = Input::GetInstance();
	// Game Manager
	gameManager = GameManager::GetInstance();
	// Unique Meshes
	meshList[GEO_BG] = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	meshList[GEO_BG]->textureID = LoadTGA("Image/GraveyardBG.tga");

	//Level Loading
	std::vector<GameObject*> tiles;
	if(LevelLoader::GetInstance()->LoadTiles("GRAVEYARD_1_1", this->meshList, this->tileSize, tiles, gridLength, gridHeight))
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
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_ENEMY_GHOST)
		{
			Ghost* ghost = new Ghost();

			ghost->active = true;
			ghost->scale = go->scale;
			ghost->pos = go->pos;
			ghost->geoTypeID = go->geoTypeID;
			ghost->physics = go->physics->Clone();
			ghost->physics->SetInelasticity(0.99f);
			ghost->physics->SetIsBouncable(false);
			ghost->Init(this, inventory, player->pos);

			goManager->AddGO(ghost);

			//Delete Grid ghost
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_ENEMY_TUMBLEWEED)
		{
			Tumbleweed* weed = new Tumbleweed();

			weed->active = true;
			weed->scale = go->scale;
			weed->pos = go->pos;
			weed->physics = go->physics->Clone();
			weed->physics->SetInelasticity(0.99f);
			weed->physics->SetIsBouncable(false);
			weed->Init();

			goManager->AddGO(weed);

			//Delete Grid weed
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_GY_GATEKEEPER)
		{
			gatekeeper = new Gatekeeper();

			gatekeeper->active = true;
			gatekeeper->scale = go->scale;
			gatekeeper->pos = go->pos;
			gatekeeper->physics = go->physics->Clone();
			gatekeeper->physics->SetInelasticity(0.99f);
			gatekeeper->physics->SetIsBouncable(false);
			gatekeeper->Init(inventory);

			goManager->AddGO(gatekeeper);

			//Delete Grid weed
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_PICKAXE)
		{
			pickaxe = go;
			pickaxe->active = false;
		}
		
	}
	tiles.erase(std::remove(tiles.begin(), tiles.end(), nullptr), tiles.end());
	

	// Add all remaining tiles
	goManager->AddAllGO(tiles);

	// Camera 
	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.SetLimits(m_screenWidth, m_screenHeight, m_worldWidth, m_worldHeight);
	camera.SetFocusTarget(player->pos);
	camera.SetMode(Camera::CENTER);

	// ABILITIESZ
	gameManager->initAbilities(this, &camera, goManager, player);
	player->SetAbilities(gameManager->getCurrAbility(1), gameManager->getCurrAbility(2));

	CSoundController::GetInstance()->PlaySoundByID(SOUND_TYPE::BG_GRAVEYARD);

	story_state = GY_INTRO;
}

void SceneGraveyard::Update(double dt)
{
	SceneBase::Update(dt);
	//inventory->Update(dt);
	camera.Update(player->pos, dt);

	// Updating of light things - if got time, shift to within flashlight instead to organise better, but low priority
	lights[0].position.Set(player->pos.x, player->pos.y, player->pos.z + 10);
	double mouseposx, mouseposy;
	CursorToWorldPosition(mouseposx, mouseposy);
	lights[1].position.Set(mouseposx, mouseposy, 10);


	if (input->IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (input->IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	if (input->IsKeyPressed('C'))
	{
		LoadBossScene();
		story_state = CHURCH_DEFAULT;
	}

	goManager->Update(dt, &this->camera);
	inventory->Update(dt);

	if (player->currentHP <= 0)
	{
		gameLost = true;
		return;
	}

	// STORY UPDATES
	switch (story_state)
	{
	case GY_INTRO:
		if (player->physics->GetOnGround())
		{
			dialogueManager->AddDialogue(PLAYER, "Is this a graveyard?", LEFT, 1.0f);
			dialogueManager->AddDialogue(PLAYER, "Its so dark", LEFT, 1.0f);
			dialogueManager->AddDialogue(PLAYER, "Oh! There's a flashlight there!", LEFT, 1.0f);
			story_state = GY_FINDGATEKEEPER;
		}
		break;
	case GY_FINDGATEKEEPER:
		if (abs(gatekeeper->pos.x - player->pos.x) < 20)
		{
			if (gatekeeper->Interact())
			{
				dialogueManager->AddDialogue(PLAYER, "??!! Who's there?", LEFT);
				dialogueManager->AddDialogue(GATEKEEPER, "Oh. You must be the one who broke the space-time continuum and made boss angry", RIGHT, 3.0f);
				dialogueManager->AddDialogue(GATEKEEPER, "That machinery part inside the church belongs to you doesn't it", RIGHT, 3.0f);
				dialogueManager->AddDialogue(PLAYER, "!!!", LEFT);
				dialogueManager->AddDialogue(GATEKEEPER, "Here's a pickaxe. bring me 5 skulls and 10 bones and I'll let you into the church.", RIGHT, 3.0f);
				story_state = GY_GATEKEEPER_DIALOGUE;
			}
		}
		break;
	case GY_GATEKEEPER_DIALOGUE:
		if (!dialogueManager->isDialogue())
		{
			pickaxe->active = true;
			gatekeeper->TurnBack();
			story_state = GY_GATEKEEPER2;
		}
		break;
	case GY_GATEKEEPER2:
		if (abs(gatekeeper->pos.x - player->pos.x) < 20)
		{
			if (gatekeeper->Interact() && input->IsKeyPressed('E'))
			{
				if (gatekeeper->CheckEntry())
				{
					dialogueManager->AddDialogue(GATEKEEPER, "Did you bring the goods?", RIGHT, 2.0f);
					dialogueManager->AddDialogue(PLAYER, "Here.");
					dialogueManager->AddDialogue(GATEKEEPER, "Nice and ripe. Thanks.", RIGHT, 2.0f);
					dialogueManager->AddDialogue(GATEKEEPER, "You can enter the church now.", RIGHT, 2.0f);
					story_state = GY_GATEKEEPER2_DIALOGUE;
				}
				else
				{
					dialogueManager->AddDialogue(GATEKEEPER, "Didn't I say to bring me 5 skulls and 10 bones?", RIGHT, 2.0f);
				}
			}
		}
		break;
	case GY_GATEKEEPER2_DIALOGUE:
		if (!dialogueManager->isDialogue())
		{
			LoadBossScene();
			story_state = CHURCH_INTRO;
		}
		break;
	case CHURCH_INTRO:
		if (reaper->Interact())
		{
			story_state = CHURCH_DIALOGUE;
			dialogueManager->AddDialogue(GRIMREAPER, "YOU! You're the cause of all this mess aren't you?", RIGHT);
			dialogueManager->AddDialogue(PLAYER, "(He's holding onto a part of the time machine!)");
			dialogueManager->AddDialogue(GRIMREAPER, "You need this don't you?", RIGHT);
			dialogueManager->AddDialogue(GRIMREAPER, "There's no way im just going to give it to you after the chaos you've brought to this world. Not until my death!", RIGHT);
			dialogueManager->AddDialogue(GRIMREAPER, "My lifesource comes from all these tombstones all over the church. Just try and see if you can destroy all of them.", RIGHT);
			dialogueManager->AddDialogue(GRIMREAPER, "Not before I kill you first!", RIGHT);
		}
		break;
	case CHURCH_DIALOGUE:
		if (!dialogueManager->isDialogue())
		{
			story_state = CHURCH_DEFAULT;
		}
		break;
	case CHURCH_DEFAULT:
		if (reaper->currentHP <= 0)
		{
			machinepart->active = true;
			machinepart->pos = reaper->pos;
			story_state = CHURCH_END;
			dialogueManager->AddDialogue(GRIMREAPER, "Ugh.. it seems I have lost.", RIGHT);
			dialogueManager->AddDialogue(GRIMREAPER, "Greedy humans.. You were never supposed to mess with the spacetime continuum, only disaster awaits you.", RIGHT);
		}
		break;
	case CHURCH_END:
		if (gameManager->getMachineStatus(4))
		{
			gameWin = true;
		}
		break;

	}
}

void SceneGraveyard::Render()
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


	// BG
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, -0.01);
	modelStack.Scale(m_screenWidth, m_screenHeight, 1);
	RenderMesh(meshList[GEO_BG], true);
	modelStack.PopMatrix();

	goManager->Render(this);
	inventory->Render();

	std::ostringstream ss;
	//ss.str("");
	//ss << "LIGHT COLOR: " << Vector3(lights[0].color.r, lights[0].color.g, lights[0].color.b);
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 6);
	/*ss.str("");
	ss << "player vel: " << player->physics->GetVelocity();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 9);*/
	//ss.str("");
	//ss << "camera pos: " << camera.position;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 12);



	// fps tings
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2, 0, 2);

	RenderTextOnScreen(meshList[GEO_TEXT], "Graveyard", Color(1, 1, 1), 2, 0, 0);
}

void SceneGraveyard::InitLights()
{
	lights[0].type = Light::LIGHT_POINT;
	lights[0].position.Set(player->pos.x, player->pos.y, player->pos.z + 10);
	lights[0].color.Set(1, 1, 1);
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

	bLightEnabled = true;
}

void SceneGraveyard::CursorToWorldPosition(double& theX, double& theY)
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

void SceneGraveyard::LoadBossScene()
{
	goManager->DeleteAllGOs();

	m_worldHeight = 144;
	m_worldWidth = 256;

	// Unique Meshes
	meshList[GEO_BG] = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	meshList[GEO_BG]->textureID = LoadTGA("Image/churchBG.tga");

	//Level Loading
	std::vector<GameObject*> tiles;
	if (LevelLoader::GetInstance()->LoadTiles("GRAVEYARD_FINAL", this->meshList, this->tileSize, tiles, gridLength, gridHeight))
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


			//Delete Grid Player
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_ENEMY_GHOST)
		{
			Ghost* ghost = new Ghost();

			ghost->active = true;
			ghost->scale = go->scale;
			ghost->pos = go->pos;
			ghost->physics = go->physics->Clone();
			ghost->physics->SetInelasticity(0.99f);
			ghost->physics->SetIsBouncable(false);
			ghost->physics->SetGravity(Vector3(0, 0, 0));
			ghost->Init(this, inventory, player->pos);

			goManager->AddGO(ghost);

			//Delete Grid ghost
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_ENEMY_GRIMREAPER)
		{
			reaper = new GrimReaper();

			reaper->active = true;
			reaper->scale = go->scale;
			reaper->pos = go->pos;
			reaper->physics = go->physics->Clone();
			reaper->physics->SetInelasticity(0.99f);
			reaper->physics->SetIsBouncable(false);
			reaper->physics->SetGravity(Vector3(0, 0, 0));
			reaper->Init(this, goManager, player->pos);

			goManager->AddGO(reaper);

			//Delete Grid reaper
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_TOMBSTONE || go->geoTypeID == GEOMETRY_TYPE::GEO_TOMBSTONE_CROSS)
		{
			Tombstone* stone = new Tombstone();

			stone->active = true;
			stone->scale = go->scale;
			stone->pos = go->pos;
			stone->physics = go->physics->Clone();
			stone->mesh = MeshBuilder::GenerateQuad("stone", Color(1.0f, 1.0f, 1.0f), 2.0f);
			stone->mesh->textureID = go->mesh->textureID;
			stone->Init(this, inventory);

			goManager->AddGO(stone);
			//Delete Grid stone
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_MACHINEPART_4)
		{
			machinepart = go;
			machinepart->active = false;
		}
	}
	tiles.erase(std::remove(tiles.begin(), tiles.end(), nullptr), tiles.end());

	// Add all remaining tiles
	goManager->AddAllGO(tiles);
	

	camera.SetLimits(m_screenWidth, m_screenHeight, m_worldWidth, m_worldHeight);
	camera.SetFocusTarget(player->pos);
	camera.SetMode(Camera::CENTER);

	// ABILITIES
	gameManager->initAbilities(this, &camera, goManager, player);
	player->SetAbilities(gameManager->getCurrAbility(1), gameManager->getCurrAbility(2));

	CSoundController::GetInstance()->StopPlayingSoundByID(SOUND_TYPE::BG_GRAVEYARD, 1, 0.5);
	CSoundController::GetInstance()->PlaySoundByID(SOUND_TYPE::BG_CHURCH);
}

void SceneGraveyard::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	goManager->Exit();
	inventory->Clear();

	CSoundController::GetInstance()->StopPlayingSoundByID(SOUND_TYPE::BG_GRAVEYARD, 1, 0.5);
	CSoundController::GetInstance()->StopPlayingSoundByID(SOUND_TYPE::BG_CHURCH, 1, 0.5);
}
