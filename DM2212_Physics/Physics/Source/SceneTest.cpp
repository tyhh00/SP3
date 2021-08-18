﻿#include "SceneTest.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"
#include "Utility.h"
#include <sstream>

SceneTest::SceneTest()
{
}

SceneTest::~SceneTest()
{
}

void SceneTest::Init()
{
	SceneBase::Init();

	// Calculating aspect ratio
	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_worldHeight = m_screenHeight * 3;
	m_worldWidth = m_screenWidth * 5;

	//Physics code here
	m_speed = 1.f;
	m_objectCount = 0;

	Math::InitRNG();

	meshList[GEO_WALL] = MeshBuilder::GenerateQuad("Wall", Color(1, 1, 1), 2.0f);
	meshList[GEO_BALL] = MeshBuilder::GenerateCircle("circle", 1.0f, Color(1, 1, 1));
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("light", Color(1, 1, 1), 36, 36, 0.1f);
	

	goManager = new GameObjectManager();
	goManager->Init();

	player = new Player;
	player->active = true;
	player->scale.Set(3, 3, 1);
	player->pos.Set(m_worldWidth * 0.5, m_worldHeight * 0.5, 0);
	player->Init();
	player->AddBottomSprite();
	player->bottomSprite->mesh = meshList[GEO_WALL];
	goManager->AddGO(player);


	testobj = new GameObject;
	testobj->active = true;
	testobj->type = GameObject::GO_BALL;
	testobj->scale.Set(5, 5, 5);
	testobj->pos.Set(player->pos.x + 100, player->pos.y, player->pos.z);
	testobj->mesh = meshList[GEO_BALL];
	goManager->AddGO(testobj);

	testWall = new GameObject;
	testWall->active = true;
	testWall->type = GameObject::GO_WALL;
	testWall->pos.Set(player->pos.x, player->pos.y - 20, 0);
	testWall->scale.Set(5, 100, 1);
	testWall->physics->SetVelocity(Vector3(1, 0, 0));
	testWall->physics->shapeType = RECTANGLE;
	testWall->physics->SetNormal(Vector3(0, 1, 0));
	testWall->mesh = meshList[GEO_WALL];
	goManager->AddGO(testWall);

	GameObject* go;
	go = new GameObject;
	go->active = true;
	go->type = GameObject::GO_WALL;
	go->pos.Set(player->pos.x - 10, player->pos.y - 15, 0);
	go->scale.Set(5, 5, 1);
	go->physics->SetVelocity(Vector3(1, 0, 0));
	go->physics->shapeType = RECTANGLE;
	go->physics->SetNormal(Vector3(1, 0, 0));
	go->mesh = meshList[GEO_WALL];
	goManager->AddGO(go);

	go = new GameObject;
	go->active = true;
	go->type = GameObject::GO_WALL;
	go->pos.Set(player->pos.x + 10, player->pos.y - 15, 0);
	go->scale.Set(5, 5, 1);
	go->physics->SetVelocity(Vector3(1, 0, 0));
	go->physics->shapeType = RECTANGLE;
	go->physics->SetNormal(Vector3(1, 0, 0));
	go->mesh = meshList[GEO_WALL];
	goManager->AddGO(go);

	
	lights[0].type = Light::LIGHT_POINT;
	lights[0].position.Set(player->pos.x, player->pos.y, player->pos.z + 10);
	lights[0].color.Set(1, 1, 0.9);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 1.f;
	lights[0].spotDirection.Set(0.f, 0.f, 1.f);

	lights[1].type = Light::LIGHT_SPOT;
	lights[1].position.Set(0, 1, 0);
	lights[1].color.Set(0.6, 0.6, 1);
	lights[1].power = 1;
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

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.SetFocusTarget(player->pos);
	camera.SetLimits(m_screenWidth, m_screenHeight, m_worldWidth, m_worldHeight);
}

GameObject* SceneTest::FetchGO()
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

GameObject* SceneTest::FetchGO(GameObject::GAMEOBJECT_TYPE type)
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

GameObject* SceneTest::FetchGO(bool isMovable)
{

	// Fetch a game object from m_goList and return it
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (!go->active)
		{
			go->active = true;
			go->physics->SetMovable(isMovable);
			return go;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		GameObject* temp = new GameObject;
		m_goList.push_back(temp);
	}


	return FetchGO(isMovable);
}

void SceneTest::ReturnGO(GameObject *go)
{
	go->active = false;
	m_objectCount--;
}

void SceneTest::Update(double dt)
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
		camera.ToggleAutoLock(true);
		//camera.SetFocusTarget(player->pos, true);

	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}

	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
		camera.ToggleAutoLock(false);
		camera.SetFocusTarget(testobj->pos, true);
		
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

	}

	if (Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(3 * dt);
	if (Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(3 * dt);
	if (Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(3 * dt);
	if (Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(3 * dt);
	if (Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(3 * dt);
	if (Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(3 * dt);

	goManager->Update(dt);
	camera.Update(player->pos, dt);
	lights[0].position.Set(player->pos.x, player->pos.y, player->pos.z + 10);
	double mouseposx, mouseposy;
	CursorToWorldPosition(mouseposx, mouseposy);
	lights[1].position.Set(mouseposx, mouseposy, 10);
}

bool SceneTest::CheckCollision(GameObject* go1, GameObject* go2, float dt)
{
	// in case of self collision
	/*if (go1 == go2)
	{
		return false;
	}*/
	if (go1->physics->shapeType == CIRCLE)
	{
		switch (go2->physics->shapeType)
		{
		case CIRCLE:
		{
			Vector3 dis = go2->pos - go1->pos;
			float disSquared = dis.LengthSquared();
			if (disSquared <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x) && dis.Dot(go1->physics->GetVelocity() - go2->physics->GetVelocity()) > 0)
			{
				return true;
			}
		}
		break;
		case RECTANGLE:
			Vector3 dis = go2->pos - go1->pos;
			Vector3 N = go2->physics->GetNormal();
			if (dis.Dot(N) < 0)
			{
				N = -1 * N;
			}
			Vector3 NP(N.y, -1 * N.x, 0);

			if (dis.Dot(N) < go1->scale.x + go2->scale.x
				&& abs(dis.Dot(NP)) < go2->scale.y
				&& go1->physics->GetVelocity().Dot(N) > 0)
			{
				go2->physics->SetCollisionNormal(N);
				return true;
			}

			N = NP;
			if (dis.Dot(N) < 0)
			{
				N = -1 * N;
			}
			NP.Set(N.y, -1 * N.x, 0);

			if (dis.Dot(N) < go1->scale.y + go2->scale.y
				&& abs(dis.Dot(NP)) < go2->scale.x
				&& go1->physics->GetVelocity().Dot(N) > 0)
			{
				go2->physics->SetCollisionNormal(N);
				return true;
			}
			break;
		}
		return false;
	}
	else
	{
		switch (go2->physics->shapeType)
		{
		case CIRCLE:
		{
			Vector3 dis = go2->pos - go1->pos;
			Vector3 N = go2->physics->GetNormal();
			if (dis.Dot(N) < 0)
			{
				N = -1 * N;
			}
			Vector3 NP(N.y, -1 * N.x, 0);

			if (dis.Dot(N) < go1->scale.x + go2->scale.x
				&& abs(dis.Dot(NP)) < go2->scale.y
				&& go1->physics->GetVelocity().Dot(N) > 0)
			{
				go2->physics->SetCollisionNormal(N);
				return true;
			}

			N = NP;
			if (dis.Dot(N) < 0)
			{
				N = -1 * N;
			}
			NP.Set(N.y, -1 * N.x, 0);

			if (dis.Dot(N) < go1->scale.y + go2->scale.y
				&& abs(dis.Dot(NP)) < go2->scale.x
				&& go1->physics->GetVelocity().Dot(N) > 0)
			{
				go2->physics->SetCollisionNormal(N);
				return true;
			}
		}
		break;
		case RECTANGLE:
			Vector3 dis = go2->pos - go1->pos;
			Vector3 N = go2->physics->GetNormal();
			if (dis.Dot(N) < 0)
			{
				N = -1 * N;
			}
			Vector3 NP(N.y, -1 * N.x, 0);

			if (dis.Dot(N) < go1->scale.x + go2->scale.x
				&& abs(dis.Dot(NP)) < go2->scale.y
				&& go1->physics->GetVelocity().Dot(N) > 0)
			{
				go2->physics->SetCollisionNormal(N);
				return true;
			}

			N = NP;
			if (dis.Dot(N) < 0)
			{
				N = -1 * N;
			}
			NP.Set(N.y, -1 * N.x, 0);

			if (dis.Dot(N) < go1->scale.y + go2->scale.y
				&& abs(dis.Dot(NP)) < go2->scale.x
				&& go1->physics->GetVelocity().Dot(N) > 0)
			{
				go2->physics->SetCollisionNormal(N);
				return true;
			}
			break;
		}
	}


	/*switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		Vector3 dis = go2->pos - go1->pos;
		float disSquared = dis.LengthSquared();
		if (disSquared <= (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x) && dis.Dot(go1->physics->GetVelocity() - go2->physics->GetVelocity()) > 0)
		{
			return true;
		}
		return false;
	}
	break;
	case GameObject::GO_MWALL:
	{
		Vector3 dis = go2->pos - go1->pos;
		Vector3 N = go2->physics->GetNormal();
		if (dis.Dot(N) < 0)
		{
			N = -1 * N;
		}
		Vector3 NP(N.y, -1 * N.x, 0);

		if (dis.Dot(N) < go1->scale.x + go2->scale.x
			&& abs(dis.Dot(NP)) < go2->scale.y
			&& go1->physics->GetVelocity().Dot(N) > 0)
		{
			go2->physics->SetCollisionNormal(N);
			return true;
		}

		N = NP;
		if (dis.Dot(N) < 0)
		{
			N = -1 * N;
		}
		NP.Set(N.y, -1 * N.x, 0);

		if (dis.Dot(N) < go1->scale.x + go2->scale.x
			&& abs(dis.Dot(NP)) < go2->scale.y
			&& go1->physics->GetVelocity().Dot(N) > 0)
		{
			go2->physics->SetCollisionNormal(N);
			return true;
		}
		return false;
	}
	break;
	case GameObject::GO_MPILLAR:
	{
		Vector3 u = go1->physics->GetVelocity();
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
	default:
		break;
	}*/
	return false;
}

void SceneTest::Constraint(GameObject* ball, GameObject* other)
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
		float angle = atan2(other->physics->GetDir().y, other->physics->GetDir().x);
		float theta = Math::DegreeToRadian(other->physics->GetRotateZ());
		otherPos.Set(other->pos.x + 8.0f * cos(angle + theta), other->pos.y + 8.0f * sin(angle + theta), 0);
		iN = (ball->pos - otherPos).Normalized();
	}
	else if (other->type == GameObject::GO_WALL || other->type == GameObject::GO_ONESIDEDWALL 
		|| other->type == GameObject::GO_MWALL || other->type == GameObject::GO_BWALL)
	{
		if (ball->physics->GetVelocity().Dot(other->physics->GetNormal()) < 0)
		{
			iN = other->physics->GetNormal();
		}
		else
		{
			iN = -1 * other->physics->GetNormal();
		}
	}
	else if (other->type == GameObject::GO_RWALL)
	{
		float angle = atan2(other->physics->GetNormal().y, other->physics->GetNormal().x);
		float theta = Math::DegreeToRadian(other->physics->GetRotateZ());
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
		if (ball->physics->GetVelocity().Dot(other->physics->GetNormal()) < 0)
		{
			iN = other->physics->GetNormal();
		}
		else
		{
			iN = -1 * other->physics->GetNormal();
		}
	}
	else if (other->type == GameObject::GO_BALL || other->type == GameObject::GO_100
		|| other->type == GameObject::GO_50 || other->type == GameObject::GO_10)
	{
		iN = (ball->pos - other->pos).Normalized();
	}
	else if (other->type == GameObject::GO_BONUS)
	{
		iN = -1 * other->physics->GetNormal();
		otherPos = other->pos;
		otherPos.y -= other->scale.y * 0.5;
		combinedR = ball->scale.x;
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

bool SceneTest::CheckZero(double number)
{
	if (number - 0 < Math::EPSILON || 0 - number > Math::EPSILON)
	{
		return true;
	}
	return false;
}

void SceneTest::CursorToWorldPosition(double& theX, double& theY)
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

void SceneTest::RenderGO(GameObject *go)
{
	switch(go->type)
	{
	case GameObject::GO_BALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		//modelStack.Rotate(90, 1, 0, 0);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_WALL:
	case GameObject::GO_SPRING:
	case GameObject::GO_ONESIDEDWALL:
	case GameObject::GO_MWALL:
	case GameObject::GO_BWALL:
	{
		float angle = Math::RadianToDegree(atan2(go->physics->GetNormal().y, go->physics->GetNormal().x));
		
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WALL], false);
		modelStack.PopMatrix();
	}
	break;
	case GameObject::GO_RWALL:
	{
		float angle = Math::RadianToDegree(atan2(go->physics->GetNormal().y, go->physics->GetNormal().x));
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(angle + go->physics->GetRotateZ(), 0, 0, 1);
		modelStack.Translate(0, go->scale.y, 0);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WALL], false);
		modelStack.PopMatrix();
	}
		break;
	case GameObject::GO_MARKER:
	{
		meshList[GEO_WALL]->material.kAmbient = go->mat.kAmbient;
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z + 0.0001);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WALL], false);
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
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_RPILLAR:
		meshList[GEO_BALL]->material.kAmbient = go->mat.kAmbient;
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);

		modelStack.Rotate(go->physics->GetRotateZ(), 0, 0, 1);
		modelStack.Translate(go->physics->GetDir().x * 8.0f, go->physics->GetDir().y * 8.0f, 0);

		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		modelStack.Rotate(90, 1, 0, 0);
		RenderMesh(meshList[GEO_BALL], false);
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

void SceneTest::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(-1 * m_screenWidth * 0.5f, m_screenWidth * 0.5f, -1 * m_screenHeight * 0.5f, m_screenHeight * 0.5f, -10, 10);
//	projection.SetToPerspective(45.f, m_screenWidth / m_screenHeight, 0.1f, 1000.f);

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

	RenderMesh(meshList[GEO_AXES], false);

	//modelStack.PushMatrix();
	//modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	//RenderMesh(meshList[GEO_LIGHTBALL], false);
	//modelStack.PopMatrix();

	goManager->Render(this);

	/*modelStack.PushMatrix();
	modelStack.Translate(player->pos.x, player->pos.y, player->pos.z);
	RenderMesh(player->mesh, false);
	modelStack.PopMatrix();*/

	std::ostringstream ss;
	ss.str("");
	ss << "player onground: " << player->physics->GetOnGround();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 6);
	ss.str("");
	ss << "player pos: " << player->pos;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 9);
	ss.str("");
	ss << "camera pos: " << camera.position;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 12);
	


	// fps tings
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 3);

	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(1, 1, 1), 3, 0, 0);
}

void SceneTest::Exit()
{
	SceneBase::Exit();
	

	goManager->Exit();
}
