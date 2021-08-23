#include "PlasmaRobot.h"

PlasmaRobot::PlasmaRobot(GameObject* target, BulletSpawner* spawner)
	: target(target)
	, spawner(spawner)
	, Enemy(ROBOT_ENEMY_PLASMA)
	, targetRange(25)
	, dyingAnim(0.0)
	, attackCD(0.0)
	, attackTime(0.0)
{
	//ANIMATED SPRITE
	animatedSprite = MeshBuilder::GenerateSpriteAnimation(12, 2, 2.0f, 2.0f);
	animatedSprite->AddAnimation("idle", 0, 3);
	animatedSprite->AddAnimation("walking", 4, 11);
	animatedSprite->AddAnimation("chargingattack", 12, 15);
	mesh = animatedSprite;
	mesh->textureID = LoadTGA("Image/robot_enemy1.tga");

	animatedSprite->PlayAnimation("idle", 0, 1.0f);
}

PlasmaRobot::~PlasmaRobot()
{
	delete spawner;
}

void PlasmaRobot::Update(double dt)
{
	if (state == WALKING)
	{
		animatedSprite->PlayAnimation("walking", -1, 3.3);
		Vector3 dis = this->pos - target->pos;
		if (dis.LengthSquared() <= targetRange * targetRange)
		{
			state = AIMING;
		}
	}
	if (state == AIMING)
	{

		Vector3 dis = this->pos - target->pos;
		if (dis.LengthSquared() >= targetRange * targetRange)
		{
			state = WALKING;
		}
		else
		{
			animatedSprite->PlayAnimation("idle", -1, 1.2);
			attackCD -= dt;
			if (attackCD <= 0.0)
			{
				animatedSprite->PlayAnimation("walking", -1, 3.3);
				attackCD = ATTACK_COOLDOWN;
				Vector3 dis = target->pos-this->pos;
				spawner->SpawnBullet(this->pos, dis * BULLET_SPEED, dis);
			}
		}
	}
	if (currentHP <= 0.0)
	{
		dead = true;
	}
}

void PlasmaRobot::CollidedWith(GameObject* col)
{
	if (col->pos.y + col->scale.y >= this->scale.y) //Not ground tile
	{
		if (state == WALKING)
		{
			if (col->pos.x <= this->pos.x) //Reverse direction
			{
				this->pos.x *= -1;
			}
			else if (col->pos.x >= this->pos.x)
			{
				this->pos.x *= -1;
			}
		}
	}
}