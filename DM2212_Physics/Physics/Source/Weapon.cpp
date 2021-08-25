#include "Weapon.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Weapon::Weapon(ITEM_TYPE type, Mesh* _mesh, BulletSpawner* _bulletSpawner) : Item(G_WEAPON, type, 1, false, mesh)
{
	mesh = _mesh;
	bulletSpawner = _bulletSpawner;
}

Weapon::Weapon(ITEM_TYPE type, BulletSpawner* _bulletSpawner) : Item(G_WEAPON, type, 1, false)
{
	bulletSpawner = _bulletSpawner;
}

void Weapon::Init()
{
	
}

void Weapon::Update(double dt)
{
	
}

BulletSpawner* Weapon::GetBulletSpawner()
{
	return bulletSpawner;
}
