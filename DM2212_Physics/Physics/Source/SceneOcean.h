#ifndef SCENE_OCEAN_H
#define SCENE_OCEAN_H

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
#include "Input.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "Trident.h"
#include "Crab.h"
#include "Dragon.h"
#include "ThunderBlock.h"

class SceneOcean : public SceneBase
{
public:
	SceneOcean();
	~SceneOcean();

	virtual void Init();
	virtual void Update(double dt);
	virtual void InitLights();
	virtual void Render();
	virtual void Exit();

protected:

	float m_speed;

	Player* player;
	Inventory* inventory;

	Input* input;
	GameObjectManager* goManager;
	GameManager* gameManager;

	double gridLength, gridHeight;
};

#endif