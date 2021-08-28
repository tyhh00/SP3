#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

Flashlight::Flashlight(Mesh* _mesh) : Weapon(I_FLASHLIGHT, mesh)
{
	isStackable = true;
	active = false;
	intensified = false;
	input = Input::GetInstance();
	currBatt = maxBatt = 100;
	batt_usage_rate = 2.0f;
	rate_multiplier = 1.0f;
	mesh = _mesh;
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

	if (input->IsKeyPressed('E'))
	{
		active = !active;
		if (active && currBatt > 0)
		{
			scene->ToggleLightOnOff(1, true);
			std::cout << "Flashlight has been turned on." << std::endl;
		}
		else
		{
			scene->ToggleLightOnOff(1, false);
			intensified = false;
			std::cout << "Flashlight has been turned off." << std::endl;
		}
	}


	if (active)
	{
		scene->ToggleLightPower(1, scene->lights[1].defaultPower);
		if (input->IsMousePressed(0))
		{
			intensified = true;
			rate_multiplier = 2.0f;
			scene->ToggleLightPower(1, scene->lights[1].defaultPower + 2);
			scene->ToggleLightRadius(1, 15, 30);
		}
		else if (input->IsMouseReleased(0))
		{
			intensified = false;
			rate_multiplier = 1.0f;
			scene->ToggleLightPower(1, scene->lights[1].defaultPower);
			scene->ToggleLightRadius(1, 30, 45);
		}
	
		currBatt -= rate_multiplier * batt_usage_rate * dt;
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

bool Flashlight::isIntensified()
{
	return intensified;
}

void Flashlight::RefillBattery()
{
	currBatt = maxBatt;
}

