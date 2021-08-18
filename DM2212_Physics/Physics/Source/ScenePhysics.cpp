#include "ScenePhysics.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"
#include <sstream>

ScenePhysics::ScenePhysics()
{
	goManager = new GameObjectManager();
}

ScenePhysics::~ScenePhysics()
{
	go = NULL;
	keyboard = NULL;
}

void ScenePhysics::Init()
{
	SceneBase::Init();
	goManager->Init();

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_gameWidth = m_worldWidth * 0.75;

	camera.Init(Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1), Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0), Vector3(0, 1, 0));
	//camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	keyboard = Keyboard::GetInstance();

	meshList[GEO_UIFRAME] = MeshBuilder::GenerateQuad("UI Frame", Color(1, 1, 1), 1.0f);
	meshList[GEO_UIFRAME]->textureID = LoadTGA("Image/uiframe.tga");
	meshList[GEO_WALL] = MeshBuilder::GenerateQuad("Wall", Color(1, 1, 1), 2.0f);
	meshList[GEO_BALL] = MeshBuilder::GenerateCircle("circle", 1.0f, Color(1, 1, 1));
	meshList[GEO_100] = MeshBuilder::GenerateCircle("score tings", 1.0f, Color(1, 1, 1));
	meshList[GEO_100]->textureID = LoadTGA("Image/100.tga");
	meshList[GEO_10] = MeshBuilder::GenerateCircle("score tings", 1.0f, Color(1, 1, 1));
	meshList[GEO_10]->textureID = LoadTGA("Image/10.tga");
	meshList[GEO_50] = MeshBuilder::GenerateCircle("score tings", 1.0f, Color(1, 1, 1));
	meshList[GEO_50]->textureID = LoadTGA("Image/50.tga");
	meshList[GEO_HIGHLIGHT] = MeshBuilder::GenerateCircle("highlight", 1.0f, Color(0.0f, 1.0f, 0.0f));

	// PILLARs
	go = new GameObject;
	go->type = GameObject::GO_WALL;
	go->physics->shapeType = RECTANGLE;
	go->physics->SetMass(3);
	go->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0);
	go->scale.Set(2.5, 2.5, 2.5);
	go->physics->SetMovable(true);
	go->active = true;
	go->mesh = meshList[GEO_WALL];
	goManager->AddGO(go);

	GameObject* go2 = new GameObject;
	go2->type = GameObject::GO_WALL;
	go2->physics->SetNormal(Vector3(cos(Math::DegreeToRadian(45)), sin(Math::DegreeToRadian(45)), 0));
	go2->physics->shapeType = RECTANGLE;
	go2->pos.Set(m_worldWidth * 0.25f, m_worldHeight * 0.15f, 0);
	go2->scale.Set(10, 10, 1);
	go2->physics->SetMovable(false);
	go2->active = true;
	go2->mesh = meshList[GEO_WALL];
	goManager->AddGO(go2);

	go2 = new GameObject;
	go2->type = GameObject::GO_WALL;
	go2->physics->SetVelocity(Vector3(1, 0, 0));
	go2->physics->shapeType = RECTANGLE;
	go2->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.35f, 0);
	go2->scale.Set(5, 5, 1);
	go2->physics->SetMovable(true);
	go2->active = true;
	go2->mesh = meshList[GEO_WALL];
	goManager->AddGO(go2);

	go2 = new GameObject;
	go2->type = GameObject::GO_WALL;
	go2->physics->shapeType = RECTANGLE;
	go2->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.15f, 0);
	go2->scale.Set(2.5, 70, 2.5);
	go2->physics->SetMovable(false);
	go2->active = true;
	go2->mesh = meshList[GEO_WALL];
	goManager->AddGO(go2);

	inventory = new Inventory();
	inventory->Init();
}

void ScenePhysics::Update(double dt)
{
	SceneBase::Update(dt);
	inventory->Update(dt);

	if (keyboard->IsKeyPressed('P'))
	{
		std::cout << "PRESSESD P" << std::endl;
		Apple* newApple = new Apple();
		inventory->AddItem(newApple);
		//inventory.setmax(i_apple, 10);
	}
	if (keyboard->IsKeyPressed('O'))
	{
		std::cout << "PRESSESD O" << std::endl;
		Cheese* newCheese = new Cheese();
		inventory->AddItem(newCheese);
	}
	if (keyboard->IsKeyPressed('L'))
	{
		std::cout << "PRESSED L" << std::endl;
		Cheese* newCheese = new Cheese(2);
		inventory->AddItem(newCheese);
	}

	if(Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	if (Application::IsKeyPressed('A'))
	{
		go->physics->SetVelocity(go->physics->GetVelocity() + Vector3(-5,0,0));
	}
	else if (Application::IsKeyPressed('D'))
	{
		go->physics->SetVelocity(go->physics->GetVelocity() + Vector3(5, 0, 0));
	}
	else
	{
		go->physics->SetVelocity(go->physics->GetVelocity() + Vector3(0, 0, 0));
	}

	goManager->Update(dt);
}

void ScenePhysics::Render()
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

	goManager->Render(this);
}

void ScenePhysics::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	goManager->Exit();
}
