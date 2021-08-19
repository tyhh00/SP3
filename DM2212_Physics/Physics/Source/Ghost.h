#ifndef GHOST_H
#define GHOST_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Keyboard.h"
#include "Enemy.h"

class Ghost : public Enemy {
public:
	void Init();
	void Update(Vector3 playerPos, double dt);
	void IsInSight(float f_radius);

private:
	enum GHOST_STATE {
		INACTIVE,
		FOLLOWING,
		HOSTILE,

		GSTATE_TOTAL
	};

	GHOST_STATE state;

	SpriteAnimation* animatedSprites;

};


#endif