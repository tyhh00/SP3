#include "PlasmaRobot.h"
#include "Debug.h"

PlasmaRobot::PlasmaRobot()
	: target(target)
	, spawner(spawner)
	, Enemy(ROBOT_ENEMY_PLASMA)
	, targetRange(25)
	, dyingAnim(0.0)
	, attackCD(0.0)
	, attackTime(0.0)
	, stateActive(0.f)
	, walkingDir(RIGHT)
{
}

void PlasmaRobot::Init(GameObject* target, BulletSpawner* spawner)
{
	Enemy::Init();

	//HEALTH
	this->currentHP = 10;
	this->target = target;
	this->spawner = spawner;

	//ANIMATED SPRITE
	state = IDLE;
	animatedSprite = MeshBuilder::GenerateSpriteAnimation(4, 12, 2.0f, 2.0f);
	animatedSprite->AddAnimation("idle-right", 0, 3);
	animatedSprite->AddAnimation("walking-right", 4, 11);
	animatedSprite->AddAnimation("chargingattack-right", 12, 15);

	animatedSprite->AddAnimation("idle-left", 25, 28);
	animatedSprite->AddAnimation("walking-left", 29, 36);
	animatedSprite->AddAnimation("chargingattack-left", 37, 40);
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
	//DEBUG_MSG("enemypos" << this->pos);
	stateActive += dt;
	animatedSprite->Update(dt);

	if (this->physics->GetVelocity().LengthSquared() > 0.1)
	{
		Vector3 V = this->physics->GetVelocity();
		Vector3 right = Vector3(1, 0, 0);
		if (V.Dot(right) > 0)
		{
			walkingDir = RIGHT;
		}
		else
		{
			walkingDir = LEFT;
		}
	}

	if (state == IDLE && stateActive > 3.0)
	{
		state = WALKING;
		stateActive = 0.0;
	}
	if (state == WALKING)
	{
		animatedSprite->PlayAnimation("walking-" + GetWalkingDirection(), -1, 3.3);
		Vector3 dis = target->pos - this->pos;
		this->physics->SetVelocity(dis.Normalized() * 6);
		if (dis.LengthSquared() <= targetRange * targetRange)
		{
			state = AIMING;
		}
		else if (stateActive > 3.0)
		{
			stateActive = 0;
			state = IDLE;
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
			animatedSprite->PlayAnimation("idle-"+GetWalkingDirection(), -1, 1.2);
			attackCD -= dt;
			if (attackCD <= 0.0)
			{
				attackCD = ATTACK_COOLDOWN;
				animatedSprite->PlayAnimation("walking-" + GetWalkingDirection(), -1, 3.3);
				Vector3 dis = target->pos - this->pos;
				spawner->SpawnBullet(this->pos, dis * BULLET_SPEED, dis);
				DEBUG_MSG("Spawning bullet at " << this->pos);
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

std::string PlasmaRobot::GetWalkingDirection()
{
	if (walkingDir == LEFT)
		return "left";
	else if (walkingDir == RIGHT)
		return "right";
}
