#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Keyboard.h"

class Player : public GameObject {
public:
	void Init();
	void Update(double dt);
	
private:
	SpriteAnimation* animatedSprites;

	float dashTimer;
	bool isDashing = false;

	float speed;
	float jump_force;

	bool AkeyDown, DkeyDown;
	bool spaceKeyDown;

	Keyboard* keyboard;
};


#endif