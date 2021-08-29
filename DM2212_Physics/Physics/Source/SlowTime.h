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
	SlowTimeAbility(Mesh* mesh); //constructor
	~SlowTimeAbility(); //destructor

	void Init(); //init
	void Update(double dt); //update
	void Render(); //render
	void Reset(); //reset
	ABILITY_TYPE GetAbilityType(); //ability type

private:
	//INPUT
	Input* input;
	//VARIABLE
	double abilityTimer;

	void CursorToWorldPosition(double& theX, double& theY); //mouse to world position
	void SetLighting(); //set lightning render to lightup when slowtime
};

#endif