#ifndef GRAPPLING_H
#define GRAPPLING_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Ability.h"

class GrapplingHook : public GameObject {
	
public:
	GrapplingHook()
	{
		active = false;
		mesh = MeshBuilder::GenerateQuad("Wall", Color(1, 1, 1), 2.0f);
		enableCollision = false;
	}
	~GrapplingHook()
	{
		delete mesh;
		mesh = nullptr;
	}
};

class GrapplingAbility : public Ability {
	
public:
	GrapplingAbility();
	~GrapplingAbility();

	void Update(double dt);
	void UpdatePlayer(Vector3& pos, Physics* playerPhysics, float& _maxVel);
	void Render();
	ABILITY_TYPE GetAbilityType();

private:
	Input* input;

	Vector3 temp;
	Vector3 initialDisplacement;
	Vector3 playerPos;
	Physics* playerPhysics;

	bool isGrappling;
	float maxVel;

	GrapplingHook grapplingHook;

	void CursorToWorldPosition(double& theX, double& theY);
};

#endif