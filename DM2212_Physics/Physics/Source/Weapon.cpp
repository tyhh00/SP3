#include "Weapon.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Weapon::Weapon(ITEM_TYPE type, Mesh* _mesh) : Item(G_WEAPON, type, 1, false, mesh)
{
	mesh = _mesh;
}

void Weapon::Init()
{
	
}

void Weapon::Update(double dt)
{
	
}
