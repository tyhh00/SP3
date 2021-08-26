#ifndef TUMBLEWEED_H
#define TUMBLEWEED_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Enemy.h"
#include "Inventory.h"


class Tumbleweed : public Enemy {
public:
	Tumbleweed();
	~Tumbleweed();

	void Init(); 
	void Update(double dt);
	void CollidedWith(GameObject* go);

private:
	enum ENEMY_STATE { // keep in case want add more
		ROLL,

		GSTATE_TOTAL
	};

	float roll_speed;
	double roll_timer;
	
	ENEMY_STATE state;
	
	SpriteAnimation* animatedSprites;

};


#endif