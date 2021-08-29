#include "SceneOcean.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"
#include <sstream>
#include "LevelLoader.h"
#include "Utility.h"
#include "Debug.h"

//Entity Includes
#include "Player.h"
#include "SlowTime.h"

SceneOcean::SceneOcean()
{
	goManager = new GameObjectManager();
	inventory = new Inventory();
}

SceneOcean::~SceneOcean()
{
	input = NULL;
}

void SceneOcean::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_worldHeight = m_screenHeight * 3;
	m_worldWidth = m_screenWidth * 7;

	//Inventory init
	inventory->Init(this);

	// GO Manager
	goManager->Init();

	// Dialogue Manager
	dialogueManager = DialogueManager::GetInstance();

	//Physics code here
	m_speed = 1.f;
	Math::InitRNG();

	//Store keyboard instance
	input = Input::GetInstance();

	//stores gameManager instance
	gameManager = GameManager::GetInstance();

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
			player->Init(&camera, Player::PLATFORMER, goManager, inventory);
			player->AddBottomSprite();
			player->bottomSprite->mesh = meshList[GEO_WALL];
			goManager->AddGO(player);
			//delete grid player
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_OCEAN_CRAB)
		{
			Crab* crab = new Crab();
			crab->active = true;
			crab->scale = go->scale;
			crab->pos = go->pos;
			crab->physics = go->physics->Clone();
			crab->physics->SetInelasticity(0.99f);
			crab->physics->SetIsBouncable(false);
			crab->Init(player, Crab::LAR);
			crab->AddBottomSprite();
			crab->bottomSprite->mesh = meshList[GEO_WALL];
			goManager->AddGO(crab);
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_OCEAN_DRAGONHEAD)
		{
			Dragon* dragon = new Dragon();
			dragon->active = true;
			dragon->scale = go->scale;
			dragon->pos = go->pos;
			dragon->physics = go->physics->Clone();
			dragon->physics->SetInelasticity(0.99f);
			dragon->physics->SetIsBouncable(false);
			dragon->physics->SetGravity(Vector3(0, 0, 0));			
			dragon->Init(this, player, 8, goManager);
			dragon->mesh = nullptr;
			goManager->AddGO(dragon);
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_OCEAN_THUNDERBLOCK)
		{
			ThunderBlock* thunder = new ThunderBlock(goManager);
			thunder->active = true;
			thunder->scale = go->scale;
			thunder->pos = go->pos;
			thunder->physics = go->physics->Clone();
			thunder->physics->SetInelasticity(0.99f);
			thunder->physics->SetIsBouncable(false);
			thunder->Init();
			thunder->mesh = go->mesh;
			goManager->AddGO(thunder);
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_OCEAN_YH)
		{
			yh = new YongHong();
			yh->active = true;
			yh->scale = go->scale;
			yh->pos = go->pos;
			yh->physics = go->physics->Clone();
			yh->physics->SetInelasticity(0.99f);
			yh->physics->SetIsBouncable(false);
			yh->Init(inventory);
			goManager->AddGO(yh);
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_OCEAN_WHALE)
		{
			whale = new Whale();

			whale->active = true;
			whale->scale = go->scale;
			whale->pos = go->pos;
			whale->physics = go->physics->Clone();
			whale->physics->SetInelasticity(0.99f);
			whale->physics->SetIsBouncable(false);
			whale->Init(inventory);
			goManager->AddGO(whale);
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
	gameManager->initAbilities(this, &camera, goManager, player);
	player->SetAbilities(gameManager->getCurrAbility(1), gameManager->getCurrAbility(2));

	// TRIDENT
	Trident* trident = new Trident;
	trident->Init(&camera, goManager, player);
	inventory->AddItem(trident);

	// STORYSTATE INIT
	story_state = INTRO;

	// SOUND
	CSoundController::GetInstance()->PlaySoundByID(SOUND_TYPE::BG_OCEAN);
}

void SceneOcean::Update(double dt)
{
	//Update SceneBase
	SceneBase::Update(dt);
	
	//Update Inventory
	inventory->Update(dt);
	
	//Update Camera
	camera.Update(player->pos, dt);

	//Update goManager
	goManager->Update(dt, &this->camera);

	//checks if gameLost / player dies
	if (player->currentHP <= 0)
	{
		gameLost = true;
		return;
	}

	// STORY UPDATES
	switch (story_state)
	{
	case INTRO:
		if (player->physics->GetOnGround())
		{
			dialogueManager->AddDialogue(PLAYER, "Where am I now? This smells familiar.", LEFT, 1.0f);
			dialogueManager->AddDialogue(PLAYER, "The ocean? But why's there so many crabs?", LEFT, 1.0f);
			dialogueManager->AddDialogue(PLAYER, "Why am I holding a trident?", LEFT, 1.0f);
			story_state = FINDWHALE;
		}
		break;
	case FINDWHALE:
		if (abs(whale->pos.x - player->pos.x) < 30)
		{
			if (input->IsKeyPressed(gameManager->INTERACT_KEYBIND))
			{
				dialogueManager->AddDialogue(PLAYER, "???", LEFT);
				dialogueManager->AddDialogue(PLAYER, "A beached whale?", LEFT);
				dialogueManager->AddDialogue(WHALE, "Hello second time-traveller! Welcome to my empire.", RIGHT, 3.0f);
				dialogueManager->AddDialogue(PLAYER, "Second? What do you mean?", LEFT);
				dialogueManager->AddDialogue(WHALE, "There was another one of your kind who landed earlier. Had a strange aura coming from it.", RIGHT, 3.0f);
				dialogueManager->AddDialogue(PLAYER, "I see... did you happen to know where they went?", LEFT);
				dialogueManager->AddDialogue(PLAYER, "Do you also happen to have a time-machine part?", LEFT);
				dialogueManager->AddDialogue(WHALE, "Unfortunately no. But I did happen to see the part you speak of.", RIGHT, 3.0f);
				dialogueManager->AddDialogue(WHALE, "Bring me at least 5 of the blue and red colored shells each to prove your worth, and I'll tell you.", RIGHT, 3.0f);
				dialogueManager->AddDialogue(PLAYER, "Okay.", LEFT);
				story_state = WHALE_TEXT;
			}
		}
		break;
	case WHALE_TEXT:
		if (abs(whale->pos.x - player->pos.x) < 30)
		{
			if (input->IsKeyPressed(gameManager->INTERACT_KEYBIND))
			{
				if (whale->CheckEntry())
				{
					dialogueManager->AddDialogue(WHALE, "Alright I'll tell you now. I last saw the parts with the time-traveller.", RIGHT, 2.0f);
					story_state = FIND_YH;
				}
				else
				{
					dialogueManager->AddDialogue(WHALE, "Unfortunately, it seems you do not have enough of the required items.", RIGHT, 2.0f);
					dialogueManager->AddDialogue(WHALE, "Come back when you have them.", RIGHT, 2.0f);
				}
			}
		}
		break;
	case FIND_YH:
		if (abs(yh->pos.x - player->pos.x) < 20)
		{
			if (input->IsKeyPressed(gameManager->INTERACT_KEYBIND))
			{
				dialogueManager->AddDialogue(PLAYER, "Excuse me? Are you the time-traveller the whale speaks of?");
				dialogueManager->AddDialogue(YH, "Yes I am. Fancy seeing another human here on this planet.", RIGHT);
				dialogueManager->AddDialogue(YH, "What do you need?", RIGHT);
				dialogueManager->AddDialogue(PLAYER, "I need the time-machine part to fix my machine. Do you happen to have it?");
				dialogueManager->AddDialogue(YH, "I can give it to you on one condition.", RIGHT);
				dialogueManager->AddDialogue(YH, "I've lost my bottle. It contains great power beyond this world's knowledge, and can cause chaos.", RIGHT);
				dialogueManager->AddDialogue(YH, "If you find my bottle and return it to me, I will give you your machine part.", RIGHT);
				dialogueManager->AddDialogue(PLAYER, "Okay.", LEFT);
				story_state = YH_TEXT;
			}
		}
		break;
	case YH_TEXT:
		if (abs(yh->pos.x - player->pos.x) < 20)
		{
			if (input->IsKeyPressed(gameManager->INTERACT_KEYBIND))
			{
				if (yh->CheckEntry())
				{
					dialogueManager->AddDialogue(YH, "You've found it! Thank you for returning it. Here's the machine part.", RIGHT, 2.0f);
					story_state = OCEAN_END;
				}
				else
				{
					dialogueManager->AddDialogue(YH, "Where's my bottle?", RIGHT, 2.0f);
					dialogueManager->AddDialogue(YH, "No bottle no bueno.", RIGHT, 2.0f);
				}
			}
		}
		break;
	case OCEAN_END:
		//checks if game won
		if (gameManager->getMachineStatus(3))
		{
			gameWin = true;
		}
		break;
	}

	//Lights update when slowtime ability used
	lights[0].position.Set(player->pos.x, player->pos.y, player->pos.z + 10);

}

//Initialise Lights
void SceneOcean::InitLights()
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

void SceneOcean::Render()
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
	//Cleanup sound
	CSoundController::GetInstance()->StopPlayingSoundByID(SOUND_TYPE::BG_OCEAN, 1, 0.5);
	//Cleanup scenebase
	SceneBase::Exit();
	//Cleanup GameObjects
	goManager->Exit();
	//Cleanup inventory
	inventory->Clear();
}
