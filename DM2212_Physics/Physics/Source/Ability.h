#pragma once

#include <vector>

#include "SceneBase.h"
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
	Ability(char buttonChar, ABILITY_TYPE type);
	~Ability();

	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual ABILITY_TYPE GetAbilityType() = 0;

	void SetCamera(Camera* camera);
	void SetGOManager(GameObjectManager* GOManager);
	void SetScenePointer(SceneBase* scene);

protected:


	Camera* camera;
	GameObjectManager* goManager;
	SceneBase* scene; //Used for custom rendering of abilities
	ABILITY_TYPE type;

private:
	char buttonChar;
};

