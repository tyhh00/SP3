#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "SpriteAnimation.h"

class Player : public GameObject {
public:
	void Init();
	void Update(double dt);
	

private:
	SpriteAnimation* animatedSprites;

	float speed;
	bool AkeyDown, DkeyDown;
};


#endif