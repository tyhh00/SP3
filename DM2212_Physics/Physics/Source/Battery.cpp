#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Battery.h"

Battery::Battery(Mesh* _mesh, int _quantity) : Consumable(I_BATTERY, mesh)
{
	isStackable = true;
	quantity = _quantity;

	input = Input::GetInstance();
	mesh = _mesh;
}

void Battery::Init()
{
}

void Battery::Update(double dt)
{
	
}

bool Battery::IsEqual(Item* item1)
{
	Battery* checkBattery = static_cast<Battery*>(item1);
	
	return true;
}
