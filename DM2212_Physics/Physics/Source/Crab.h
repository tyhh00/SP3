#ifndef CRAB_H
#define CRAB_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Enemy.h"

class Crab : public Enemy {
public:
	enum MOVEMENT_TYPE
	{
		LAR,
		CHASE,
		MTYPE_TOTAL
	};

	Crab();
	~Crab();

	void Init(GameObject* target, MOVEMENT_TYPE type); // keep target only if you need to access player pos
	void Update(double dt);

private:
	enum ENEMY_STATE {
		IDLE,
		WLAR,
		ATTACK,
		DIE,
		GSTATE_TOTAL
	};
	GameObject* player; // to keep track of player pos
	MOVEMENT_TYPE mType; //movement types of crab
	ENEMY_STATE state; //state of enemy
	SpriteAnimation* animatedSprites;

	float activeRange; //if player is within range, walk left and right
	float hostileRange; //if player is within range, chase player
	float attackRange; //if player within range, attack player

	double crabTimer; //timer that switches between IDLE and WLAR state
	double WLARTimer; //walk left and right timer
	double deathTimer; //time it takes for animation + deletion
	float tempVel; //temporary velocity of crab

	void updateMType(double dt); //update the mType
	void CollidedWith(GameObject* go); //check if collided with block type
};


#endif