#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

Flashlight::Flashlight()
{
	isStackable = true;
	itemType = I_FLASHLIGHT;
	active = false;
	input = Input::GetInstance();
	currBatt = maxBatt = 100;
	batt_usage_rate = 2;
}

Flashlight::~Flashlight()
{
}

void Flashlight::Init()
{
}

void Flashlight::Update(double dt)
{
	if (input->IsKeyPressed('F'))
	{
		active = !active;
		if (active && currBatt > 0)
		{
			scene->ToggleLight(1, true);
			std::cout << "Flashlight has been turned on." << std::endl;
		}
		else
		{
			scene->ToggleLight(1, false);
			std::cout << "Flashlight has been turned off." << std::endl;
		}
	}

	if (active)
	{
		currBatt -= batt_usage_rate * dt;
		durability = currBatt / maxBatt;
	}
}

bool Flashlight::IsEqual(Item* item1)
{
	Flashlight* checkApple = static_cast<Flashlight*>(item1);
	return false;
}

