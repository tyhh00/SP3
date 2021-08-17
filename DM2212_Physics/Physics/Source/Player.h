#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"

class Player : public GameObject {
public:
	void Init();
	void Update(double dt);
	

private:
	SpriteAnimation* animatedSprites;

	float speed;
	float jump_force;

	bool AkeyDown, DkeyDown;
	bool spaceKeyDown;
};


#endif