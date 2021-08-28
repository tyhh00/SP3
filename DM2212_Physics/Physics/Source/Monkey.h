#ifndef MONKEY_H
#define MONKEY_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Enemy.h"
#include "Inventory.h"
#include "Weapon.h"


class Monkey : public Enemy {
public:
	Monkey();
	~Monkey();

	void Init(SceneBase* scene, GameObjectManager* _goManager, Inventory* inventory, Vector3& target, BulletSpawner* _bulletSpawner);
	void Update(double dt);
	void StartAttackCooldown();
	bool isBeingAttacked();

private:
	enum MONKEY_STATE {
		IDLE , // default state, facing back and does not move
		JUMP,
		ATTACK, // haunting state, moves towards player (speed slower than player)

		MSTATE_TOTAL
	};

	float movement_speed;
	float activeRange;
	float shootTimer;

	MONKEY_STATE state;
	double state_timer;
	double state_interval;

	Vector3 *playerPos;
	
	SpriteAnimation* animatedSprites;
	Inventory* inventory;
	BulletSpawner* bulletSpawner;
	GameObjectManager* goManager;
};


#endif