#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Bottle.h"

Bottle::Bottle(int _quantity) : Consumable(I_BOTTLE, mesh)
{
	//STACKING
	isStackable = true;

	//QUANTITY
	quantity = _quantity;

	//INPUT
	input = Input::GetInstance();

	//MESH
	mesh = MeshBuilder::GenerateQuad("redShell", Color(1, 1, 1), 2.0f);
	mesh->textureID = LoadTGA("Image/Tiles/OCEAN_seashell2.tga");
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