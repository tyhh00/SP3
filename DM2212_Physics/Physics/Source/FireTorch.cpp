#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "FireTorch.h"

FireTorch::FireTorch() : Weapon(I_FIRETORCH)
{
	isStackable = false;
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