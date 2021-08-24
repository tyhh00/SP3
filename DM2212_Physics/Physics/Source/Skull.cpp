#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Skull.h"

Skull::Skull(Mesh* _mesh, int _quantity) : Consumable(I_SKULL, mesh)
{
	isStackable = true;
	quantity = _quantity;

	input = Input::GetInstance();

	mesh = _mesh;
}

void Skull::Init()
{
}

void Skull::Update(double dt)
{
	
}

bool Skull::IsEqual(Item* item1)
{
	Skull* checkSkull = static_cast<Skull*>(item1);
	
	return false;
}