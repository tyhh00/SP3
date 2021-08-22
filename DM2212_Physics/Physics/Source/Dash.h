#ifndef DASH_H
#define DASH_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Ability.h"

class DashAbility : public Ability {
	
public:
	DashAbility();
	~DashAbility();

	void Update(double dt);
	void Render();
	void UpdatePlayer(float& accel, Physics* physics, float& speed, bool& enableCollision, bool& isDashing);
	ABILITY_TYPE GetAbilityType();

private:

	float dashTimer;
	bool isDashing;

	Input* input;

	Physics* playerPhysics;
	float speed;
	bool enableCollision;
	float accel;

	void CursorToWorldPosition(double& theX, double& theY);

};

#endif