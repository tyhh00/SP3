#ifndef SCENE_ROBOT_H
#define SCENE_ROBOT_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Item.h"
#include "Inventory.h"
#include "Player.h"
#include "Input.h"
#include "GameObjectManager.h"


#include "BulletSpawner.h"
#include "PlasmaBullet.h"

class SceneRobot : public SceneBase
{
public:
	SceneRobot();
	~SceneRobot();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	virtual void SetLights();

protected:

	float m_speed;

	float m_screenWidth;
	float m_screenHeight;
	float m_worldWidth;
	float m_worldHeight;

	Inventory* inventory;
	Input* input;
	GameObjectManager* goManager;
	Player* player;

	double gridLength, gridHeight;
	void CursorToWorldPosition(double& theX, double& theY);
};

#endif