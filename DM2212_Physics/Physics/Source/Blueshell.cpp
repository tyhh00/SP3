#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Blueshell.h"

Blueshell::Blueshell(int _quantity) : Consumable(I_BLUESHELL, mesh)
{
	isStackable = true;
	quantity = _quantity;

	input = Input::GetInstance();

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