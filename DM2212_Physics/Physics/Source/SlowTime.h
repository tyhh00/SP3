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
	SlowTimeAbility(Mesh* mesh);
	~SlowTimeAbility();

	void Init();
	void Update(double dt);
	void Render();
	void Reset();
	ABILITY_TYPE GetAbilityType();

private:

	Input* input;
	void CursorToWorldPosition(double& theX, double& theY);
	void SetLighting();
	double abilityTimer;

};

#endif