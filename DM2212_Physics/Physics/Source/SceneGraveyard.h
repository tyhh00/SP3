#ifndef SCENE_GRAVEYARD_H
#define SCENE_GRAVEYARD_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Item.h"
#include "Inventory.h"
#include "Player.h"
#include "Input.h"
#include "GameObjectManager.h"
#include "SceneManager.h"

class SceneGraveyard : public SceneBase
{
public:
	SceneGraveyard();
	~SceneGraveyard();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	virtual void InitLights();

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