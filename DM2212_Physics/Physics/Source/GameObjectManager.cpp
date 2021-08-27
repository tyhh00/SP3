#include "GameObjectManager.h"
#include "Debug.h"

#include <algorithm>


void GameObjectManager::Init()
{
	m_speed = 1.f;
}

bool GameObjectManager::CheckCollision(GameObject* go1, GameObject* go2, float dt, bool rangeCheck)
{
	if (go1 == nullptr || go2 == nullptr)
		return false;
	if (!go1->enableCollision || !go2->enableCollision)
		return false;
	// in case of self collision
	/*if (go1 == go2)
	{
		return false;
	}*/

	//Radius checking if explosive
	Vector3 go1_fScale, go2_fScale;
	if (rangeCheck)
	{
		go1_fScale = go1->scale * (go1->IsExplosive() ? go1->explosiveRadius : 1);
		go2_fScale = go2->scale * (go2->IsExplosive() ? go2->explosiveRadius : 1);
		//DEBUG_MSG("Collided Scale: " << go1_fScale << " org : " << go1->scale << " | go2 pos: " << go2->pos);
	}
	else
	{
		go1_fScale = go1->scale * go1->rangeCheckMulti;
		go2_fScale = go2->scale * go2->rangeCheckMulti;
	}
	if (go1_fScale.LengthSquared() <= Math::EPSILON || go2_fScale.LengthSquared() <= Math::EPSILON)
	{
		return false;
	}
	if (!EfficientRangeCheck(go1->pos, go2->pos, (go1_fScale.x + go2_fScale.y) * 1.5))
	{
		//DEBUG_MSG("RANGECHECKEDDENY");
	}

	if (go1->physics->shapeType == CIRCLE)
	{
		switch (go2->physics->shapeType)
		{
		case CIRCLE:
		{
			Vector3 dis = go2->pos - go1->pos;
			float disSquared = dis.LengthSquared();
			if (disSquared <= (go1_fScale.x + go2_fScale.x) * (go1_fScale.x + go2_fScale.x) && dis.Dot(go1->physics->GetVelocity() - go2->physics->GetVelocity()) > 0)
			{
				return true;
			}
		}
		break;
		case RECTANGLE:
			Vector3 dis = go2->pos - go1->pos;

			if (dis.LengthSquared() < go1_fScale.x * go1_fScale.x)
				return true;
			else
				dis = (dis.Length() - go1_fScale.x) * dis.Normalized();

			Vector3 N = go2->physics->GetNormal();
			if (dis.Dot(N) < 0)
			{
				N = -1 * N;
			}
			Vector3 NP(N.y, -1 * N.x, 0);

			if (dis.Dot(N) < go1_fScale.x + go2_fScale.x
				&& abs(dis.Dot(NP)) < go2_fScale.y
				//&& go1->physics->GetVelocity().Dot(N) > 0
				)
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

			if (dis.Dot(N) < go1_fScale.y + go2_fScale.y
				&& abs(dis.Dot(NP)) < go2_fScale.x
				//&& go1->physics->GetVelocity().Dot(N) > 0
				)
	
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
			if (dis.LengthSquared() < go2_fScale.x * go2_fScale.x)
				dis = Vector3(0, 0, 0);
			else
				dis = (dis.Length() - go2_fScale.x) * dis.Normalized();
			Vector3 N = go2->physics->GetNormal();
			if (dis.Dot(N) < 0)
			{
				N = -1 * N;
			}
			Vector3 NP(N.y, -1 * N.x, 0);

			if (dis.Dot(N) < go1_fScale.x + go2_fScale.x
				&& abs(dis.Dot(NP)) < go2_fScale.y
				//&& go1->physics->GetVelocity().Dot(N) > 0
				)
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

			if (dis.Dot(N) < go1_fScale.y + go2_fScale.y
				&& abs(dis.Dot(NP)) < go2_fScale.x
			//	&& go1->physics->GetVelocity().Dot(N) > 0 
				)
			{
				go2->physics->SetCollisionNormal(N);
				/*
				go1->physics->scale = Vector3(go1_fScale.y, go1_fScale.x, go1_fScale.z);
				go2->physics->scale = Vector3(go2_fScale.y, go2_fScale.x, go2_fScale.z);*/
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

			if (dis.Dot(N) < go1_fScale.x + go2_fScale.x
				&& abs(dis.Dot(NP)) < go2_fScale.y
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

			if (dis.Dot(N) < go1_fScale.y + go2_fScale.y
				&& abs(dis.Dot(NP)) < go2_fScale.x
				&& go1->physics->GetVelocity().Dot(N) > 0)
			{
				go2->physics->SetCollisionNormal(N);/*
				go1->physics->scale = Vector3(go1_fScale.y, go1_fScale.x, go1_fScale.z);
				go2->physics->scale = Vector3(go2_fScale.y, go2_fScale.x, go2_fScale.z);*/
				return true;
			}
			break;
		}
	}
	return false;
}

bool GameObjectManager::EfficientRangeCheck(Vector3 pos1, Vector3 pos2, float withinBoxRadius)
{
	if (pos1.x - withinBoxRadius >= pos2.x &&
		pos1.x + withinBoxRadius <= pos2.x
		&& pos1.y - withinBoxRadius >= pos2.y
		&& pos1.y + withinBoxRadius <= pos2.y)
		return true;
	return false;
}

void GameObjectManager::Update(double dt)
{
	int loops = 1;
	if (dt > 0.1)
		loops = dt / 0.08;

	double splitDt = dt / loops;
	for (int i = 0; i < loops; ++i)
	{
		// Add GOs that need to be added
		for (std::vector<GameObject*>::iterator it = toAddList.begin(); it != toAddList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go->physics->GetMovable())
			{
				m_movableGOList.push_back(go);
			}
			else
			{
				m_stationaryGOList.push_back(go);
			}
		}
		toAddList.clear();


		// Game Objects
		// Update Moveable GOs
		for (std::vector<GameObject*>::iterator it = m_movableGOList.begin(); it != m_movableGOList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;

			if (go == nullptr || !go->active)
				continue;

			if (go->dead)
			{
				toRemoveList.push_back(go);
				continue;
			}

			go->Update(splitDt);
			go->physics->Update(splitDt);
			if (go->physics->GetUpdateEnabled())
				go->pos += go->physics->GetVelocity() * m_speed * splitDt;
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

				if (go2 == nullptr || !go2->active)
					continue;

				if (go->dead)
					break;

				if (go->bottomSprite != nullptr)
				{
					if (CheckCollision(go->bottomSprite, go2, splitDt, false))
					{
						go->physics->SetOnGround(true);
					}
				}
				if (go->type == GameObject::GAMEOBJECT_TYPE::GO_BULLET
					|| go2->type == GameObject::GAMEOBJECT_TYPE::GO_BULLET)
				{
					int a = -5;
				}

				if (CheckCollision(go, go2, splitDt, false))
				{
					if (go->type == GameObject::GAMEOBJECT_TYPE::GO_BULLET
						|| go2->type == GameObject::GAMEOBJECT_TYPE::GO_BULLET)
					{
						DEBUG_MSG("SOME BULLET COLLISION");
					}
					if (go->IsExplosive())
					{
						for (auto& go3 : m_movableGOList)
						{
							if (go3 == nullptr || !go3->active)
								continue;
							if (go != go3 && go3 != go2 && EfficientRangeCheck(go->pos, go3->pos, go->explosiveRadius * 1.5) && CheckCollision(go, go3, splitDt, true))
							{
								go->CollidedWith(go3);
								go3->CollidedWith(go);
							}
						}
						for (auto& go3 : m_stationaryGOList)
						{
							if (go3 == nullptr)
								continue;
							if (go != go3 && go3 != go2 && EfficientRangeCheck(go->pos, go3->pos, go->explosiveRadius * 1.5) && CheckCollision(go, go3, splitDt, true))
							{
								go->CollidedWith(go3);
								go3->CollidedWith(go);
							}
						}
					}

					go->CollidedWith(go2);
					go2->CollidedWith(go);
					if (go->CanCollisionRespondTo(go2->type) &&
						go2->CanCollisionRespondTo(go->type))
					{
						go->physics->CollisionResponse(go2->physics, splitDt);
					}

					go->pos = go->physics->pos;
					go2->pos = go2->physics->pos;
					continue; //remove if stays at the end
				}
			}
			// Collision with moving and stationary
			for (std::vector<GameObject*>::iterator it2 = m_stationaryGOList.begin(); it2 != m_stationaryGOList.end(); ++it2)
			{
				GameObject* go2 = (GameObject*)*it2;

				if (go2 == nullptr)
					continue;

				if (go2->dead)
				{
					toRemoveList.push_back(go2);
					continue;
				}

				go2->Update(splitDt);
				// attachment for checking if onGround
				if (go->bottomSprite != nullptr)
				{
					if (CheckCollision(go->bottomSprite, go2, splitDt, false))
					{
						go->physics->SetOnGround(true);
					}
				}

				if (CheckCollision(go, go2, splitDt, false))
				{
					if (go->type == GameObject::GAMEOBJECT_TYPE::GO_BULLET)
					{
						int a = -5;
					}
					go2->physics->pos = go2->pos;
					go2->physics->scale = go2->scale;
					if (go->IsExplosive())
					{
						for (auto& go3 : m_movableGOList)
						{
							if (go3 == nullptr || !go3->active)
								continue;
							if (go != go3 && go3 != go2 && EfficientRangeCheck(go->pos, go3->pos, go->explosiveRadius * 1.5) && CheckCollision(go, go3, splitDt, true))
							{
								go->CollidedWith(go3);
								go3->CollidedWith(go);
							}
						}
						for (auto& go3 : m_stationaryGOList)
						{
							if (go3 == nullptr)
								continue;
							if (go != go3 && go3 != go2 && EfficientRangeCheck(go->pos, go3->pos, go->explosiveRadius * 1.5) && CheckCollision(go, go3, splitDt, true))
							{
								go->CollidedWith(go3);
								go3->CollidedWith(go);
							}
						}
					}
					go->CollidedWith(go2);
					go2->CollidedWith(go);
					if (go->CanCollisionRespondTo(go2->type) &&
						go2->CanCollisionRespondTo(go->type))
					{
						go->physics->CollisionResponse(go2->physics, splitDt);
					}
					go->pos = go->physics->pos;
					go2->pos = go2->physics->pos;
					continue; //remove if stays at the end
				}

			}

		}
		// Update Stationary GOs
		for (std::vector<GameObject*>::iterator it = m_stationaryGOList.begin(); it != m_stationaryGOList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go == nullptr || !go->active)
				continue;

			if (go->dead)
			{
				toRemoveList.push_back(go);
				continue;
			}

			go->Update(splitDt);
		}

		// Remove GOs that need to be deleted
		std::sort(toRemoveList.begin(), toRemoveList.end());
		toRemoveList.erase(std::unique(toRemoveList.begin(), toRemoveList.end()), toRemoveList.end());

		for (std::vector<GameObject*>::iterator it = toRemoveList.begin(); it != toRemoveList.end(); ++it)
		{
			GameObject* go = (GameObject*)*it;
			if (go == nullptr || go == NULL)
				continue;
			if (go->physics->GetMovable())
			{
				if (go->geoTypeID == 150)
				{
					DEBUG_MSG("Deleted robot?");
				}
				for (int i = 0; i < m_movableGOList.size(); i++)
				{
					if (m_movableGOList.at(i) == go)
					{
						DEBUG_MSG("Deleted: " << m_movableGOList.at(i));
						delete m_movableGOList.at(i);
						m_movableGOList.at(i) = nullptr;
						go = nullptr;
					}
				}
			}
			else
			{
				for (int i = 0; i < m_stationaryGOList.size(); i++)
				{
					if (m_stationaryGOList.at(i) == go)
					{
						delete m_stationaryGOList.at(i);
						m_stationaryGOList.at(i) = nullptr;
						go = nullptr;
					}
				}
			}
		}
		toRemoveList.clear();
		m_movableGOList.erase(std::remove(m_movableGOList.begin(), m_movableGOList.end(), nullptr),
			m_movableGOList.end());
		m_stationaryGOList.erase(std::remove(m_stationaryGOList.begin(), m_stationaryGOList.end(), nullptr),
			m_stationaryGOList.end());
	}

	
}

void GameObjectManager::Render(SceneBase* scene)
{
	// all gos
	for (std::vector<GameObject*>::iterator it = m_movableGOList.begin(); it != m_movableGOList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if (go != nullptr && go->active)
		{
			go->Render(scene);
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

		if (go == nullptr || !go->active)
			continue;

		go->Render(scene);
	}
}
void GameObjectManager::AddGO(GameObject* go)
{
	toAddList.push_back(go);
}
void GameObjectManager::RemoveGO(GameObject* go)
{
	toRemoveList.push_back(go);
}
void GameObjectManager::AddAllGO(std::vector<GameObject*> gos)
{
	for (auto& go : gos)
		AddGO(go);
}
int GameObjectManager::GetNumGOsbyType(GameObject::GAMEOBJECT_TYPE type)
{
	int num = 0;
	for (auto go : m_movableGOList)
	{
		if (go != nullptr)
		{
			if (go->type == type)
			{
				num++;
			}
		}
	}
	for (auto go : m_stationaryGOList)
	{
		if (go != nullptr)
		{
			if (go->type == type)
			{
				num++;
			}
		}
	}
	return num;
}
void GameObjectManager::DeleteAllGOs()
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
	toAddList.clear();
	toRemoveList.clear();
}
void GameObjectManager::Exit()
{
	DeleteAllGOs();
}

std::vector<GameObject*> GameObjectManager::GetStationaryList()
{
	return this->m_stationaryGOList;
}
