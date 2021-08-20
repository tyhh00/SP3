#pragma once

#include <vector>

#include "SceneBase.h"
#include "Player.h"
#include "Camera.h"
#include "GameObjectManager.h"

enum ABILITY_TYPE
{
	ABILITY_DASH,
	ABILITY_PORTAL,
	ABILITY_COUNT
};

class Ability
{

public:
	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual ABILITY_TYPE GetAbilityType() = 0;

	void SetPlayer(Player* player);
	void SetCamera(Camera* camera);
	void SetGOManager(GameObjectManager* GOManager);
	void SetScenePointer(SceneBase* scene);

protected:
	Ability(char buttonChar, ABILITY_TYPE type);
	~Ability();


	Camera* camera;
	Player* player;
	GameObjectManager* goManager;
	SceneBase* scene; //Used for custom rendering of abilities

private:
	ABILITY_TYPE type;
	char buttonChar;
};

