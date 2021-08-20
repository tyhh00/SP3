#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

Flashlight::Flashlight() : Weapon(I_FLASHLIGHT)
{
	isStackable = true;
	active = false;
	input = Input::GetInstance();
	currBatt = maxBatt = 100;
	batt_usage_rate = 2;

	// CALCULATING LIGHT POS
	float thetaR = acos(scene->lights[1].cosCutoff);
	light_radius = scene->lights[1].position.z * tan(thetaR);
}

Flashlight::~Flashlight()
{
}

void Flashlight::Init()
{
}

void Flashlight::Update(double dt)
{
	light_pos.Set(scene->lights[1].position.x, scene->lights[1].position.y, scene->lights[1].position.z);

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

bool Flashlight::isWithinLight(Vector3 objPos)
{
	float dis = (objPos - light_pos).Length();
	if (active && dis < light_radius)
	{
		return true;
	}
	return false;
}

