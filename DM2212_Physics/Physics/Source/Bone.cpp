#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Bone.h"

Bone::Bone(Mesh* _mesh, int _quantity) : Consumable(I_BONE, mesh)
{
	isStackable = true;
	quantity = _quantity;

	input = Input::GetInstance();

	mesh = _mesh;
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