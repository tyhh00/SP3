#pragma once
#include <vector>
#include "GameObject.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"
#include "SceneBase.h"
#include "Camera.h"

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
	void Update(double dt, Camera* cam);
	void Render(SceneBase* scene);
	void AddGO(GameObject* go, bool instant = false);
	void RemoveGO(GameObject* go);
	void AddAllGO(std::vector<GameObject*> gos);
	int GetNumGOsbyType(GameObject::GAMEOBJECT_TYPE type);
	void DeleteAllGOs();
	bool CheckCollision(GameObject* go1, GameObject* go2, float dt, bool rangeCheck);
	bool EfficientRangeCheck(Vector3 pos1, Vector3 pos2, float withinBoxRadius);

	void SetmSpeed(float mspeed);
	
	void Exit();

	std::vector<GameObject*> GetStationaryList();
};

