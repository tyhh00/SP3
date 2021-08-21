#ifndef GHOST_H
#define GHOST_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Enemy.h"
#include "Inventory.h"


class Ghost : public Enemy {
public:
	Ghost();
	~Ghost();

	void Init(SceneBase* scene, Inventory* inventory, Vector3& target);
	void Update(double dt);

private:
	enum GHOST_STATE {
		INACTIVE, // default state, facing back and does not move
		FOLLOWING, // haunting state, moves towards player (speed slower than player)
		HIDING, // haunting state but facing back to hide from light, unable to move when in light
		WANDER, // wandering state, where it was haunting the player but lost sight of player while hiding (moves randomly for period of time (10s?))
		HOSTILE, // attack state, moves towards player (very quickly, unavoidable)

		GSTATE_TOTAL
	};

	float haunting_speed;
	float hostile_speed;

	float activeRange;
	float inactiveRange;
	float hostileRange;

	GHOST_STATE state;
	double state_timer;
	double state_interval;

	Vector3 *playerPos;
	
	SpriteAnimation* animatedSprites;
	Inventory* inventory;

	bool isWithinFlashlight();

};


#endif