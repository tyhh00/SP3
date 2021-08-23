#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "FireTorch.h"

FireTorch::FireTorch(Mesh* _mesh) : Weapon(I_FIRETORCH, mesh)
{
	isStackable = false;
	mesh = _mesh;
}

FireTorch::~FireTorch()
{
}

void FireTorch::Init()
{

}

void FireTorch::Update(double dt)
{

}

bool FireTorch::IsEqual(Item* item1)
{
	//Flashlight* checkApple = static_cast<Flashlight*>(item1);
	return false;
}