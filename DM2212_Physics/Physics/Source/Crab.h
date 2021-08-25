#ifndef CRAB_H
#define CRAB_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Enemy.h"
#include "Inventory.h"


class Crab : public Enemy {
public:
	enum MOVEMENT_TYPE {
		LAR,
		EDGE,
		JUMP,
		MTYPE_TOTAL
	};

	Crab();
	~Crab();

	void Init(Vector3& target, MOVEMENT_TYPE type); // keep target only if you need to access player pos
	void Update(double dt);

private:
	enum ENEMY_STATE {
		IDLE,
		WLAR,
		ATTACK,
		DIE,
		GSTATE_TOTAL
	};


	float activeRange;
	double crabTimer;
	double WLARTimer; //walk left and right timer
	ENEMY_STATE state;
	float tempVel;
	MOVEMENT_TYPE mType;
	Vector3 *playerPos; // to keep track of player pos
	
	SpriteAnimation* animatedSprites;

	void updateMType(double dt);
	void CollidedWith(GameObject* go);
};


#endif