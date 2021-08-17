#include "GameObjectManager.h"

void GameObjectManager::Init()
{
	m_speed = 1.f;
}


bool GameObjectManager::CheckCollision(GameObject* go1, GameObject* go2, float dt)
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
	return false;
}

void GameObjectManager::Update(double dt)
{
	// Game Objects
	for (std::vector<GameObject*>::iterator it = m_movableGOList.begin(); it != m_movableGOList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active)
		{
			go->physics->Update(dt);
			go->pos += go->physics->GetVelocity() * m_speed * dt;
			go->physics->pos = go->pos;
			go->physics->scale = go->scale;

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

void GameObjectManager::Render(SceneBase* scene)
{
	// all gos
	for (std::vector<GameObject*>::iterator it = m_movableGOList.begin(); it != m_movableGOList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active)
		{
			float angle = Math::RadianToDegree(atan2(go->physics->GetNormal().y, go->physics->GetNormal().x));
			scene->modelStack.PushMatrix();
			scene->modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			scene->modelStack.Rotate(angle + go->physics->GetRotateZ(), 0, 0, 1);
			scene->modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			scene->RenderMesh(go->mesh, false);
			scene->modelStack.PopMatrix();
		}
	}
	for (std::vector<GameObject*>::iterator it = m_stationaryGOList.begin(); it != m_stationaryGOList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active)
		{
			float angle = Math::RadianToDegree(atan2(go->physics->GetNormal().y, go->physics->GetNormal().x));
			scene->modelStack.PushMatrix();
			scene->modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			scene->modelStack.Rotate(angle + go->physics->GetRotateZ(), 0, 0, 1);
			scene->modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			scene->RenderMesh(go->mesh, false);
			scene->modelStack.PopMatrix();
		}
	}
}
void GameObjectManager::AddGO(GameObject* go)
{
	if (go->physics->GetMovable())
	{
		m_movableGOList.push_back(go);
	}
	else
	{
		m_stationaryGOList.push_back(go);
	}
}
//
//void GameObjectManager::RenderGO(GameObject* go)
//{
//	switch (go->type)
//	{
//	case GameObject::GO_BALL:
//		meshList[GEO_BALL]->material.kAmbient = go->mat.kAmbient;
//		modelStack.PushMatrix();
//		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
//		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
//		modelStack.Rotate(90, 1, 0, 0);
//		RenderMesh(meshList[GEO_BALL], true);
//		modelStack.PopMatrix();
//		break;
//	case GameObject::GO_WALL:
//	case GameObject::GO_SPRING:
//	case GameObject::GO_ONESIDEDWALL:
//	case GameObject::GO_MWALL:
//	case GameObject::GO_BWALL:
//	{
//		float angle = Math::RadianToDegree(atan2(go->physics->GetNormal().y, go->physics->GetNormal().x));
//		meshList[GEO_WALL]->material.kAmbient = go->mat.kAmbient;
//		modelStack.PushMatrix();
//		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
//		modelStack.Rotate(angle, 0, 0, 1);
//		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
//		RenderMesh(meshList[GEO_WALL], true);
//		modelStack.PopMatrix();
//	}
//	break;
//	case GameObject::GO_PILLAR:
//	case GameObject::GO_BPILLAR:
//	case GameObject::GO_EPILLAR:
//	case GameObject::GO_MPILLAR:
//		meshList[GEO_BALL]->material.kAmbient = go->mat.kAmbient;
//		modelStack.PushMatrix();
//		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
//		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
//		modelStack.Rotate(90, 1, 0, 0);
//		RenderMesh(meshList[GEO_BALL], true);
//		modelStack.PopMatrix();
//		break;
//	default:
//		break;
//	}
//}

void GameObjectManager::Exit()
{
	while (m_movableGOList.size() > 0)
	{
		GameObject* go = m_movableGOList.back();
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