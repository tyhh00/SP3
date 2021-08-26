#pragma once

#include <vector>

#include "SceneBase.h"
#include "Camera.h"
#include "GameObjectManager.h"

enum ABILITY_TYPE
{
	ABILITY_DASH,
	ABILITY_PORTAL,
	ABILITY_GRAPPLER,
	ABILITY_RECALL,
	ABILITY_BLACKHOLE,
	ABILITY_COUNT
};

class Ability
{

public:
	Ability(char buttonChar, ABILITY_TYPE type, double abilityDurationCooldown);
	~Ability();

	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual ABILITY_TYPE GetAbilityType() = 0;

	void SetCamera(Camera* camera);
	void SetGOManager(GameObjectManager* GOManager);
	void SetScenePointer(SceneBase* scene);

	void SetAbilityCooldownDuration(double cooldown);
	void ResetAbilityCooldown();

	double GetCooldownLeft();
	double GetCooldownDuration();
	double GetCooldownTillReadyPercentage();

protected:

	double abilityCD_timeleft;
	double abilityCooldownDuration;

	Camera* camera;
	GameObjectManager* goManager;
	SceneBase* scene; //Used for custom rendering of abilities
	ABILITY_TYPE type;
	const char buttonChar;
};

