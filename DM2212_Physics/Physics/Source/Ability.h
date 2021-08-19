#pragma once

#include "SceneBase.h"
#include "Player.h"
#include "Camera.h"

enum ABILITY_TYPE
{
	ABILITY_DASH,
	ABILITY_COUNT
};

class Ability
{

public:

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render(SceneBase* scene) = 0;
	virtual void GetAbilityType();
protected:
	Ability(char buttonLetter, ABILITY_TYPE type);
	Camera* cam;
	Player* player;
private:
	ABILITY_TYPE type;
};

