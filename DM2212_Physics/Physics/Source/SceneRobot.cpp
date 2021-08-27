#include "SceneRobot.h"
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
#include "PlasmaRobot.h"
#include "BlackHoleBullet.h"

#include "Coin.h"

//Ability Includes
#include "Recall.h"
#include "BlackHole.h"

SceneRobot::SceneRobot()
{
	goManager = new GameObjectManager();
	inventory = new Inventory();
}

SceneRobot::~SceneRobot()
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

void SceneRobot::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_worldHeight = 143;
	m_worldWidth = 500;

	//Physics code here
	m_speed = 1.f;
	Math::InitRNG();

	// GO Manager
	goManager->Init();
	// Inventory 
	inventory->Init(this);

	//Store keyboard instance
	input = Input::GetInstance();

	// Unique Meshes
	meshList[GEO_BG] = MeshBuilder::GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	meshList[GEO_BG]->textureID = LoadTGA("Image/bg_robot.tga");

	//Level Loading
	std::vector<GameObject*> tiles;
	if(LevelLoader::GetInstance()->LoadTiles("ROBOT_1_1", this->meshList, this->tileSize, tiles, gridLength, gridHeight))
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
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_ROBOT_ENEMY_1)
		{
			PlasmaRobot* robot = new PlasmaRobot();
			robot->active = true;
			robot->scale = go->scale;
			robot->pos = go->pos;
			robot->physics = go->physics->Clone();
			robot->Init(player, new BulletSpawner(goManager, new PlasmaBullet(Vector3(2, 2, 2), robot, 40)));
			robot->AddBottomSprite();
			robot->bottomSprite->mesh = meshList[GEO_WALL];

			goManager->AddGO(robot);
			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_COIN)
		{
			Coin* coin = new Coin(1);
			coin->active = true;
			coin->scale = go->scale * 0.9;
			coin->pos = go->pos;
			coin->physics = go->physics->Clone();
			coin->Init();
			goManager->AddGO(coin);

			delete go;
			go = nullptr;
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_ROBOT_SMALLCUBE_9_MISCDECOR)
		{
			go->SetDamagableByExplosive(true);
		}
		else if (go->geoTypeID == GEOMETRY_TYPE::GEO_ROBOT_SMALLCUBE_11_MISCDECOR)
		{
			go->SetDamagableByExplosive(true);
			go->SetRespawnable(true);
			
		}
	}
	tiles.erase(std::remove(tiles.begin(), tiles.end(), nullptr), tiles.end());
	
	// Add all remaining tiles
	goManager->AddAllGO(tiles);


	DEBUG_MSG("GOManager Robot: " << goManager);

	// Camera 
	camera.Init(Vector3(player->pos.x, player->pos.y, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.SetLimits(m_screenWidth, m_screenHeight, m_worldWidth, m_worldHeight);
	camera.SetFocusTarget(player->pos);

	player->SetAbilities(
		new RecallAbility(player, 3.0, meshList[GEO_ABILITYICON_RECALL]),
		new BlackHoleAbility(player, new BulletSpawner(goManager, new BlackHoleBullet(meshList[GEO_BLACKHOLE], GEO_BLACKHOLE, Vector3(3, 3, 3), player, 40)), &camera, m_screenWidth, m_screenHeight, meshList[GEO_ABILITYICON_BLACKHOLE]));
	

}

void SceneRobot::Update(double dt)
{
	SceneBase::Update(dt);
	//inventory->Update(dt);
	camera.Update(player->pos, dt);

	// Updating of light things
	lights[0].position.Set(player->pos.x, player->pos.y, player->pos.z + 10);
	double mouseposx, mouseposy;
	CursorToWorldPosition(mouseposx, mouseposy);
	lights[1].position.Set(mouseposx, mouseposy, 10);

	
	if (input->IsMousePressed(0))
	{
		// flashlgiht power/exponent
	}

	if(input->IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(input->IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	if (input->IsKeyReleased('F'))
	{
		spawner->SpawnBullet(player->pos, player->physics->GetNormal(), player->physics->GetNormal());
		/*PlasmaBullet* bul = new PlasmaBullet(Vector3(2, 2, 2), player);
		bul->physics->SetVelocity(player->physics->GetNormal() * 12);
		bul->physics->SetNormal(player->physics->GetNormal());
		bul->pos = player->pos;
		bul->pos.z += 1;
		goManager->AddGO(bul);*/
	}
	
	goManager->Update(dt);

	if (player->currentHP <= 0)
	{
		gameLost = true;
	}

}

void SceneRobot::Render()
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


	std::ostringstream ss;
	/*ss.str("");
	ss << "LIGHT COLOR: " << Vector3(lights[0].color.r, lights[0].color.g, lights[0].color.b);
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 6);
	ss.str("");
	ss << "player pos: " << player->pos;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 9);
	ss.str("");
	ss << "camera pos: " << camera.position;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 12);*/



	// fps tings
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 3);

	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(1, 1, 1), 3, 0, 0);
}

void SceneRobot::InitLights()
{
	bLightEnabled = false;
}

void SceneRobot::CursorToWorldPosition(double& theX, double& theY)
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

void SceneRobot::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	goManager->Exit();
	inventory->Clear();
}
