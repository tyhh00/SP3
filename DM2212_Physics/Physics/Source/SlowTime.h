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

	void Update(double dt);
	void Render();
	ABILITY_TYPE GetAbilityType();

	void setGOM(GameObjectManager* goM);

private:

	Input* input;
	void CursorToWorldPosition(double& theX, double& theY);
	double abilityTimer;

};

#endif