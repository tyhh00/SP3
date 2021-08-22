#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Battery.h"

Battery::Battery(int _quantity) : Consumable(I_BATTERY)
{
	isStackable = true;
	quantity = _quantity;

	input = Input::GetInstance();
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
