#ifndef SCENE_JUNGLE_H
#define SCENE_JUNGLE_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Item.h"
#include "Inventory.h"
#include "Apple.h"
#include "Cheese.h"
#include "Campfire.h"
#include "Monkey.h"
#include "Player.h"
#include "Prowler.h"
#include "LightBullet.h"
#include "Pistol.h"
#include "Input.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "Grappling.h"

class SceneJungle : public SceneBase
{
public:
	SceneJungle();
	~SceneJungle();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

protected:

	float m_speed;
	float dashTimer;
	bool isDashing = false;
	bool isGrappling = false;

	Player* player;
	Inventory* inventory;

	Input* input;
	GameObjectManager* goManager;
	GameManager* gameManager;

	double gridLength, gridHeight;
};

#endif