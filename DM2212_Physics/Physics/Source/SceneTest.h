#ifndef SCENE_TEST_H
#define SCENE_TEST_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Keyboard.h"

class SceneTest : public SceneBase
{
public:
	SceneTest();
	~SceneTest();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	virtual void SetLights();

	void RenderGO(GameObject *go);

protected:


	//Physics
	std::vector<GameObject *> m_goList;
	
	Player* player;
	GameObject* testobj, *testWall;

	GameObjectManager* goManager;

	float m_speed;
	float m_screenWidth;
	float m_screenHeight;
	float m_worldWidth;
	float m_worldHeight;
	int m_objectCount;
	
	GameObject* FetchGO();
	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);
	GameObject* FetchGO(bool isMovable);
	void ReturnGO(GameObject *go);

	bool CheckZero(double number); // Checks if a number is 0 [EPSILON things]
	void CursorToWorldPosition(double& theX, double& theY);

};

#endif