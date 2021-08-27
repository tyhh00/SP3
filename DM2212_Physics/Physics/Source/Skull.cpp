#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Skull.h"

Skull::Skull(int _quantity) : Consumable(I_SKULL, mesh)
{
	isStackable = true;
	quantity = _quantity;

	input = Input::GetInstance();

	mesh = MeshBuilder::GenerateQuad("bone", Color(1, 1, 1), 2.0f);
	mesh->textureID = LoadTGA("Image//Items/skull.tga");
}

void Skull::Init()
{
}

void Skull::Update(double dt)
{
	
}

bool Skull::IsEqual(Item* item1)
{
	return true;
}