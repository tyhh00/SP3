#include "SceneTest.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"

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


	player = new Player;
	player->Init();
	player->active = true;
	player->pos.Set(m_worldWidth * 0.5, m_worldHeight * 0.5, 0);
	player->scale.Set(3, 3, 3);

//	m_goList.push_back(player);


	testobj = FetchGO(GameObject::GO_BALL);
	testobj->scale.Set(5, 5, 5);
	testobj->pos.Set(player->pos.x + 100, player->pos.y, player->pos.z);


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
		camera.SetFocusTarget(player->pos, true);

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

	player->Update(dt);
	camera.Update(player->pos, dt);

	// Game Objects
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{		
			// ...

		}
	}

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

	RenderMesh(meshList[GEO_AXES], false);

	// all gos
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(player->pos.x, player->pos.y, player->pos.z);
	RenderMesh(player->mesh, false);
	modelStack.PopMatrix();

	std::ostringstream ss;
	//ss.str("");
	//ss << "cam target: " << camera.target;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 6);
	//ss.str("");
	//ss << "cam pos: " << camera.position;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 9);
	//ss.str("");
	//ss << "player: " << player->pos;
	//RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 3, 0, 12);
	


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
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}

	if (player)
	{
		delete player;
		player = NULL;
	}
	
}
