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
#include "Player.h"
#include "Weapon.h"


class Prowler : public Enemy {
public:
	Prowler();
	~Prowler();

	void Init(SceneBase* scene, Inventory* inventory, Player* player, GameObjectManager* goManager);
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

	const float ATTACK_DAMAGE = 10.f;

	float movement_speed;
	float activeRange;
	float attackRange;
	float defendRange;
	float shootTimer;

	PROWLER_STATE state;
	float attackAnimationTimer;
	float spawnAnimationTimer;
	float stunnedTimer;
	float spawnCooldownTimer;

	bool readyToSpawnMonkey;
	bool readyToAttack;

	Player* player;
	
	SpriteAnimation* animatedSprites;
	Inventory* inventory;
	GameObjectManager* goManager;
};


#endif