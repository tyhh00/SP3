#include "SceneCollision.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"

#include <sstream>

SceneCollision::SceneCollision()
{
}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_gameWidth = m_worldWidth * 0.75;

	camera.Init(Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 1), Vector3(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0), Vector3(0, 1, 0));
	//camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_objectCount = 0;

	m_gravity.Set(0, -30, 0);

	m_score = 0;
	m_highscore = 0;
	numBalls = 3;

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

	m_ghost = new GameObject(GameObject::GO_BALL);

	//main game-related - vars
	pBall_radius = 2;
	f_radius1 = 3.0;
	f_radius2 = 1.5;
	f_length = 13;
	f_max_d = 50;
	f_ori_d = 15;

	// COLOR CODE (for ref)
	// WHITE/GREY - INELASTIC
	// YELLOW - ELASTIC
	// LIGHT RED - BUMPER
	// (BONUS THINGY) - ELASTIC


	//game boundaries/layout - vars
	offset_wall_x = 0.2 * m_worldHeight;
	int offset_wall_top = 0.03 * m_worldHeight;
	int offset_wall_bot = 0.2 * m_worldHeight;
	float offset_tube_bot = 0.2 * m_worldHeight;
	float offset_tube_top = 0.2 * m_worldHeight;
	float tube_height = m_worldHeight - offset_tube_top - offset_tube_bot;
	float tube_width = pBall_radius * 2 + 0.5;
	inner_width = m_gameWidth - 2 * offset_wall_x - tube_width;
	float hole_width = 0.4 * inner_width;
	float hole_height = tube_width + f_radius1;
	float pipe_width = hole_height;
	float curve_length = tube_width * 2;


	// FLIPPERS
	leftFlipper = FetchGO(GameObject::GO_FLIPPER);
	leftFlipper->pos.Set(offset_wall_x + (inner_width - hole_width) * 0.5, hole_height + 1.0f, 0);
	leftFlipper->scale.Set(1, 1, 1);
	leftFlipper->vel.Set(0, 0, 0);
	leftFlipper->angularVelocity = 0;
	leftFlipper->rotateZ = -1 * f_ori_d;
	leftFlipper->mass = 3;

	rightFlipper = FetchGO(GameObject::GO_FLIPPER);
	rightFlipper->pos.Set(offset_wall_x + (inner_width - hole_width) * 0.5 + hole_width, hole_height + 1.0f, 0);
	rightFlipper->scale.Set(1, 1, 1);
	rightFlipper->vel.Set(0, 0, 0);
	rightFlipper->angularVelocity = 0;
	rightFlipper->rotateZ = 180 + f_ori_d;
	rightFlipper->mass = 3;

	// SPRING
	spring_ori_y = offset_tube_bot + 0.1 * tube_height + 1.0f;
	spring_max_d = 0.09 * tube_height;
	sholder_ori_height = spring_ori_y - offset_tube_bot;

	spring = FetchGO(GameObject::GO_SPRING);
	spring->vel.SetZero();
	spring->pos.Set(offset_wall_x + inner_width + tube_width * 0.5, spring_ori_y, 0);
	spring->normal.Set(0, -1, 0);
	spring->scale.Set(0.5 * 0.5, tube_width * 0.5, 1);
	spring->mat.kAmbient.Set(1.0f, 1.0f, 0.4f);
	spring->mass = 10;

	springHolder = FetchGO(GameObject::GO_WALL);
	springHolder->pos.Set(offset_wall_x + inner_width + tube_width * 0.5, offset_tube_bot + 0.5 * sholder_ori_height, 0);
	springHolder->scale.Set(0.5 * 0.3f, 0.5 * sholder_ori_height, 1);
	springHolder->normal.Set(1, 0, 0);
	springHolder->mat.kAmbient.Set(0.6f, 0.6f, 0.6f);
	spring->physics->Update();

	// PLAYER BALL
	pBall = FetchGO(GameObject::GO_BALL);
	pBall->scale.Set(pBall_radius, pBall_radius, 1);
	pBall->pos.Set(m_gameWidth - offset_wall_x - tube_width * 0.5, spring_ori_y + pBall_radius + 1.0f, 0);
	pBall_ori_pos = pBall->pos;
	pBall->vel.SetZero();
	pBall->mat.kAmbient.Set(1.0f, 1.0f, 1.0f);

	//ALL GAMEOBJECTS
	GameObject* go;
	Wall* wall;
	// LEFT WALL
	wall = new Wall(Vector3(offset_wall_x, m_worldHeight * 0.5 + (offset_wall_bot - offset_wall_top) * 0.5, 0), m_worldHeight - offset_wall_top - offset_wall_bot, 1, Vector3(-1, 0, 0));
	wallList.push_back(wall);
	// TUBE WALLS - left, right, bot, holder(?)
	wall = new Wall(Vector3(m_gameWidth - offset_wall_x - tube_width, offset_tube_bot + (tube_height * 0.5), 0), tube_height, 0.3, Vector3(-1, 0, 0));
	wallList.push_back(wall);
	AddPillars(wall);
	wall = new Wall(Vector3(m_gameWidth - offset_wall_x, offset_tube_bot + (tube_height * 0.5), 0), tube_height, 0.3, Vector3(1, 0, 0));
	wallList.push_back(wall);
	AddPillars(wall);
	wall = new Wall(Vector3(m_gameWidth - offset_wall_x - (tube_width * 0.5), offset_tube_bot, 0), tube_width, 0.3, Vector3(0, -1, 0));
	wallList.push_back(wall);
	wall = new Wall(Vector3(m_gameWidth - offset_wall_x - (tube_width * 0.5), offset_tube_bot + 0.1 * tube_height, 0), tube_width, 0.3, Vector3(0, -1, 0));
	wallList.push_back(wall);
	// TUBE WALL - ONE SIDED WALL COVER
	go = FetchGO(GameObject::GO_ONESIDEDWALL);
	go->pos.Set(m_gameWidth - offset_wall_x - tube_width * 0.5, m_worldHeight - offset_tube_top, 0);
	go->scale.Set(0.15, 0.5 * tube_width, 1);
	go->normal.Set(cos(Math::DegreeToRadian(-70)), sin(Math::DegreeToRadian(-70)), 0);
	go->pos.y += tube_width * 0.5 * tan(Math::DegreeToRadian(20));
	go->mat.kAmbient.Set(0.4f, 0.4f, 0.4f);

	// TOP WALL
	wall = new Wall(Vector3(m_gameWidth * 0.5 - curve_length * 0.5, m_worldHeight - offset_wall_top, 0), m_gameWidth - 2 * offset_wall_x - curve_length, 1, Vector3(0, 1, 0));
	wallList.push_back(wall);
	AddPillars(wall);
	// SLANTED WALLS
	Vector3 temp; //length and height of wall
	float angle;
	// BOTTOM SLANTED WALLS - left, right
	temp.Set(0.5 * (inner_width - hole_width) - pipe_width, offset_wall_bot - hole_height, 0);
	angle = Math::RadianToDegree(atan2(temp.y, temp.x)) - 90;
	wall = new Wall(Vector3(offset_wall_x + temp.x * 0.5, hole_height + temp.y * 0.5, 0), temp.Length(), 1, -1 * angle);
	Vector3 leftN = wall->N;
	AddPillars(wall);
	wallList.push_back(wall);
	wall = new Wall(Vector3(m_gameWidth - offset_wall_x - temp.x * 0.5 - tube_width, hole_height + temp.y * 0.5, 0), temp.Length(), 1, angle);
	Vector3 rightN = wall->N;
	AddPillars(wall);
	wallList.push_back(wall);
	// INNER BOTTOM SLANTED WALLS - left, right
	temp.Set(0.5 * (inner_width - hole_width) - pipe_width, offset_wall_bot - hole_height, 0);
	angle = Math::RadianToDegree(atan2(temp.y, temp.x)) - 90;
	wall = new Wall(Vector3(offset_wall_x + temp.x * 0.5, hole_height + temp.y * 0.5 + f_radius1, 0), temp.Length(), 1, -1 * angle);
	wall->pos.x += tube_width + f_radius1;
	AddPillars(wall);
	wallList.push_back(wall);
	wall = new Wall(Vector3(m_gameWidth - offset_wall_x - temp.x * 0.5 - tube_width, hole_height + temp.y * 0.5 + f_radius1, 0), temp.Length(), 1, angle);
	wall->pos.x -= tube_width + f_radius1;
	AddPillars(wall);
	wallList.push_back(wall);
	// BOTTOM HOLE WALLS - left, right
	wall = new Wall(Vector3(offset_wall_x + (inner_width - hole_width) * 0.5 - tube_width - f_radius1, hole_height * 0.5, 0), hole_height, 1, Vector3(-1, 0, 0));
	wallList.push_back(wall);
	wall = new Wall(Vector3(offset_wall_x + (inner_width - hole_width) * 0.5, hole_height * 0.5, 0), hole_height, 1, Vector3(-1, 0, 0));
	wallList.push_back(wall);
	wall = new Wall(Vector3(offset_wall_x + (inner_width - hole_width) * 0.5 + hole_width + tube_width + f_radius1, hole_height * 0.5, 0), hole_height, 1, Vector3(1, 0, 0));
	wallList.push_back(wall);
	wall = new Wall(Vector3(offset_wall_x + (inner_width - hole_width) * 0.5 + hole_width, hole_height * 0.5, 0), hole_height, 1, Vector3(1, 0, 0));
	wallList.push_back(wall);


	// CURVED WALL
	int numWall = 36;
	float angleperWall = 90.0f / 36;
	for (float angle = 90.0f / numWall; angle < 90; angle += 90.0f / numWall)
	{
		wall = new Wall(Vector3(offset_wall_x + inner_width - tube_width + curve_length * cos(Math::DegreeToRadian(angle)), offset_tube_bot + tube_height + (offset_tube_top - offset_wall_top) * sin(Math::DegreeToRadian(angle))),
			1.2, 0.3 + (1.0 - 0.3) / numWall * (angle / angleperWall), angle);
		wallList.push_back(wall);
	}

	// PILLARS
	int numTotal = 2;
	float space = 10;
	for (int i = 0; i < numTotal; i++)
	{
		go = FetchGO(GameObject::GO_EPILLAR);
		go->pos.Set(offset_wall_x + inner_width * 0.8 - (numTotal - 1) * space * 0.5 + i * space, m_worldHeight * 0.35, 0);
		go->scale.Set(1.5, 1.5, 1.5);
		go->mat.kAmbient.Set(1.0f, 1.0f, 0.4f);
	}
	for (int i = 0; i < numTotal; i++)
	{
		go = FetchGO(GameObject::GO_EPILLAR);
		go->pos.Set(offset_wall_x + inner_width * 0.7 - (numTotal - 1) * space * 0.5 + i * space, m_worldHeight * 0.25, 0);
		go->scale.Set(1.5, 1.5, 1.5);
		go->mat.kAmbient.Set(1.0f, 1.0f, 0.4f);
	}
	for (int i = 0; i < numTotal; i++)
	{
		go = FetchGO(GameObject::GO_EPILLAR);
		go->pos.Set(offset_wall_x + inner_width * 0.2 - (numTotal - 1) * space * 0.5 + i * space, m_worldHeight * 0.35, 0);
		go->scale.Set(1.5, 1.5, 1.5);
		go->mat.kAmbient.Set(1.0f, 1.0f, 0.4f);
	}
	for (int i = 0; i < numTotal; i++)
	{
		go = FetchGO(GameObject::GO_EPILLAR);
		go->pos.Set(offset_wall_x + inner_width * 0.3 - (numTotal - 1) * space * 0.5 + i * space, m_worldHeight * 0.25, 0);
		go->scale.Set(1.5, 1.5, 1.5);
		go->mat.kAmbient.Set(1.0f, 1.0f, 0.4f);
	}



	// BOTTOM PIPES - left, right
	GameObject* pipe = FetchGO(GameObject::GO_MARKER);
	pipe->pos.Set(offset_wall_x + (inner_width - hole_width) * 0.5 - pipe_width * 0.5, (hole_height - f_radius1) * 0.5, 0);
	pipe->scale.Set(0.5 * pipe_width - 0.5, 0.5 * (hole_height - f_radius1), 1);
	pipe->mat.kAmbient.Set(0.3f, 0.3f, 0.3f);
	pipe = FetchGO(GameObject::GO_MARKER);
	pipe->pos.Set(offset_wall_x + (inner_width - hole_width) * 0.5 + hole_width + pipe_width * 0.5, (hole_height - f_radius1) * 0.5, 0);
	pipe->scale.Set(0.5 * pipe_width - 0.5, 0.5 * (hole_height - f_radius1), 1);
	pipe->normal.Set(0, 1, 0);
	pipe->mat.kAmbient.Set(0.3f, 0.3f, 0.3f);
	// TOP PIPE
	topPipe = FetchGO(GameObject::GO_MARKER);
	topPipe->pos.Set((m_gameWidth - tube_width) * 0.5, m_worldHeight - offset_wall_top - 5 - 0.5, 0);
	topPipe->scale.Set(5.0f, 0.5 * 10, 1);
	topPipe->mat.kAmbient.Set(0.3f, 0.3f, 0.3f);
	// TOP PIPE SIDE WALLS (to prevent balls escaping) - left, right
	wall = new Wall(Vector3(topPipe->pos.x - topPipe->scale.x, topPipe->pos.y, 2), topPipe->scale.y * 2, 0.5f, Vector3(-1, 0, 0));
	wall->SetColour(topPipe->mat.kAmbient);
	wallList.push_back(wall);
	wall = new Wall(Vector3(topPipe->pos.x + topPipe->scale.x, topPipe->pos.y, 2), topPipe->scale.y * 2, 0.5f, Vector3(1, 0, 0));
	wall->SetColour(topPipe->mat.kAmbient);
	wallList.push_back(wall);
	// TOP PIPE OPENING WALL
	go = FetchGO(GameObject::GO_ONESIDEDWALL);
	go->pos = topPipe->pos;
	go->scale.Set(0.5f, topPipe->scale.x, 1);
	go->pos.y -= topPipe->scale.y + go->scale.x;
	go->normal.Set(0, 1, 0);
	go->mat.kAmbient.Set(0.3f, 0.3f, 0.3f);
	AddPillars(go);

	// BONUS THICK WALLS - left, right
	bonus1 = FetchGO(GameObject::GO_BONUS);
	bonus1->scale.Set(6.0f, topPipe->scale.y * 2, 0);
	bonus1->pos = topPipe->pos;
	bonus1->pos.x += bonus1->scale.x * 0.5 + topPipe->scale.x;
	bonus1->normal.Set(0, 1, 0);
	bonus1->mat.kAmbient.Set(0.2f, 0.2f, 0.5f);
	bonus1->timeout = 0;

	bonus2 = FetchGO(GameObject::GO_BONUS);
	bonus2->scale.Set(6.0f, topPipe->scale.y * 2, 0);
	bonus2->pos = topPipe->pos;
	bonus2->pos.x -= bonus2->scale.x * 0.5 + topPipe->scale.x;
	bonus2->normal.Set(0, 1, 0);
	bonus2->mat.kAmbient.Set(0.2f, 0.2f, 0.5f);
	bonus2->timeout = 0;

	// slanted walls beside bumper - left, right
	temp.Set(7.0f, bonus1->scale.y, 0);
	angle = Math::RadianToDegree(atan2(temp.y, temp.x)) - 90;
	wall = new Wall(Vector3(bonus1->pos.x + bonus1->scale.x * 0.5 + temp.x * 0.5, bonus1->pos.y, 0), temp.Length(), 1.0f, angle);
	wallList.push_back(wall);
	AddPillars(wall);
	wall = new Wall(Vector3(bonus2->pos.x - bonus2->scale.x * 0.5 - temp.x * 0.5, bonus1->pos.y, 0), temp.Length(), 1.0f, -1 * angle);
	wallList.push_back(wall);
	AddPillars(wall);

	// MOVING WALLS - left, right
	GameObject** temp1, ** temp2;
	mWall1[0] = FetchGO(GameObject::GO_MWALL);
	mWall1[0]->pos.Set(offset_wall_x + 0.4 * inner_width, 0.5 * m_worldHeight, 0);
	mWall1[0]->vel.Set(20, 0, 0);
	mWall1[0]->scale.Set(0.5f, 6.0f, 1.0f);
	mWall1[0]->normal.Set(cos(Math::DegreeToRadian(90 - 20)), sin(Math::DegreeToRadian(90 - 20)), 0);
	mWall1[0]->mat.kAmbient.Set(0.8f, 0.8f, 0.8f);
	temp1 = &mWall1[1];
	temp2 = &mWall1[2];
	AddPillars(mWall1[0], temp1, temp2);
	mWall1[1]->type = GameObject::GO_MPILLAR;
	mWall1[2]->type = GameObject::GO_MPILLAR;
	mWall2[0] = FetchGO(GameObject::GO_MWALL);
	mWall2[0]->pos.Set(offset_wall_x + 0.6 * inner_width, 0.5 * m_worldHeight, 0);
	mWall2[0]->vel.Set(-20, 0, 0);
	mWall2[0]->scale.Set(0.5f, 6.0f, 1.0f);
	mWall2[0]->normal.Set(cos(Math::DegreeToRadian(90 + 20)), sin(Math::DegreeToRadian(90 + 20)), 0);
	mWall2[0]->mat.kAmbient.Set(0.8f, 0.8f, 0.8f);
	temp1 = &mWall2[1];
	temp2 = &mWall2[2];
	AddPillars(mWall2[0], temp1, temp2);
	mWall2[1]->type = GameObject::GO_MPILLAR;
	mWall2[2]->type = GameObject::GO_MPILLAR;

	// MOVING BUMPER WALLS
	bWall1[0] = FetchGO(GameObject::GO_BWALL);
	bWall1[0]->scale.Set(0.5, 5, 1);
	bWall1[0]->pos.Set(offset_wall_x + 1.0, 0.35 * m_worldHeight, 0);
	bWall1[0]->normal.Set(-1, 0, 0);
	bWall1[0]->mat.kAmbient.Set(1.0f, 0.4f, 0.4f);
	temp1 = &bWall1[1];
	temp2 = &bWall1[2];
	AddPillars(bWall1[0], temp1, temp2);
	bWall1[1]->type = GameObject::GO_BPILLAR;
	bWall1[2]->type = GameObject::GO_BPILLAR;
	bWall2[0] = FetchGO(GameObject::GO_BWALL);
	bWall2[0]->scale.Set(0.5, 5, 1);
	bWall2[0]->pos.Set(offset_wall_x + inner_width - go->scale.x * 0.5 - 0.3, 0.35 * m_worldHeight, 0);
	bWall2[0]->normal.Set(1, 0, 0);
	bWall2[0]->mat.kAmbient.Set(1.0f, 0.4f, 0.4f);
	temp1 = &bWall2[1];
	temp2 = &bWall2[2];
	AddPillars(bWall2[0], temp1, temp2);
	bWall2[1]->type = GameObject::GO_BPILLAR;
	bWall2[2]->type = GameObject::GO_BPILLAR;

	// SPINNING/ROTATING WALLS
	Vector3 pivot;
	pivot.Set(0.7 * m_gameWidth, 0.7 * m_worldHeight, 0);
	go = FetchGO(GameObject::GO_PILLAR);
	go->pos = pivot;
	go->scale.Set(0.5f, 0.5f, 0.5f);
	for (int angle = 0; angle < 360; angle += 90)
	{
		go = FetchGO(GameObject::GO_RWALL);
		go->normal.Set(cos(Math::DegreeToRadian(angle)), sin(Math::DegreeToRadian(angle)), 0);
		go->scale.Set(0.5f, 4.0f, 1); 
		go->pos = pivot;
		go->angularVelocity = 120;
		
		Vector3 plane = go->normal.Cross(Vector3(0, 0, -1));
		go = FetchGO(GameObject::GO_RPILLAR);
		go->angularVelocity = 120;
		go->dir = plane;
		go->scale.Set(0.5f, 0.5f, 0.5f);
		go->pos = pivot;
	}

	pivot.Set(offset_wall_x + 0.5 * inner_width, 0.65 * m_worldHeight, 0);
	go = FetchGO(GameObject::GO_PILLAR);
	go->pos = pivot;
	go->scale.Set(0.5f, 0.5f, 0.5f);
	for (int angle = 0; angle < 360; angle += 90)
	{
		go = FetchGO(GameObject::GO_RWALL);
		go->normal.Set(cos(Math::DegreeToRadian(angle)), sin(Math::DegreeToRadian(angle)), 0);
		go->scale.Set(0.5f, 4.0f, 1);
		go->pos = pivot;
		go->angularVelocity = 150;

		Vector3 plane = go->normal.Cross(Vector3(0, 0, -1));
		go = FetchGO(GameObject::GO_RPILLAR);
		go->angularVelocity = 150;
		go->dir = plane;
		go->scale.Set(0.5f, 0.5f, 0.5f);
		go->pos = pivot;
	}
	



	for (auto wall : wallList)
	{
		GameObject* go;
		go = FetchGO(GameObject::GO_WALL);
		go->pos = wall->pos;
		go->normal = wall->N;
		go->scale = wall->scale;
		go->mat.kAmbient = wall->col;
	}
	for (auto pillar : pillarList)
	{
		GameObject* go;
		go = FetchGO(GameObject::GO_PILLAR);
		go->pos = pillar->pos;
		go->scale = pillar->scale;
		go->mat.kAmbient.Set(pillar->col.r, pillar->col.g, pillar->col.b);
	}

	// POINT
	target100 = FetchGO(GameObject::GO_100);
	target100->pos.Set(offset_wall_x + 0.2 * inner_width, 0.87 * m_worldHeight, 0);
	target100->scale.Set(3, 3, 3);
	target100->timeout = 0;
	target50 = FetchGO(GameObject::GO_50);
	target50->pos.Set(offset_wall_x + 0.3 * inner_width, 0.77 * m_worldHeight, 0);
	target50->scale.Set(4, 4, 4);
	target50->timeout = 0;
	target10 = FetchGO(GameObject::GO_10);
	target10->pos.Set(offset_wall_x + 0.15 * inner_width, 0.72 * m_worldHeight, 0);
	target10->scale.Set(5, 5, 5);
	target10->timeout = 0;
	
	// PILLARs
	go = FetchGO(GameObject::GO_PILLAR);
	go->pos.Set(offset_wall_x +inner_width * 0.4, m_worldHeight * 0.35, 0);
	go->scale.Set(2.5, 2.5, 2.5);
	go->mat.kAmbient.Set(1.0f, 1.0f, 1.0f);
	go = FetchGO(GameObject::GO_PILLAR);
	go->pos.Set(offset_wall_x + inner_width * 0.6, m_worldHeight * 0.35, 0);
	go->scale.Set(2.5, 2.5, 2.5);
	go->mat.kAmbient.Set(1.0f, 1.0f, 1.0f);


}

GameObject* SceneCollision::FetchGO()
{
	// Fetch a game object from m_goList and return it
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (!go->active)
		{
			m_objectCount++;
			go->active = true;
			return go;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		GameObject* temp = new GameObject;
		m_goList.push_back(temp);
	}
	m_objectCount++;
	return FetchGO();
}

GameObject* SceneCollision::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	// Fetch a game object from m_goList and return it
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (!go->active)
		{
			m_objectCount++;
			go->type = type;
			go->active = true;
			return go;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		GameObject* temp = new GameObject;
		m_goList.push_back(temp);
	}
	m_objectCount++;
	return FetchGO(type);
}

void SceneCollision::ReturnGO(GameObject *go)
{
	go->active = false;
	m_objectCount--;
}

void SceneCollision::AddPillars(Wall* wall)
{
	Pillar* temp = new Pillar;
	temp->scale.Set(wall->scale.x, wall->scale.x, wall->scale.x);
	temp->pos = wall->pos + wall->N.Cross(Vector3(0, 0, 1)).Normalized() * wall->scale.y;
	temp->col = wall->col;
	pillarList.push_back(temp);

	temp = new Pillar;
	temp->scale.Set(wall->scale.x, wall->scale.x, wall->scale.x);
	temp->pos = wall->pos + wall->N.Cross(Vector3(0, 0, -1)).Normalized() * wall->scale.y;
	temp->col = wall->col;
	pillarList.push_back(temp);

}

void SceneCollision::AddPillars(GameObject* wall)
{
	Pillar* temp = new Pillar;
	temp->scale.Set(wall->scale.x, wall->scale.x, wall->scale.x);
	temp->pos = wall->pos + wall->normal.Cross(Vector3(0, 0, 1)).Normalized() * wall->scale.y;
	temp->col = wall->mat.kAmbient;
	pillarList.push_back(temp);

	temp = new Pillar;
	temp->scale.Set(wall->scale.x, wall->scale.x, wall->scale.x);
	temp->pos = wall->pos + wall->normal.Cross(Vector3(0, 0, -1)).Normalized() * wall->scale.y;
	temp->col = wall->mat.kAmbient;
	pillarList.push_back(temp);
}

void SceneCollision::AddPillars(GameObject* wall, GameObject** pillar1, GameObject** pillar2)
{
	GameObject* temp = FetchGO(GameObject::GO_PILLAR);
	temp->scale.Set(wall->scale.x, wall->scale.x, wall->scale.x);
	temp->pos = wall->pos + wall->normal.Cross(Vector3(0, 0, 1)).Normalized() * wall->scale.y;
	temp->mat.kAmbient = wall->mat.kAmbient;
	*pillar1 = temp;

	temp = FetchGO(GameObject::GO_PILLAR);
	temp->scale.Set(wall->scale.x, wall->scale.x, wall->scale.x);
	temp->pos = wall->pos + wall->normal.Cross(Vector3(0, 0, -1)).Normalized() * wall->scale.y;
	temp->mat.kAmbient = wall->mat.kAmbient;
	*pillar2 = temp;
}

void SceneCollision::Update(double dt)
{
	SceneBase::Update(dt);
	
	if(Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		// Getting mouse pos 
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		// convert to world space
		x /= (w / m_worldWidth);
		y = h - y;
		y /= (h / m_worldHeight);


		m_ghost->active = true;
		m_ghost->scale.Set(pBall_radius, pBall_radius, 1);
		m_ghost->mass = 5 * pBall_radius;
		m_ghost->pos.Set(x, y, 0);
		m_ghost->mat.kAmbient.Set(Math::RandFloatMinMax(0.1, 1), Math::RandFloatMinMax(0.1, 1), Math::RandFloatMinMax(0.1, 1));

	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		// Getting mouse pos 
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		// convert to world space
		x /= (w / m_worldWidth);
		y = h - y;
		y /= (h / m_worldHeight);

		GameObject* ball = FetchGO(GameObject::GO_BALL);
		ball->pos = m_ghost->pos;
		ball->vel.Set(m_ghost->pos.x - x, m_ghost->pos.y - y, 0);
		ball->scale = m_ghost->scale;
		ball->mass = m_ghost->mass;
		ball->mat = m_ghost->mat;
		m_ghost->active = false;
	}

	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
		
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

	}

	// Spring
	if (Application::IsKeyPressed(VK_DOWN))
	{
		Vector3 accel(0, -20, 0);
		spring->vel += accel * m_speed * dt;
		spring->pos += spring->vel * m_speed * dt;
		springHolder->scale.y += ((spring->vel.y * m_speed * dt) / (2 * springHolder->scale.y)) * springHolder->scale.y;
		springHolder->pos.y += spring->vel.y * 0.5 * m_speed * dt;
	}
	else
	{
		Vector3 accel(0, 500, 0);
		spring->vel += accel * m_speed * dt;
		spring->pos += spring->vel * m_speed * dt;
		springHolder->scale.y += ((spring->vel.y * m_speed * dt) / (2 * springHolder->scale.y)) * springHolder->scale.y;
		springHolder->pos.y += spring->vel.y * 0.5 * m_speed * dt;

	}
	if (spring->pos.y <= spring_ori_y - spring_max_d)
	{
		spring->pos.y = spring_ori_y - spring_max_d;
		springHolder->scale.y = 0.5 * (sholder_ori_height - spring_max_d);
		springHolder->pos.y = spring->pos.y - 0.5 * springHolder->scale.y - spring->scale.x;
		spring->vel.SetZero();
	}
	else if (spring->pos.y >= spring_ori_y)
	{
		spring->pos.y = spring_ori_y;
		springHolder->scale.y = 0.5 * sholder_ori_height;
		springHolder->pos.y = spring->pos.y - 0.5 * sholder_ori_height;
		spring->vel.SetZero();
	}

	// Flippers
	int velamount = 250;
	if (Application::IsKeyPressed(VK_LEFT))
	{
		//float force = 2000;
		//float accel = force * (1.0f / leftFlipper->mass);
		leftFlipper->angularVelocity = velamount;
	}
	else
	{
		//float force = 2000;
		//float accel = -1 * force * (1.0f / leftFlipper->mass);
		leftFlipper->angularVelocity = -1 * velamount;
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		//float force = 1000;
		//float accel = -1 * force * (1.0f / rightFlipper->mass);
		rightFlipper->angularVelocity = -1 * velamount;
	}
	else
	{
		//float force = 1000;
		//float accel = force * (1.0f / rightFlipper->mass);
		rightFlipper->angularVelocity = velamount;
	}

	leftFlipper->rotateZ += leftFlipper->angularVelocity * m_speed * dt;
	rightFlipper->rotateZ += rightFlipper->angularVelocity * m_speed * dt;

	if (leftFlipper->rotateZ >= -1 * f_ori_d + f_max_d)
	{
		leftFlipper->angularVelocity = 0;
		leftFlipper->rotateZ = -1 * f_ori_d + f_max_d;
	}
	else if (leftFlipper->rotateZ <= -1 * f_ori_d)
	{
		leftFlipper->angularVelocity = 0;
		leftFlipper->rotateZ = -1 * f_ori_d;
	}

	if (rightFlipper->rotateZ <= 180 + f_ori_d - f_max_d)
	{
		rightFlipper->angularVelocity = 0;
		rightFlipper->rotateZ = 180 + f_ori_d - f_max_d;
	}
	else if (rightFlipper->rotateZ >= 180 + f_ori_d)
	{
		rightFlipper->angularVelocity = 0;
		rightFlipper->rotateZ = 180 + f_ori_d;
	}

	// bonus
	if (bonus1->timeout > 0)
	{
		bonus1->timeout -= m_speed * dt;
		if (bonus1->timeout <= 0)
		{
			bonus1->timeout = 0;
			bonus1->mat.kAmbient.Set(0.2f, 0.2f, 0.5f);
		}
		if (bonus1->fireInterval <= 0)
		{
			bonus1->fireInterval = 0.3f;
			// Getting mouse pos 
			double x, y;
			Application::GetCursorPos(&x, &y);
			int w = Application::GetWindowWidth();
			int h = Application::GetWindowHeight();
			// convert to world space
			x /= (w / m_worldWidth);
			y = h - y;
			y /= (h / m_worldHeight);

			GameObject* ball = FetchGO(GameObject::GO_BALL);
			ball->pos.z = 0;
			ball->pos.x = Math::RandFloatMinMax(topPipe->pos.x - topPipe->scale.x + ball->scale.x, topPipe->pos.x + topPipe->scale.x - ball->scale.x);
			ball->pos.y = Math::RandFloatMinMax(topPipe->pos.y - topPipe->scale.y + ball->scale.x, topPipe->pos.y + topPipe->scale.y - ball->scale.x);
			ball->vel.Set(0, 0, 0);
			ball->scale.Set(0.75 * pBall_radius, 0.75 * pBall_radius, 1);
			ball->mass = 5 * ball->scale.x;
			ball->mat.kAmbient.Set(Math::RandFloatMinMax(0.2, 1), Math::RandFloatMinMax(0.2, 1), Math::RandFloatMinMax(0.2, 1));
		}
		else
		{
			bonus1->fireInterval -= m_speed * dt;
		}
	}
	if (bonus2->timeout > 0)
	{
		bonus2->timeout -= m_speed * dt;
		if (bonus2->timeout <= 0)
		{
			bonus2->timeout = 0;
			bonus2->mat.kAmbient.Set(0.2f, 0.2f, 0.5f);
		}
		if (bonus2->fireInterval <= 0)
		{
			bonus2->fireInterval = 0.3f;
			// Getting mouse pos 
			double x, y;
			Application::GetCursorPos(&x, &y);
			int w = Application::GetWindowWidth();
			int h = Application::GetWindowHeight();
			// convert to world space
			x /= (w / m_worldWidth);
			y = h - y;
			y /= (h / m_worldHeight);

			GameObject* ball = FetchGO(GameObject::GO_BALL);
			ball->pos.z = 0;
			ball->pos.x = Math::RandFloatMinMax(topPipe->pos.x - topPipe->scale.x + ball->scale.x, topPipe->pos.x + topPipe->scale.x - ball->scale.x);
			ball->pos.y = Math::RandFloatMinMax(topPipe->pos.y - topPipe->scale.y + ball->scale.x, topPipe->pos.y + topPipe->scale.y - ball->scale.x);
			ball->vel.Set(0, 0, 0);
			ball->scale.Set(0.75 * pBall_radius, 0.75 * pBall_radius, 1);
			ball->mass = 5 * ball->scale.x;
			ball->mat.kAmbient.Set(Math::RandFloatMinMax(0.2, 1), Math::RandFloatMinMax(0.2, 1), Math::RandFloatMinMax(0.2, 1));
		}
		else
		{
			bonus2->fireInterval -= m_speed * dt;
		}
	}
	
	// point tings
	if (target100->timeout > 0 && target100->fireInterval >= 0)
	{
		target100->fireInterval -= m_speed * dt;
	}
	if (target50->timeout > 0 && target50->fireInterval >= 0)
	{
		target50->fireInterval -= m_speed * dt;
	}
	if (target10->timeout > 0 && target10->fireInterval >= 0)
	{
		target10->fireInterval -= m_speed * dt;
	}


	// moving inelastic(grey) walls
	float wallvel = 5;
	if (mWall1[0]->pos.x <= offset_wall_x + 0.2 * inner_width)
	{
		for (int i = 0; i < 3; i++)
		{
			mWall1[i]->vel.Set(wallvel, 0, 0);
		}
	}
	else if (mWall1[0]->pos.x >= offset_wall_x + 0.4 * inner_width)
	{
		for (int i = 0; i < 3; i++)
		{
			mWall1[i]->vel.Set(-1 * wallvel, 0, 0);
		}
	}

	if (mWall2[0]->pos.x <= offset_wall_x + 0.6 * inner_width)
	{
		for (int i = 0; i < 3; i++)
		{
			mWall2[i]->vel.Set(wallvel, 0, 0);
		}
	}
	else if (mWall2[0]->pos.x >= offset_wall_x + 0.8 * inner_width)
	{
		for (int i = 0; i < 3; i++)
		{
			mWall2[i]->vel.Set(-1 * wallvel, 0, 0);
		}
	}
	

	// moving bumper walls
	float ewallvel = 7;
	if (bWall1[0]->pos.y <= 0.35 * m_worldHeight)
	{
		for (int i = 0; i < 3; i++)
		{
			bWall1[i]->vel.Set(0, ewallvel, 0);
		}
	}
	else if (bWall1[0]->pos.y >= 0.6 * m_worldHeight)
	{
		for (int i = 0; i < 3; i++)
		{
			bWall1[i]->vel.Set(0, -1 * ewallvel, 0);
		}
	}

	if (bWall2[0]->pos.y <= 0.35 * m_worldHeight)
	{
		for (int i = 0; i < 3; i++)
		{
			bWall2[i]->vel.Set(0, ewallvel, 0);
		}
	}
	else if (bWall2[0]->pos.y >= 0.6 * m_worldHeight)
	{
		for (int i = 0; i < 3; i++)
		{
			bWall2[i]->vel.Set(0, -1 * ewallvel, 0);
		}
	}
	

	// Game Objects
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			switch (go->type)
			{
			case GameObject::GO_BALL:
				go->vel += m_gravity * dt * m_speed;
				go->pos += go->vel * dt * m_speed;
				break;
			case GameObject::GO_BWALL:
			case GameObject::GO_BPILLAR:
			case GameObject::GO_MWALL:
			case GameObject::GO_MPILLAR:
				go->pos += go->vel * dt * m_speed;
				break;
			case GameObject::GO_RWALL:
			case GameObject::GO_RPILLAR:
				go->rotateZ += go->angularVelocity * m_speed * dt;
				if (go->rotateZ >= 360)
				{
					go->rotateZ -= 360;
				}
				else if (go->rotateZ <= 0)
				{
					go->rotateZ += 360;
				}
				break;
			default:
				break;
			}
			
			// ball fell tru hole
			if (pBall->pos.y + pBall_radius < 0)
			{
				pBall->pos = pBall_ori_pos;
				pBall->vel.SetZero();
				numBalls--;
				if (numBalls < 0)
				{
					numBalls = 3;
					messageList.clear();
					std::ostringstream temp;
					messageList.push_back(new Message("You have run out of balls!", Color(1.0f, 0.4f, 0.4f)));
					temp << "Score has been reset. Your score was " << m_score;
					messageList.push_back(new Message(temp.str(), Color(1.0f, 0.4f, 0.4f)));
					if (m_score > m_highscore)
					{
						m_highscore = m_score;
					}
					m_score = 0;
				}
			}

			// In case leave screen Unspawn
			if (go->pos.x >= m_worldWidth + go->scale.x
				|| go->pos.x + go->scale.x <= 0
				|| go->pos.y >= m_worldHeight + go->scale.y
				|| go->pos.y + go->scale.y <= 0)
			{
				ReturnGO(go);
			}

			// Collision
			for (std::vector<GameObject*>::iterator it2 = it + 1; it2 != m_goList.end(); ++it2)
			{
				GameObject* go2 = (GameObject*)*it2;

				if (go2->active)
				{
					GameObject* ball = go;
					GameObject* other = go2;

					if (ball->type != GameObject::GO_BALL)
					{
						ball = go2;
						other = go;
						if (ball->type != GameObject::GO_BALL)
						{
							continue;
						}
					}
					
					if (CheckCollision(ball, other, dt))
					{
						Constraint(ball, other);
						CollisionResponse(ball, other, dt);
						continue; //remove if stays at the end
					}

					
				}
			}

		}
	}

	// Message
	if (messageList.size() > 5)
	{
		Message* popup = messageList.front();
		delete popup;
		messageList.pop_front();
		
	}

	// Camera
	camera.position.Set(pBall->pos.x + 0.125 * m_worldWidth, pBall->pos.y, 1);
	camera.target.Set(pBall->pos.x + 0.125 * m_worldWidth, pBall->pos.y, 0);
}

bool SceneCollision::CheckCollision(GameObject* go1, GameObject* go2, float dt)
{
	// in case of self collision
	/*if (go1 == go2)
	{
		return false;
	}*/
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		Vector3 dis = go2->pos - go1->pos;
		float disSquared = dis.LengthSquared();
		if (disSquared <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x) && dis.Dot(go1->vel - go2->vel) > 0)
		{
			return true;
		}
		return false;
	}
	break;
	case GameObject::GO_100:
	case GameObject::GO_50:
	case GameObject::GO_10:
	{
		Vector3 dis = go2->pos - go1->pos;
		float disSquared = dis.LengthSquared();
		if (disSquared <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x))
		{
			return true;
		}
		else if (go2->timeout > 0 && go2->fireInterval <= 0)
		{
			go2->timeout = 0;
		}
		return false;
	}
	break;
	case GameObject::GO_WALL:
	case GameObject::GO_MWALL:
	{
		Vector3 dis = go2->pos - go1->pos;
		Vector3 N = go2->normal;
		if (dis.Dot(N) < 0)
		{
			N = -1 * N;
		}
		Vector3 NP(N.y, -1 * N.x, 0);

		if (dis.Dot(N) < go1->scale.x + go2->scale.x
			&& abs(dis.Dot(NP)) < go2->scale.y
			&& go1->vel.Dot(N) > 0)
		{
			return true;
		}
		return false;
	}
	break;
	case GameObject::GO_SPRING:
	{
		Vector3 dis = go2->pos - go1->pos;
		Vector3 N = go2->normal;
		if (dis.Dot(N) < 0)
		{
			N = -1 * N;
		}
		Vector3 NP(N.y, -1 * N.x, 0);

		if (dis.Dot(N) < go1->scale.x + go2->scale.x
			&& abs(dis.Dot(NP)) < go2->scale.y)
			//&& go1->vel.Dot(N) > 0)
		{
			return true;
		}
		return false;
	}
	break;
	case GameObject::GO_RWALL:
	{
		float angle = atan2(go2->normal.y, go2->normal.x);
		float theta = Math::DegreeToRadian(go2->rotateZ);
		float angle90 = Math::DegreeToRadian(90);
		Vector3 N(cos(theta + angle), sin(theta + angle), 0);
		Vector3 plane(cos(theta + angle + angle90), sin(theta + angle + angle90), 0);
		Vector3 wallpos(go2->pos.x + go2->scale.y * plane.x, go2->pos.y + go2->scale.y * plane.y, 0);
		Vector3 dis = wallpos - go1->pos;
		N *= -1;
		Vector3 w0_b1 = go2->pos - go1->pos;
		Vector3 proj = w0_b1.Dot(plane) * plane;
		float displacement = proj.Length();
		Vector3 Fvel = Math::DegreeToRadian(go2->angularVelocity) * displacement * N;
		if (dis.Dot(N) < 0)
		{
			N = -1 * N;
		}
		Vector3 NP(N.y, -1 * N.x, 0);

		if (dis.Dot(N) < go1->scale.x + go2->scale.x
			&& abs(dis.Dot(NP)) < go2->scale.y
			&& (go1->vel - Fvel).Dot(N) > 0)
		{
			return true;
		}
		return false;
	}
	break;
	case GameObject::GO_ONESIDEDWALL:
	case GameObject::GO_BWALL:
	{
		Vector3 dis = go2->pos - go1->pos;
		Vector3 N = go2->normal;
		Vector3 NP(N.y, -1 * N.x, 0);

		if (abs(dis.Dot(N)) < go1->scale.x + go2->scale.x
			&& abs(dis.Dot(NP)) < go2->scale.y
			&& go1->vel.Dot(N) > 0)
		{
			return true;
		}
		return false;
	}
	break;
	case GameObject::GO_MARKER:
	{
		if (go1->pos.x + go1->scale.x < go2->pos.x + go2->scale.x
			&& go1->pos.x - go1->scale.x > go2->pos.x - go2->scale.x
			&& go1->pos.y + go1->scale.y < go2->pos.y + go2->scale.y)
		{
			return true;
		}
		return false;
	}
	break;
	case GameObject::GO_PILLAR:
	case GameObject::GO_BPILLAR:
	case GameObject::GO_EPILLAR:
	case GameObject::GO_MPILLAR:
	{
		Vector3 u = go1->vel;
		Vector3 go2pos(go2->pos.x, go2->pos.y, 0);
		Vector3 dis = go2pos - go1->pos;
		if (dis.Length() < go1->scale.x + go2->scale.x
			&& u.Dot(dis) > 0)
		{
			return true;
		}
		return false;
	}
	break;
	case GameObject::GO_RPILLAR:
	{
		Vector3 u = go1->vel;
		float angle = atan2(go2->dir.y, go2->dir.x);
		float theta = Math::DegreeToRadian(go2->rotateZ);
		float angle90 = Math::DegreeToRadian(90);
		Vector3 oN(cos(theta + angle + angle90), sin(theta + angle + angle90), 0); // normal of rwall 
		Vector3 plane(cos(angle + theta), sin(angle + theta), 0);
		Vector3 go2pos(go2->pos.x + 8.0f * plane.x, go2->pos.y + 8.0f * plane.y, 0);

		Vector3 dis = go2pos - go1->pos;
		Vector3 N = (go2pos - go1->pos).Normalized();

		// velocity of rpillar
		Vector3 w0_b1 = go2->pos - go1->pos;
		Vector3 proj = w0_b1.Dot(plane) * plane;
		float displacement = proj.Length();
		Vector3 Fvel = Math::DegreeToRadian(go2->angularVelocity) * displacement * oN;

		if (dis.Length() < go1->scale.x + go2->scale.x
			&& (u - Fvel).Dot(dis) > 0)
		{
			return true;
		}
		return false;
	}
	break;
	case GameObject::GO_BONUS:
	{
		Vector3 dis = go2->pos - go1->pos;
		Vector3 N = go2->normal;
		Vector3 NP(N.y, -1 * N.x, 0);

		if (abs(dis.Dot(N)) < go1->scale.x + go2->scale.y * 0.5
			&& abs(dis.Dot(NP)) < go2->scale.x * 0.5
			&& go1->vel.Dot(N) > 0)
		{
			return true;
		}
		return false;
	}
	break;
	case GameObject::GO_FLIPPER:
	{

		if (GetContactWithFlipper(go1, go2) == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	break;
	default:
		break;
	}
	return false;
}

void SceneCollision::CollisionResponse(GameObject* go1, GameObject* go2, float dt)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{

		int m1 = go1->mass;
		int m2 = go2->mass;
		Vector3 u1 = go1->vel;
		Vector3 u2 = go2->vel;

		Vector3 N = (go1->pos - go2->pos).Normalized();
		go1->vel = u1 + (2 * m2 / (m1 + m2)) * ((u2 - u1).Dot(N)) * N;
		go2->vel = u2 + (2 * m1 / (m1 + m2)) * ((u1 - u2).Dot(N)) * N;
		ApplyFriction(go1, N, dt);
		ApplyFriction(go2, N, dt);

	}
	break;
	case GameObject::GO_100:
	{
		Vector3 N = (go2->pos - go1->pos).Normalized();
		ApplyFriction(go1, N, dt);

		go1->vel = go1->vel - 2 * go1->vel.Dot(N) * N;

		go2->timeout = 1;
		go2->fireInterval = 0.3f;
		// scoring stuff
		m_score += 100;
		messageList.push_back(new Message("You earned 100 points!", Color(1.0f, 1.0f, 1.0f)));
	}
	break;
	case GameObject::GO_50:
	{
		Vector3 N = (go2->pos - go1->pos).Normalized();
		ApplyFriction(go1, N, dt);

		go1->vel = go1->vel - 2 * go1->vel.Dot(N) * N;

		go2->timeout = 1;
		go2->fireInterval = 0.3f;
		m_score += 50;
		messageList.push_back(new Message("You earned 50 points!", Color(1.0f, 1.0f, 1.0f)));
	}
	break;
	case GameObject::GO_10:
	{
		Vector3 N = (go2->pos - go1->pos).Normalized();
		ApplyFriction(go1, N, dt);

		go1->vel = go1->vel - 2 * go1->vel.Dot(N) * N;

		go2->timeout = 1;
		go2->fireInterval = 0.3f;
		m_score += 10;
		messageList.push_back(new Message("You earned 10 points!", Color(1.0f, 1.0f, 1.0f)));
	}
	break;
	case GameObject::GO_WALL:
	case GameObject::GO_ONESIDEDWALL:
	{
		ApplyFriction(go1, go2->normal, dt);
		go1->vel = go1->vel - 2 * go1->vel.Dot(go2->normal) * (go2->normal);
		ApplyInelastic(go1, go2->normal, dt);
	}
	break;
	case GameObject::GO_BWALL:
	{
		ApplyFriction(go1, go2->normal, dt);
		go1->vel = go1->vel - 2 * (go1->vel - go2->vel).Dot(go2->normal) * (go2->normal);
		ApplyBumper(go1, go2->normal, dt);
	}
	break;
	case GameObject::GO_MWALL:
	{
		ApplyFriction(go1, go2->normal, dt);
		Vector3 Rvel = go1->vel - go2->vel;
		go1->vel = go1->vel - 2 * Rvel.Dot(go2->normal) * (go2->normal);
		ApplyInelastic(go1, go2->normal, dt);
	}
	break;
	case GameObject::GO_SPRING:
	{
		ApplyFriction(go1, go2->normal, dt);
		go1->vel = go1->vel - 2 * (go1->vel - go2->vel).Dot(go2->normal) * (go2->normal);
	}
	break;
	case GameObject::GO_PILLAR:
	{
		Vector3 go2pos(go2->pos.x, go2->pos.y, 0);
		Vector3 N = (go2pos - go1->pos).Normalized();
		ApplyFriction(go1, N, dt);
		go1->vel = go1->vel - 2 * go1->vel.Dot(N) * N;
		ApplyInelastic(go1, N, dt);

	}
	break;
	case GameObject::GO_MPILLAR:
	{
		Vector3 go2pos(go2->pos.x, go2->pos.y, 0);
		Vector3 N = (go2pos - go1->pos).Normalized();
		ApplyFriction(go1, N, dt);
		go1->vel = go1->vel - 2 * (go1->vel - go2->vel).Dot(N) * N;
		ApplyInelastic(go1, N, dt);

	}
	break;
	case GameObject::GO_BPILLAR:
	{
		Vector3 go2pos(go2->pos.x, go2->pos.y, 0);
		Vector3 N = (go2pos - go1->pos).Normalized();
		ApplyFriction(go1, N, dt);
		go1->vel = go1->vel - 2 * go1->vel.Dot(N) * N;
		ApplyBumper(go1, N, dt);
	}
	break;
	case GameObject::GO_EPILLAR:
	{
		Vector3 go2pos(go2->pos.x, go2->pos.y, 0);
		Vector3 N = (go2pos - go1->pos).Normalized();
		ApplyFriction(go1, N, dt);
		go1->vel = go1->vel - 2 * (go1->vel - go2->vel).Dot(N) * N;
	}
	break;
	case GameObject::GO_FLIPPER:
	{
		// vars to be used
		float theta = Math::DegreeToRadian(go2->rotateZ);
		float innerR = atan2(f_radius1 - f_radius2, f_length);
		float angle90 = Math::DegreeToRadian(90);

		Vector3 Fplane(cos(theta), sin(theta), 0);
		Vector3 Fnormal(cos(theta + angle90), sin(theta + angle90), 0);
		Vector3 wall1plane(cos(theta - innerR), sin(theta - innerR), 0);
		Vector3 wall1normal(cos(theta - innerR + angle90), sin(theta - innerR + angle90), 0);
		Vector3 wall2plane(cos(theta + innerR), sin(theta + innerR), 0);
		Vector3 wall2normal(cos(theta + innerR - angle90), sin(theta + innerR - angle90), 0);

		Vector3 Rvel;

		// calculating flipper vel and hence relative vel
		Vector3 w0_b1 = go2->pos - go1->pos;
		Vector3 proj = w0_b1.Dot(Fplane) * Fplane;
		float displacement = proj.Length();
		Vector3 Fvel = Math::DegreeToRadian(go2->angularVelocity) * displacement * Fnormal;
		Rvel = go1->vel - Fvel;


		switch (GetContactWithFlipper(go1, go2))
		{
		case 0:
			return;
		case 1: // wall in dir of flipper normal
		{
			ApplyFriction(go1, wall1normal, dt);
			go1->vel = go1->vel - 2 * Rvel.Dot(wall1normal) * (wall1normal);
			ApplyInelastic(go1, wall1normal, dt);
			break;
		}
		case 2: // other wall
		{
			ApplyFriction(go1, wall2normal, dt);
			go1->vel = go1->vel - 2 * Rvel.Dot(wall2normal) * (wall2normal);
			ApplyInelastic(go1, wall2normal, dt);
			break;
		}
		case 3: //pillar at pivot
		{
			Vector3 N = (go2->pos - go1->pos).Normalized();
			ApplyFriction(go1, N, dt);
			go1->vel = go1->vel - 2 * Rvel.Dot(N) * N;
			ApplyInelastic(go1, N, dt);
			break;
		}
		case 4: // pillar at edge
		{
			Vector3 pillarPos = go2->pos + f_length * Fplane;
			Vector3 N = (pillarPos - go1->pos).Normalized();
			ApplyFriction(go1, N, dt);
			go1->vel = go1->vel - 2 * Rvel.Dot(N) * N;
			ApplyInelastic(go1, N, dt);
			break;
		}
		}


	}
	break;
	case GameObject::GO_RWALL:
	{
		float angle = atan2(go2->normal.y, go2->normal.x);
		float theta = Math::DegreeToRadian(go2->rotateZ);
		float angle90 = Math::DegreeToRadian(90);
		Vector3 N(cos(theta + angle), sin(theta + angle), 0);
		Vector3 plane(cos(theta + angle + angle90), sin(theta + angle + angle90), 0);

		Vector3 Rvel;
		N *= -1; // set normal based on dir of angular vel
		Vector3 w0_b1 = go2->pos - go1->pos;
		Vector3 proj = w0_b1.Dot(plane) * plane;
		float displacement = proj.Length();
		Vector3 Fvel = Math::DegreeToRadian(go2->angularVelocity) * displacement * N;
		Rvel = go1->vel - Fvel;

		ApplyFriction(go1, N, dt);
		go1->vel = go1->vel - 2 * Rvel.Dot(N) * (N);
		ApplyInelastic(go1, N, dt);
	}
	break;
	case GameObject::GO_RPILLAR:
	{
		// vars to be used
		float angle = atan2(go2->dir.y, go2->dir.x);
		float theta = Math::DegreeToRadian(go2->rotateZ);
		float angle90 = Math::DegreeToRadian(90);
		Vector3 plane(cos(angle + theta), sin(angle + theta), 0);
		Vector3 oN(cos(angle + theta + angle90), sin(angle + theta + angle90), 0); // wall's normal
		Vector3 go2pos(go2->pos.x + 8.0f * plane.x, go2->pos.y + 8.0f * plane.y, 0);
		Vector3 N = (go2pos - go1->pos).Normalized();

		Vector3 w0_b1 = go2->pos - go1->pos;
		Vector3 proj = w0_b1.Dot(plane) * plane;
		float displacement = proj.Length();
		Vector3 Fvel = Math::DegreeToRadian(go2->angularVelocity) * displacement * oN;
		Vector3 Rvel = go1->vel - Fvel;

		ApplyFriction(go1, N, dt);
		go1->vel = go1->vel - 2 * Rvel.Dot(N) * N;
		ApplyInelastic(go1, N, dt);
	}
	break;
	case GameObject::GO_MARKER:
	{
		if (go2 != topPipe)
		{
			go1->pos.Set(topPipe->pos.x, topPipe->pos.y, 0);
		}
	}
	break;
	case GameObject::GO_BONUS:
	{
		ApplyFriction(go1, go2->normal, dt);
		go1->vel = go1->vel - 2 * go1->vel.Dot(go2->normal) * (go2->normal);
		go2->mat.kAmbient.Set(0.2f, 0.2f, 1.0f);
		if (go2->timeout <= 0)
		{
			go2->timeout = 1;
			go2->fireInterval = 0.0f;
		}
		messageList.push_back(new Message("Bonus Bumper has been hit!", Color(1.0f, 0.5f, 0.8f)));
	}
	break;
	default:
		break;
	}
}

void SceneCollision::Constraint(GameObject* ball, GameObject* other)
{
	Vector3 iN;
	float combinedR = ball->scale.x + other->scale.x;
	Vector3 otherPos = other->pos;

	if (other->type == GameObject::GO_PILLAR || other->type == GameObject::GO_BPILLAR 
		|| other->type == GameObject::GO_EPILLAR || other->type == GameObject::GO_MPILLAR)
	{
		iN = (ball->pos - other->pos).Normalized();
	}
	else if (other->type == GameObject::GO_RPILLAR)
	{
		float angle = atan2(other->dir.y, other->dir.x);
		float theta = Math::DegreeToRadian(other->rotateZ);
		otherPos.Set(other->pos.x + 8.0f * cos(angle + theta), other->pos.y + 8.0f * sin(angle + theta), 0);
		iN = (ball->pos - otherPos).Normalized();
	}
	else if (other->type == GameObject::GO_WALL || other->type == GameObject::GO_ONESIDEDWALL 
		|| other->type == GameObject::GO_MWALL || other->type == GameObject::GO_BWALL)
	{
		if (ball->vel.Dot(other->normal) < 0)
		{
			iN = other->normal;
		}
		else
		{
			iN = -1 * other->normal;
		}
	}
	else if (other->type == GameObject::GO_RWALL)
	{
		float angle = atan2(other->normal.y, other->normal.x);
		float theta = Math::DegreeToRadian(other->rotateZ);
		float angle90 = Math::DegreeToRadian(90);
		Vector3 wallpos(other->pos.x + other->scale.y * cos(angle + theta + angle90), other->pos.y + other->scale.y * sin(angle + theta + angle90), 0);
		otherPos = wallpos;
		iN.Set(cos(theta + angle), sin(theta + angle), 0);
		Vector3 dis = wallpos - ball->pos;
		if (dis.Dot(iN) > 0)
		{
			iN *= -1;
		}
	}
	else if (other->type == GameObject::GO_SPRING)
	{
		if (ball->vel.Dot(other->normal) < 0)
		{
			iN = other->normal;
		}
		else
		{
			iN = -1 * other->normal;
		}
	}
	else if (other->type == GameObject::GO_BALL || other->type == GameObject::GO_100
		|| other->type == GameObject::GO_50 || other->type == GameObject::GO_10)
	{
		iN = (ball->pos - other->pos).Normalized();
	}
	else if (other->type == GameObject::GO_BONUS)
	{
		iN = -1 * other->normal;
		otherPos = other->pos;
		otherPos.y -= other->scale.y * 0.5;
		combinedR = ball->scale.x;
	}
	else if (other->type == GameObject::GO_FLIPPER)
	{
		float theta = Math::DegreeToRadian(other->rotateZ);
		float innerR = atan2(f_radius1 - f_radius2, f_length);
		float angle90 = Math::DegreeToRadian(90);
		Vector3 Fplane(cos(theta), sin(theta), 0);
		Vector3 Fnormal(cos(theta + angle90), sin(theta + angle90), 0);
	
		// checking  wall
		switch (GetContactWithFlipper(ball, other))
		{
		case 0: // no collision
			return;
		case 1: // wall 1
		{
			Vector3 wall1plane(cos(theta - innerR), sin(theta - innerR), 0);
			Vector3 wall1normal(cos(theta - innerR + angle90), sin(theta - innerR + angle90), 0);
			iN = wall1normal;

			
			otherPos = other->pos + f_length * Fplane - wall1plane * f_length * 0.5;
			combinedR = ball->scale.x + f_radius2;
		}
			break;
		case 2: // wall 2
		{
			Vector3 wall2plane(cos(theta + innerR), sin(theta + innerR), 0);
			Vector3 wall2normal(cos(theta + innerR - angle90), sin(theta + innerR - angle90), 0);
			iN = wall2normal;
		
			otherPos = other->pos + f_length * Fplane - wall2plane * f_length * 0.5;
			combinedR = ball->scale.x + f_radius2;
		}
			break;
		case 3: // pillar at pivot
			iN = (ball->pos - other->pos).Normalized();
			combinedR = ball->scale.x + f_radius1;
			break;
		case 4: // pillar at edge
		{
			Vector3 Fplane(cos(theta), sin(theta), 0);
			otherPos = other->pos + Fplane * f_length;
			iN = (ball->pos - otherPos).Normalized();
			combinedR = ball->scale.x + f_radius2;
		}
			break;
		}
	}

	
	Vector3 w0_b1 = otherPos - ball->pos;
	float projection = abs(w0_b1.Dot(iN));
	if (projection < combinedR)
	{
		Vector3 pVector = projection * -1 * iN;
		Vector3 disFromCentre = pVector - w0_b1;
		ball->pos = otherPos + disFromCentre + iN * (combinedR);
	}
	else
	{
		return;
	}
}

void SceneCollision::ApplyFriction(GameObject* ball, Vector3 normal, double dt)
{
	// - velocity parallel to surface by amount of Nforce
	const float FRICTION_K = 0.0005f;
	float Nforce = abs(Vector3(ball->mass * m_gravity).Dot(normal));
	Vector3 plane = normal.Cross(Vector3(0, 0, 1));
	if (ball->vel.Dot(plane) < 0)
	{
		plane *= -1;
	}
	ball->vel -= FRICTION_K * Nforce * plane * m_speed * dt;
}

void SceneCollision::ApplyBumper(GameObject* ball, Vector3 normal, double dt)
{
	// + 100% of velocity in the direction of the normal
	Vector3 iN = normal;
	if (ball->vel.Dot(iN) < 0)
	{
		iN = -1 * iN;
	}
	Vector3 proj = ball->vel.Dot(iN) * iN;
	ball->vel += 1.0f * proj;
}

void SceneCollision::ApplyInelastic(GameObject* ball, Vector3 normal, double dt)
{
	// - 50% of velocity in the direction of the normal
	Vector3 iN = normal;
	if (ball->vel.Dot(iN) < 0)
	{
		iN = -1 * iN;
	}
	Vector3 proj = ball->vel.Dot(iN) * iN;
	ball->vel -= 0.5f * proj;
}

int SceneCollision::GetContactWithFlipper(GameObject* ball, GameObject* flipper)
{
	// angles to be used
	float theta = Math::DegreeToRadian(flipper->rotateZ);
	float innerR = atan2(f_radius1 - f_radius2, f_length);
	float angle90 = Math::DegreeToRadian(90);
	// vectors to be used
	Vector3 Fplane(cos(theta), sin(theta), 0);
	Vector3 Fnormal(cos(theta + angle90), sin(theta + angle90), 0);
	Vector3 wall1plane(cos(theta - innerR), sin(theta - innerR), 0);
	Vector3 wall1normal(cos(theta - innerR + angle90), sin(theta - innerR + angle90), 0);
	Vector3 wall2plane(cos(theta + innerR), sin(theta + innerR), 0);
	Vector3 wall2normal(cos(theta + innerR - angle90), sin(theta + innerR - angle90), 0);
	// finding vel of flipper
	Vector3 w0_b1 = flipper->pos - ball->pos;
	Vector3 proj = (w0_b1.Dot(Fplane) / Fplane.Dot(Fplane)) * Fplane;
	float displacement = proj.Length();
	Vector3 Fvel = Math::DegreeToRadian(flipper->angularVelocity) * displacement * Fnormal;
	// vars to be used
	Vector3 wallpos, dis, N, NP;

	// checking wall in dir of normal
	wallpos = flipper->pos + f_length * Fplane - wall1plane * f_length * 0.5;
	dis = wallpos - ball->pos;
	N = -1 * wall1normal;
	NP.Set(N.y, -1 * N.x, 0);

	if (abs(dis.Dot(N)) < ball->scale.x + f_radius2
		&& abs(dis.Dot(NP)) < f_length * 0.5
		&& (ball->vel - Fvel).Dot(N) > 0)
	{
		return 1;
	}


	// checking other wall
	wallpos = flipper->pos + f_length * Fplane - wall2plane * f_length * 0.5;
	dis = wallpos - ball->pos;
	N = -1 * wall2normal;
	NP.Set(N.y, -1 * N.x, 0);

	if (abs(dis.Dot(N)) < ball->scale.x + f_radius2
		&& abs(dis.Dot(NP)) < f_length * 0.5
		&& (ball->vel - Fvel).Dot(N) > 0)
	{
		return 2;
	}

	// checking both pillars
	// larger pillar at pivot
	dis = flipper->pos - ball->pos;
	if (dis.Length() < ball->scale.x + f_radius1
		&& ball->vel.Dot(dis) > 0)
	{
		return 3;
	}
	// smaller pillar at end
	dis = flipper->pos + Fplane * f_length - ball->pos;
	if (dis.Length() < ball->scale.x + f_radius2
		&& ball->vel.Dot(dis) > 0)
	{
		return 4;
	}

	return 0;
}

bool SceneCollision::CheckZero(double number)
{
	if (number - 0 < Math::EPSILON || 0 - number > Math::EPSILON)
	{
		return true;
	}
	return false;
}

void SceneCollision::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_BALL:
		meshList[GEO_BALL]->material.kAmbient = go->mat.kAmbient;
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		modelStack.Rotate(90, 1, 0, 0);
		RenderMesh(meshList[GEO_BALL], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_WALL:
	case GameObject::GO_SPRING:
	case GameObject::GO_ONESIDEDWALL:
	case GameObject::GO_MWALL:
	case GameObject::GO_BWALL:
	{
		float angle = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
		meshList[GEO_WALL]->material.kAmbient = go->mat.kAmbient;
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WALL], true);
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_RWALL:
	{
		float angle = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
		meshList[GEO_WALL]->material.kAmbient = go->mat.kAmbient;
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(angle + go->rotateZ, 0, 0, 1);
		modelStack.Translate(0, go->scale.y, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WALL], true);
		modelStack.PopMatrix();
	}
		break;
	case GameObject::GO_MARKER:
	{
		meshList[GEO_WALL]->material.kAmbient = go->mat.kAmbient;
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 0.0001);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WALL], true);
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_PILLAR:
	case GameObject::GO_BPILLAR:
	case GameObject::GO_EPILLAR:
	case GameObject::GO_MPILLAR:
		meshList[GEO_BALL]->material.kAmbient = go->mat.kAmbient;
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		modelStack.Rotate(90, 1, 0, 0);
		RenderMesh(meshList[GEO_BALL], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_RPILLAR:
		meshList[GEO_BALL]->material.kAmbient = go->mat.kAmbient;
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);

		modelStack.Rotate(go->rotateZ, 0, 0, 1);
		modelStack.Translate(go->dir.x * 8.0f, go->dir.y * 8.0f, 0);

		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		modelStack.Rotate(90, 1, 0, 0);
		RenderMesh(meshList[GEO_BALL], true);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_BONUS:
		meshList[GEO_BALL]->material.kAmbient = go->mat.kAmbient;
		meshList[GEO_WALL]->material.kAmbient = go->mat.kAmbient;

		// left right wall
		for (int i = -1; i < 2; i += 2)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x + i * go->scale.x * 0.25, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x * 0.25, (0.8 * go->scale.y) * 0.5, 1);
			RenderMesh(meshList[GEO_WALL], true);
			modelStack.PopMatrix();
		}
		// top bot wall
		for (int i = -1; i < 2; i += 2)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y + i * go->scale.y * 0.25, go->pos.z);
			modelStack.Scale((0.8 * go->scale.x) * 0.5, go->scale.y * 0.25, 1);
			RenderMesh(meshList[GEO_WALL], true);
			modelStack.PopMatrix();
		}
		// pillars - top 2
		for (int i = -1; i < 2; i += 2)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x + i * go->scale.x * 0.4 , go->pos.y + i * go->scale.y * 0.4, go->pos.z);
			modelStack.Scale(0.1 * go->scale.x, 0.1 * go->scale.y, 1);
			modelStack.Rotate(90, 1, 0, 0);
			RenderMesh(meshList[GEO_BALL], true);
			modelStack.PopMatrix();
		}
		// pillars - bot 2
		for (int i = -1; i < 2; i += 2)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x + i * go->scale.x * 0.4, go->pos.y - i * go->scale.y * 0.4, go->pos.z);
			modelStack.Scale(0.1 * go->scale.x, 0.1 * go->scale.y, 1);
			modelStack.Rotate(90, 1, 0, 0);
			RenderMesh(meshList[GEO_BALL], true);
			modelStack.PopMatrix();
		}
		break;
	case GameObject::GO_FLIPPER:
	{
		Vector3 temp(f_length, f_radius1 - f_radius2, 0);
		float wall_length = temp.Length();
		meshList[GEO_BALL]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);
		meshList[GEO_WALL]->material.kAmbient.Set(0.8f, 0.8f, 0.8f);

		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(go->rotateZ, 0, 0, 1);

			modelStack.PushMatrix();
			modelStack.Scale(f_radius1, f_radius1, f_radius1);
			modelStack.Rotate(90, 1, 0, 0);
			RenderMesh(meshList[GEO_BALL], true);
			modelStack.PopMatrix();

			float theta = Math::RadianToDegree(atan2(f_radius1 - f_radius2, f_length));

			modelStack.PushMatrix();
			modelStack.Translate(f_length, 0, 0);
				modelStack.Rotate(90 - theta, 0, 0, 1);
				modelStack.Translate(0, wall_length * 0.5, 0);
			modelStack.Scale(f_radius2, wall_length * 0.5, 1);
			RenderMesh(meshList[GEO_WALL], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(f_length, 0, 0);
				modelStack.Rotate(90 + theta, 0, 0, 1);
				modelStack.Translate(0, wall_length * 0.5, 0);
			modelStack.Scale(f_radius2, wall_length * 0.5, 1);
			RenderMesh(meshList[GEO_WALL], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(f_length, 0, 0);
			modelStack.Scale(f_radius2, f_radius2, f_radius2);
			modelStack.Rotate(90, 1, 0, 0);
			RenderMesh(meshList[GEO_BALL], true);
			modelStack.PopMatrix();

		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_100:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_100], false);
		modelStack.PopMatrix();

		if (go->timeout > 0)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z - 0.0001);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(go->scale.x * 1.1, go->scale.y * 1.1, go->scale.z * 1.1);
			RenderMesh(meshList[GEO_HIGHLIGHT], false);
			modelStack.PopMatrix();
		}
		break;
	case GameObject::GO_50:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_50], false);
		modelStack.PopMatrix();

		if (go->timeout > 0)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z - 0.0001);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(go->scale.x * 1.1, go->scale.y * 1.1, go->scale.z * 1.1);
			RenderMesh(meshList[GEO_HIGHLIGHT], false);
			modelStack.PopMatrix();
		}
		break;
	case GameObject::GO_10:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_10], false);
		modelStack.PopMatrix();

		if (go->timeout > 0)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z - 0.0001);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(go->scale.x * 1.1, go->scale.y * 1.1, go->scale.z * 1.1);
			RenderMesh(meshList[GEO_HIGHLIGHT], false);
			modelStack.PopMatrix();
		}
		break;
	default:
		break;
	}
}

void SceneCollision::Render()
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

	if (m_ghost->active)
	{
		RenderGO(m_ghost);
	}



	// all gos
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	//ui frame
	float scalex = ((m_worldWidth - m_gameWidth) / m_worldWidth) * 80;
	float posx = 0.875 * 80;
	RenderMeshOnScreen(meshList[GEO_UIFRAME], posx, 30, scalex, 60);
	std::ostringstream ss;


	// balls left
	RenderTextOnScreen(meshList[GEO_TEXT],"Balls Left:" , Color(1.0f, 1.0f, 1.0f), 4, m_gameWidth + 7.0f, m_worldHeight * 0.77f);
	for (int i = 0; i < numBalls; i++)
	{
		Vector3 pos;
		pos.x = camera.target.x + 38.0f + i * 5.0f;
		pos.y = camera.target.y + 17.0f;

		meshList[GEO_BALL]->material.kAmbient.Set(1.0f, 1.0f, 1.0f);
		modelStack.PushMatrix();
		modelStack.Translate(pos.x, pos.y, 0.001);
		modelStack.Scale(2.0f, 2.0f, 2.0f);
		modelStack.Rotate(90, 1, 0, 0);
		RenderMesh(meshList[GEO_BALL], true);
		modelStack.PopMatrix();
	}
	// score
	ss.str("");
	ss << "Score: " << m_score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.0f, 1.0f, 1.0f), 4, m_gameWidth + 7.0f, m_worldHeight * 0.67f);
	// high score
	ss.str("");
	ss << "High Score: " << m_highscore;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.0f, 1.0f, 1.0f), 4, m_gameWidth + 7.0f, m_worldHeight * 0.6f);

	// message
	float total = messageList.size();
	for (auto popup : messageList)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], popup->text, popup->col, 3, m_gameWidth + 7.0f, m_worldHeight * 0.2f + (total * 6.0f), 20);
		total--;
	}

	// fps tings
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 3);

	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(1, 1, 1), 3, 0, 0);
}

void SceneCollision::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	while (wallList.size() > 0)
	{
		Wall* go = wallList.back();
		delete go;
		wallList.pop_back();
	}
	while (pillarList.size() > 0)
	{
		Pillar* go = pillarList.back();
		delete go;
		pillarList.pop_back();
	}
	while (messageList.size() > 0)
	{
		Message* go = messageList.back();
		delete go;
		messageList.pop_back();
	}
	if (m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
