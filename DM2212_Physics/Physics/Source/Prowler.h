#ifndef PROWLER_H
#define PROWLER_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Enemy.h"
#include "Inventory.h"
#include "Monkey.h"
#include "Pistol.h"
#include "Weapon.h"


class Prowler : public Enemy {
public:
	Prowler();
	~Prowler();

	void Init(SceneBase* scene, Inventory* inventory, Vector3& target, GameObjectManager* goManager);
	void Update(double dt);
	bool isBeingAttacked();

private:
	enum PROWLER_STATE {
		IDLE, // default state, facing back and does not move
		CHASE,
		STUNNED,
		DEFEND,
		ATTACK, // haunting state, moves towards player (speed slower than player)
		SPAWN,

		MSTATE_TOTAL
	};

	float movement_speed;
	float activeRange;
	float attackRange;
	float defendRange;
	float shootTimer;

	PROWLER_STATE state;
	float attackTimer;
	float stunnedTimer;
	float spawnCooldownTimer;

	bool readyToSpawnMonkey;

	Vector3 *playerPos;
	
	SpriteAnimation* animatedSprites;
	Inventory* inventory;
	GameObjectManager* goManager;
};


#endif