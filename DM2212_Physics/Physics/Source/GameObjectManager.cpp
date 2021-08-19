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
				go2->physics->SetCollisionNormal(N);/*
				go1->physics->scale = Vector3(go1->scale.y, go1->scale.x, go1->scale.z);
				go2->physics->scale = Vector3(go2->scale.y, go2->scale.x, go2->scale.z);*/
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
				go2->physics->SetCollisionNormal(N);/*
				go1->physics->scale = Vector3(go1->scale.y, go1->scale.x, go1->scale.z);
				go2->physics->scale = Vector3(go2->scale.y, go2->scale.x, go2->scale.z);*/
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
			if (go->geoTypeID == SceneBase::GEOMETRY_TYPE::GEO_PLAYER_GIRL1)
			{
				std::cout << go->pos.y << std::endl;
			}
			go->Update(dt);
			go->physics->Update(dt);
			go->pos += go->physics->GetVelocity() * m_speed * dt;
			go->physics->pos = go->pos;
			go->physics->scale = go->scale;
			if (go->bottomSprite != nullptr)
			{
				go->physics->SetOnGround(false);
				go->bottomSprite->pos = go->pos + go->bottomSprite->relativePos;
				go->bottomSprite->physics->pos = go->bottomSprite->pos;
				go->bottomSprite->physics->scale = go->bottomSprite->scale;
				go->bottomSprite->physics->SetVelocity(go->physics->GetVelocity());
			}
			// Collision with moving and moving
			for (std::vector<GameObject*>::iterator it2 = it + 1; it2 != m_movableGOList.end(); ++it2)
			{
				GameObject* go2 = (GameObject*)*it2;

				if (go2->active)
				{
					if (CheckCollision(go, go2, dt))
					{
						go->CollidedWith(go2);
						go->physics->CollisionResponse(go2->physics, dt);
						go->pos = go->physics->pos;
						go2->pos = go2->physics->pos;
						continue; //remove if stays at the end
					}

				}
			}
			// Collision with moving and stationary
			for (std::vector<GameObject*>::iterator it2 = m_stationaryGOList.begin(); it2 != m_stationaryGOList.end(); ++it2)
			{
				GameObject* go2 = (GameObject*)*it2;

				// attachment for checking if onGround
				if (go->bottomSprite != nullptr)
				{
					if (CheckCollision(go->bottomSprite, go2, dt))
					{
						go->physics->SetOnGround(true);
						// continue;
					}
				}

				if (CheckCollision(go, go2, dt))
				{
					go->CollidedWith(go2);
					go2->physics->pos = go2->pos;
					go2->physics->scale = go2->scale;
					go->physics->CollisionResponse(go2->physics, dt);
					go->pos = go->physics->pos;
					go2->pos = go2->physics->pos;
					continue; //remove if stays at the end
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
			scene->RenderMesh(go->mesh, true);
			scene->modelStack.PopMatrix();
			// test things; to see bottomSprite
			/*if (go->bottomSprite)
			{
				GameObject* sprite = go->bottomSprite;
				float angle = Math::RadianToDegree(atan2(sprite->physics->GetNormal().y, sprite->physics->GetNormal().x));
				scene->modelStack.PushMatrix();
				scene->modelStack.Translate(sprite->pos.x, sprite->pos.y, sprite->pos.z);
				scene->modelStack.Rotate(angle + sprite->physics->GetRotateZ(), 0, 0, 1);
				scene->modelStack.Scale(sprite->scale.x, sprite->scale.y, sprite->scale.z);
				scene->RenderMesh(sprite->mesh, false);
				scene->modelStack.PopMatrix();
			}*/
		}
	}
	for (std::vector<GameObject*>::iterator it = m_stationaryGOList.begin(); it != m_stationaryGOList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;

		float angle = Math::RadianToDegree(atan2(go->physics->GetNormal().y, go->physics->GetNormal().x));
		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		scene->modelStack.Rotate(angle + go->physics->GetRotateZ(), 0, 0, 1);
		scene->modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		scene->RenderMesh(go->mesh, true);
		scene->modelStack.PopMatrix();
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

void GameObjectManager::AddAllGO(std::vector<GameObject*> gos)
{
	for (auto& go : gos)
		AddGO(go);
}

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