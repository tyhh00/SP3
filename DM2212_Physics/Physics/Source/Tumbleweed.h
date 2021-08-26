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

	void Init(SceneBase* scene, Inventory* inventory, Vector3& target); // keep target only if you need to access player pos
	void Update(double dt);
	void CollidedWith(GameObject* go);

private:
	enum ENEMY_STATE {
		ROLL,

		GSTATE_TOTAL
	};

	float roll_speed;
	double roll_timer;
	
	ENEMY_STATE state;

	Vector3 *playerPos; // to keep track of player pos
	
	SpriteAnimation* animatedSprites;
	Inventory* inventory;

};


#endif