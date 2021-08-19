#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Keyboard.h"

class Enemy : public GameObject {
public:
	virtual void Init();
	virtual void Update(double dt);
	
private:
	SpriteAnimation* animatedSprites;

	//float speed;
};


#endif