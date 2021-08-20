#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

Flashlight::Flashlight(int _quantity, int stemLength)
	: stemLength(stemLength)
{
	isStackable = true;
	quantity = _quantity;
	itemType = I_FLASHLIGHT;

	input = Input::GetInstance();
}

void Flashlight::Init()
{
}

void Flashlight::Update(double dt)
{
	if (input->IsKeyPressed('G'))
	{
		RemoveQuantity(1);
	}
}

bool Flashlight::IsEqual(Item* item1)
{
	Flashlight* checkFlashlight = static_cast<Flashlight*>(item1);
	std::cout << "first Flashlight has stem length: " << this->stemLength << " and second Flashlight has stem length: " << checkFlashlight->stemLength << std::endl;
	if (this->stemLength == checkFlashlight->stemLength)
		return true;
	return false;
}
