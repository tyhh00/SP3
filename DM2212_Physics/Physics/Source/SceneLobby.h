#ifndef SCENE_LOBBY_H
#define SCENE_LOBBY_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Item.h"
#include "Inventory.h"
#include "Player.h"
#include "Input.h"
#include "GameObjectManager.h"
#include "LobbyPortal.h"
#include "GameStateManagement/GameStateManager.h"
#include "SceneManager.h"


class SceneLobby : public SceneBase
{
public:
	SceneManager *sceneManager;

	SceneLobby();
	~SceneLobby();

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
	LobbyPortal* portal_graveyard, *portal_jungle, *portal_ocean, *portal_robot;

	double gridLength, gridHeight;
	void CursorToWorldPosition(double& theX, double& theY);
};

#endif