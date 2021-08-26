#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Bone.h"

Bone::Bone(int _quantity) : Consumable(I_BONE, mesh)
{
	isStackable = true;
	quantity = _quantity;

	input = Input::GetInstance();

	mesh = MeshBuilder::GenerateQuad("bone", Color(1, 1, 1), 2.0f);
	mesh->textureID = LoadTGA("Image/Items/bone.tga");
}

void Bone::Init()
{
}

void Bone::Update(double dt)
{
	
}

bool Bone::IsEqual(Item* item1)
{
	Bone* checkBone = static_cast<Bone*>(item1);
	
	return false;
}