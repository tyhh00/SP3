#pragma once
#include "Enemy.h"
#include "BulletSpawner.h"

class PlasmaRobot : public Enemy
{
	const float ATTACK_COOLDOWN = 3.0f;
	const float BULLET_SPEED = 19.0f;
public:
	PlasmaRobot(GameObject* target, BulletSpawner* spawner);
	~PlasmaRobot();
	void Update(double dt);
	void CollidedWith(GameObject* go);

private:
	enum ROBOT_STATE
	{
		WALKING,
		AIMING,
		DYING,
	};

	ROBOT_STATE state;
	float targetRange;
	float walkingSpeed;

	SpriteAnimation* animatedSprite;
	float dyingAnim;

	float attackCD;
	float attackTime;

	GameObject* target;
	BulletSpawner* spawner;

};

