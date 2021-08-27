#include "PlasmaEngine.h"

PlasmaEngine::PlasmaEngine() : GameObject(GO_PLASMAENGINE)
{
}

PlasmaEngine::~PlasmaEngine()
{
}

void PlasmaEngine::Init(BulletSpawner* spawner, GameObject* player, float bulletSpawnCD)
{
	engineType = FRONT;
	this->spawner = spawner;

	this->player = player;
	this->bulletSpawnCD = bulletSpawnCD;
	this->lastSpawnBullet = this->bulletSpawnCD;

	this->rangeToAttack = 175;


}

void PlasmaEngine::Update(double dt)
{
	lastSpawnBullet -= dt;
	if (lastSpawnBullet <= 0)
	{
		Vector3 dis = player->pos - pos;
		if (!dis.IsZero() && dis.LengthSquared() <= rangeToAttack * rangeToAttack)
		{
			lastSpawnBullet = bulletSpawnCD;
			Vector3 N = this->physics->GetNormal();
			Vector3 NP = Vector3(-N.y, N.x);
			spawner->SpawnBullet(this->pos, NP,NP);
		}

	}

}
