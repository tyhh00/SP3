#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Trident.h"

Trident::Trident() : Weapon(I_TRIDENT)
{
	isStackable = false;
	active = false;
	intensified = false;
	input = Input::GetInstance();
	currBatt = maxBatt = 100;
	batt_usage_rate = 2.0f;
	rate_multiplier = 1.0f;
}

Trident::~Trident()
{
}

void Trident::Init()
{

}

void Trident::Update(double dt)
{
	light_pos.Set(scene->lights[1].position.x, scene->lights[1].position.y, scene->lights[1].position.z);

	if (input->IsKeyPressed('F'))
	{
		active = !active;
		if (active && currBatt > 0)
		{
			scene->ToggleLightOnOff(1, true);
			std::cout << "Trident has been turned on." << std::endl;
		}
		else
		{
			scene->ToggleLightOnOff(1, false);
			intensified = false;
			std::cout << "Trident has been turned off." << std::endl;
		}
	}


	if (active)
	{
		if (input->IsMousePressed(0))
		{
			intensified = true;
			rate_multiplier = 2.0f;
			scene->ToggleLightPower(1, scene->lights[1].defaultPower + 2);
		}
		else if (input->IsMouseReleased(0))
		{
			intensified = false;
			rate_multiplier = 1.0f;
			scene->ToggleLightPower(1, scene->lights[1].defaultPower);
		}
	
		currBatt -= rate_multiplier * batt_usage_rate * dt;
		durability = currBatt / maxBatt;

	}
}

bool Trident::IsEqual(Item* item1)
{
	Trident* checkApple = static_cast<Trident*>(item1);
	return false;
}

bool Trident::isWithinLight(Vector3 objPos)
{
	// CALCULATING LIGHT RADIUS
	float thetaR = acos(scene->lights[1].cosCutoff);
	light_radius = scene->lights[1].position.z * tan(thetaR);

	float dis = (Vector3(objPos.x, objPos.y, 0) - Vector3(light_pos.x, light_pos.y, 0)).Length();
	if (active && dis < light_radius)
	{
		return true;
	}
	return false;
}

bool Trident::isIntensified()
{
	return intensified;
}

