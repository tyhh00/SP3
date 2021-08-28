#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Battery.h"
#include "Flashlight.h"

Battery::Battery(Mesh* _mesh, Inventory* inventory, int _quantity) : Consumable(I_BATTERY, mesh)
{
	isStackable = true;
	quantity = _quantity;

	input = Input::GetInstance();
	mesh = _mesh;
	this->inventory = inventory;
	isDurable = true;
}

void Battery::Init()
{
}

void Battery::Update(double dt)
{
	if (input->IsKeyPressed('E'))
	{
		Item* item = inventory->GetItem(I_FLASHLIGHT);
		if (item != nullptr)
		{
			Flashlight* flashlight = dynamic_cast<Flashlight*>(item);
			flashlight->RefillBattery();
			this->RemoveQuantity(1);
		}
	}
}

bool Battery::IsEqual(Item* item1)
{
	Battery* checkBattery = static_cast<Battery*>(item1);
	
	return true;
}
