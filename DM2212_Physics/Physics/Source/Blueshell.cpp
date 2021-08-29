#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Blueshell.h"

Blueshell::Blueshell(int _quantity) : Consumable(I_BLUESHELL, mesh)
{
	//STACKING
	isStackable = true;

	//QUANTITY
	quantity = _quantity;

	//INPUT
	input = Input::GetInstance();

	//MESH
	mesh = MeshBuilder::GenerateQuad("blueShell", Color(1, 1, 1), 2.0f);
	mesh->textureID = LoadTGA("Image/Tiles/OCEAN_seashell1.tga");
}

void Blueshell::Init()
{
}

void Blueshell::Update(double dt)
{
	
}

bool Blueshell::IsEqual(Item* item1)
{
	return true;
}