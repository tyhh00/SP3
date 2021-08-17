#include "ScenePhysics.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"

#include <sstream>

ScenePhysics::ScenePhysics()
{
}

ScenePhysics::~ScenePhysics()
{
}

void ScenePhysics::Init()
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
	go = FetchGO(true);
	go->type = GameObject::GO_WALL;
	go->physics->shapeType = RECTANGLE;
	go->physics->SetMass(3);
	go->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.5f, 0);
	go->scale.Set(2.5, 2.5, 2.5);
	go->mat.kAmbient.Set(1.0f, 1.0f, 1.0f);

	GameObject* go2 = FetchGO(false);
	go2->type = GameObject::GO_WALL;
	go2->physics->SetNormal(Vector3(cos(Math::DegreeToRadian(45)), sin(Math::DegreeToRadian(45)), 0));
	go2->physics->shapeType = RECTANGLE;
	go2->pos.Set(m_worldWidth * 0.25f, m_worldHeight * 0.15f, 0);
	go2->scale.Set(10, 10, 1);
	go2->mat.kAmbient.Set(1.0f, 1.0f, 1.0f);

	go2 = FetchGO(true);
	go2->type = GameObject::GO_WALL;
	go2->physics->SetVelocity(Vector3(1, 0, 0));
	go2->physics->shapeType = RECTANGLE;
	go2->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.35f, 0);
	go2->scale.Set(5, 5, 1);
	go2->mat.kAmbient.Set(1.0f, 1.0f, 1.0f);

	go2 = FetchGO(false);
	go2->type = GameObject::GO_WALL;
	go2->physics->shapeType = RECTANGLE;
	go2->pos.Set(m_worldWidth * 0.5f, m_worldHeight * 0.15f, 0);
	go2->scale.Set(2.5, 70, 2.5);
	go2->mat.kAmbient.Set(1.0f, 1.0f, 1.0f);
}

GameObject* ScenePhysics::FetchGO(bool isMovable)
{
	if (isMovable)
	{
		// Fetch a game object from m_goList and return it
		for (std::vector<GameObject*>::iterator it = m_movableGOList.begin(); it != m_movableGOList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (!go->active)
			{
				go->active = true;
				go->physics->SetMovable(true);
				return go;
			}
		}
		for (int i = 0; i < 10; i++)
		{
			GameObject* temp = new GameObject;
			m_movableGOList.push_back(temp);
		}
	}
	else
	{
		// Fetch a game object from m_goList and return it
		for (std::vector<GameObject*>::iterator it = m_stationaryGOList.begin(); it != m_stationaryGOList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (!go->active)
			{
				go->active = true;
				go->physics->SetMovable(false);
				return go;
			}
		}
		for (int i = 0; i < 10; i++)
		{
			GameObject* temp = new GameObject;
			m_stationaryGOList.push_back(temp);
		}
	}
	return FetchGO(isMovable);
}

void ScenePhysics::Update(double dt)
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

	// Game Objects
	for(std::vector<GameObject *>::iterator it = m_movableGOList.begin(); it != m_movableGOList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			go->physics->Update(dt);
			go->pos += go->physics->GetVelocity() * m_speed * dt;
			go->physics->pos = go->pos;
			go->physics->scale = go->scale;

			// In case leave screen Unspawn
			if (go->pos.x >= m_worldWidth + go->scale.x
				|| go->pos.x + go->scale.x <= 0
				|| go->pos.y >= m_worldHeight + go->scale.y
				|| go->pos.y + go->scale.y <= 0)
			{
				go->active = false;
			}

			// Collision with moving and moving
			for (std::vector<GameObject*>::iterator it2 = it + 1; it2 != m_movableGOList.end(); ++it2)
			{
				GameObject* go2 = (GameObject*)*it2;

				if (go2->active)
				{
					if (CheckCollision(go, go2, dt))
					{
						go->physics->CollisionResponse(go->physics, go2->physics, dt);
						continue; //remove if stays at the end
					}

					
				}
			}
			
			// Collision with moving and stationary
			for (std::vector<GameObject*>::iterator it2 = m_stationaryGOList.begin(); it2 != m_stationaryGOList.end(); ++it2)
			{
				GameObject* go2 = (GameObject*)*it2;

				if (go2->active)
				{
					go2->physics->pos = go2->pos;
					go2->physics->scale = go2->scale;
					if (CheckCollision(go, go2, dt))
					{
						go->physics->CollisionResponse(go->physics, go2->physics, dt);
						go->pos = go->physics->pos;
						go2->pos = go2->physics->pos;
						continue; //remove if stays at the end
					}

					
				}
			}
		}
	}
}

bool ScenePhysics::CheckCollision(GameObject* go1, GameObject* go2, float dt)
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

void ScenePhysics::CollisionResponse(GameObject* go1, GameObject* go2, float dt)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		int m1 = go1->physics->GetMass();
		int m2 = go2->physics->GetMass();
		Vector3 u1 = go1->physics->GetVelocity();
		Vector3 u2 = go2->physics->GetVelocity();

		Vector3 N = (go1->pos - go2->pos).Normalized();
		go1->physics->GetVelocity() = u1 + (2 * m2 / (m1 + m2)) * ((u2 - u1).Dot(N)) * N;
		go2->physics->GetVelocity() = u2 + (2 * m1 / (m1 + m2)) * ((u1 - u2).Dot(N)) * N;
		ApplyFriction(go1, N, dt);
		ApplyFriction(go2, N, dt);

	}
	break;
	case GameObject::GO_MWALL:
	{
		ApplyFriction(go1, go2->physics->GetNormal(), dt);
		Vector3 Rvel = go1->physics->GetVelocity() - go2->physics->GetVelocity();
		go1->physics->SetVelocity(go1->physics->GetVelocity() - 2 * Rvel.Dot(go2->physics->GetNormal()) * (go2->physics->GetNormal()));
		ApplyInelastic(go1, go2->physics->GetNormal(), dt);
	}
	break;
	case GameObject::GO_PILLAR:
	{
		Vector3 go2pos(go2->pos.x, go2->pos.y, 0);
		Vector3 N = (go2pos - go1->pos).Normalized();
		ApplyFriction(go1, N, dt);
		go1->physics->SetVelocity(go1->physics->GetVelocity() - 2 * go1->physics->GetVelocity().Dot(N) * N);
		ApplyInelastic(go1, N, dt);

	}
	break;
	case GameObject::GO_MPILLAR:
	{
		Vector3 go2pos(go2->pos.x, go2->pos.y, 0);
		Vector3 N = (go2pos - go1->pos).Normalized();
		ApplyFriction(go1, N, dt);
		go1->physics->SetVelocity(go1->physics->GetVelocity() - 2 * (go1->physics->GetVelocity() - go2->physics->GetVelocity()).Dot(N) * N);
		ApplyInelastic(go1, N, dt);
	}
	break;
	default:
		break;
	}
}

void ScenePhysics::ApplyFriction(GameObject* ball, Vector3 normal, double dt)
{
	// - velocity parallel to surface by amount of Nforce
	const float FRICTION_K = 0.0005f;
	float Nforce = abs(Vector3(ball->physics->GetMass() * ball->physics->GetGravity()).Dot(normal));
	Vector3 plane = normal.Cross(Vector3(0, 0, 1));
	if (ball->physics->GetVelocity().Dot(plane) < 0)
	{
		plane *= -1;
	}
	ball->physics->SetVelocity(ball->physics->GetVelocity() - FRICTION_K * Nforce * plane * m_speed * dt);
}

void ScenePhysics::ApplyInelastic(GameObject* ball, Vector3 normal, double dt)
{
	// - 50% of velocity in the direction of the normal
	Vector3 iN = normal;
	if (ball->physics->GetVelocity().Dot(iN) < 0)
	{
		iN = -1 * iN;
	}
	Vector3 proj = ball->physics->GetVelocity().Dot(iN) * iN;
	ball->physics->SetVelocity(ball->physics->GetVelocity() - 0.5f * proj);
}

void ScenePhysics::RenderGO(GameObject *go)
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
		float angle = Math::RadianToDegree(atan2(go->physics->GetNormal().y, go->physics->GetNormal().x));
		meshList[GEO_WALL]->material.kAmbient = go->mat.kAmbient;
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Rotate(angle, 0, 0, 1);
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
	default:
		break;
	}
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

	// all gos
	for (std::vector<GameObject*>::iterator it = m_movableGOList.begin(); it != m_movableGOList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}
	for (std::vector<GameObject*>::iterator it = m_stationaryGOList.begin(); it != m_stationaryGOList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}
}

void ScenePhysics::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_movableGOList.size() > 0)
	{
		GameObject *go = m_movableGOList.back();
		delete go;
		m_movableGOList.pop_back();
	}
	while (m_stationaryGOList.size() > 0)
	{
		GameObject* go = m_stationaryGOList.back();
		delete go;
		m_stationaryGOList.pop_back();
	}
}
