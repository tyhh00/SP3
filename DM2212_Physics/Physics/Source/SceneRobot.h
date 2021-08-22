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

	virtual void InitLights();

protected:

	float m_speed;

	Inventory* inventory;
	Input* input;
	GameObjectManager* goManager;
	Player* player;
	BulletSpawner* spawner;

	double gridLength, gridHeight;
	void CursorToWorldPosition(double& theX, double& theY);
};

#endif