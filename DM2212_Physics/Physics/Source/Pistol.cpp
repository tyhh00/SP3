#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Pistol.h"

Pistol::Pistol(GameObjectManager* _goManager, LightBullet* _lightBullet, Mesh* _mesh) : Weapon(I_PISTOL, mesh, bulletSpawner)
{
	bulletSpawner = new BulletSpawner(_goManager, _lightBullet);
	isStackable = false;
	mesh = _mesh;
}

Pistol::~Pistol()
{
}

void Pistol::Init()
{

}

void Pistol::Update(double dt)
{

}

bool Pistol::IsEqual(Item* item1)
{
	//Flashlight* checkApple = static_cast<Flashlight*>(item1);
	return false;
}