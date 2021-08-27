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
	DashAbility(Mesh* mesh = nullptr);
	~DashAbility();

	void Init();
	void Update(double dt);
	void Render();
//	void UpdatePlayer(int& dashDir, Physics* physics, float& maxVel, bool& enableCollision);
	ABILITY_TYPE GetAbilityType();

private:

	float dashTimer;
	bool isDashing;

	Input* input;

	//Physics* playerPhysics;
	//bool enableCollision;
	float maxVel;
	int dashDir;

	void CursorToWorldPosition(double& theX, double& theY);

};

#endif