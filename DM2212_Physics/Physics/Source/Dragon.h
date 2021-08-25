#ifndef DRAGON_H
#define DRAGON_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Enemy.h"
#include "Inventory.h"


class Dragon : public Enemy {
public:
	Dragon();
	~Dragon();

	void Init(SceneBase* scene, Inventory* inventory, Vector3& target); // keep target only if you need to access player pos
	void Update(double dt);

private:
	enum ENEMY_STATE {
		INACTIVE, // default state, not moving
		ROLL,

		GSTATE_TOTAL
	};

	float roll_speed;

	float activeRange;
	
	ENEMY_STATE state;

	Vector3 *playerPos; // to keep track of player pos
	
	SpriteAnimation* animatedSprites;
	Inventory* inventory;

};


#endif