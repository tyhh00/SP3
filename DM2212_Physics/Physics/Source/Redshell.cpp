#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Redshell.h"

Redshell::Redshell(int _quantity) : Consumable(I_REDSHELL, mesh)
{
	isStackable = true;
	quantity = _quantity;

	input = Input::GetInstance();

	mesh = MeshBuilder::GenerateQuad("redShell", Color(1, 1, 1), 2.0f);
	mesh->textureID = LoadTGA("Image/Tiles/OCEAN_seashell2.tga");
}

void Redshell::Init()
{
}

void Redshell::Update(double dt)
{
	
}

bool Redshell::IsEqual(Item* item1)
{
	return true;
}