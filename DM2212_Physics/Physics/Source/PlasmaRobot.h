#pragma once
#include "Enemy.h"
#include "BulletSpawner.h"

class PlasmaRobot : public Enemy
{
	const float ATTACK_COOLDOWN = 4.0f;
public:
	PlasmaRobot();
	~PlasmaRobot();
	void Init(GameObject* target, BulletSpawner* spawner);
	void Update(double dt);
	void CollidedWith(GameObject* go);


	std::string GetWalkingDirection();

private:
	enum ROBOT_STATE
	{
		IDLE,
		WALKING,
		AIMING,
		DYING,
	};
	enum WALKING_DIRECTION
	{
		LEFT,
		RIGHT
	};

	ROBOT_STATE state;
	WALKING_DIRECTION walkingDir;
	float targetRange;
	float walkingSpeed;

	SpriteAnimation* animatedSprite;
	float dyingAnim;

	float jumpCD
		, jumpCD_timeleft;


	float attackCD;
	float attackTime;

	float stateActive; //How long a state has been active for

	GameObject* target;
	BulletSpawner* spawner;

};

