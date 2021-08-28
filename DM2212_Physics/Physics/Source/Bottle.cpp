#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Bottle.h"

Bottle::Bottle(int _quantity) : Consumable(I_BOTTLE, mesh)
{
	isStackable = true;
	quantity = _quantity;

	input = Input::GetInstance();

	mesh = MeshBuilder::GenerateQuad("Bottle", Color(1, 1, 1), 2.0f);
	mesh->textureID = LoadTGA("Image/Tiles/OCEAN_bottle.tga");
}

void Bottle::Init()
{
}

void Bottle::Update(double dt)
{
	
}

bool Bottle::IsEqual(Item* item1)
{
	return true;
}