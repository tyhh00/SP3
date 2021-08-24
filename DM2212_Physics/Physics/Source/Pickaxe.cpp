#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Pickaxe.h"

Pickaxe::Pickaxe(Mesh* _mesh) : Weapon(I_PICKAXE, mesh)
{
	isStackable = false;
	mesh = _mesh;
}

Pickaxe::~Pickaxe()
{
}

void Pickaxe::Init()
{

}

void Pickaxe::Update(double dt)
{

}

bool Pickaxe::IsEqual(Item* item1)
{
	//Flashlight* checkApple = static_cast<Flashlight*>(item1);
	return false;
}