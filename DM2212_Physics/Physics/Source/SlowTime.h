#ifndef SLOWTIME_H
#define SLOWTIME_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Ability.h"
#include "GameObjectManager.h"

class SlowTimeAbility : public Ability {
	
public:
	SlowTimeAbility();
	~SlowTimeAbility();

	void Init();
	void Update(double dt);
	void Render();
	void UpdatePlayer(Physics* physics);
	ABILITY_TYPE GetAbilityType();

private:

	Input* input;

	Physics* playerPhysics;
	GameObjectManager* goManager;
	float maxVel;

	void CursorToWorldPosition(double& theX, double& theY);

};

#endif