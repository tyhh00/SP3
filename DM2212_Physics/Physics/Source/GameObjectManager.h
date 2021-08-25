#pragma once
#include <vector>
#include "GameObject.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"
#include "SceneBase.h"


class GameObjectManager
{

protected:
	std::vector<GameObject*> m_movableGOList; //movable game objects
	std::vector<GameObject*> m_stationaryGOList; //stationary game objects

	std::vector<GameObject*> toAddList;
	std::vector<GameObject*> toRemoveList;

	float m_speed;

public:
	void Init();
	void Render(SceneBase* scene);
	void AddGO(GameObject* go);
	void RemoveGO(GameObject* go);
	void AddAllGO(std::vector<GameObject*> gos);
	void DeleteAllGOs();
	bool CheckCollision(GameObject* go1, GameObject* go2, float dt, bool rangeCheck);
	void Exit();
	void Update(double dt);
};

