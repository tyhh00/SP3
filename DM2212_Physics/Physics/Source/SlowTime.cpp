
#include "SlowTime.h"
#include "Application.h"
#include "MeshBuilder.h"


SlowTimeAbility::SlowTimeAbility(Mesh* mesh) : Ability('Q', ABILITY_SLOWTIME, 10.f, mesh)
{
	input = Input::GetInstance();
	abilityTimer = 0;
	abilityCD_timeleft = 0;
}

SlowTimeAbility::~SlowTimeAbility()
{}

void SlowTimeAbility::Init()
{
}

void SlowTimeAbility::Update(double dt)
{
	//ability cooldown timer
	abilityCD_timeleft -= dt;
	if (abilityCD_timeleft < 0)
		abilityCD_timeleft = 0.0f;

	//update lighting
	scene->lights[0].position.Set(player->pos.x, player->pos.y, player->pos.z + 10);

	//if ability timer < 0, activate ability
	if (input->IsKeyPressed(buttonChar) && abilityTimer <= 0)
	{
		abilityTimer = 8;
		abilityCD_timeleft = 10.0f;
		goManager->SetmSpeed(0.1f);
		SetLighting();
	}

	//if ability timer > 0, update ability timer. If <= 0, reset back to normal
	if (abilityTimer > 0)
	{ 
		abilityTimer -= dt;
		if (abilityTimer <= 0)
		{
			goManager->SetmSpeed(1.f);
			scene->InitLights();
		}
	}
}

void SlowTimeAbility::Render()
{}

void SlowTimeAbility::Reset() //reset
{
	abilityTimer = 0;
	abilityCD_timeleft = 0;
}

ABILITY_TYPE SlowTimeAbility::GetAbilityType()
{
	return type;
}

void SlowTimeAbility::CursorToWorldPosition(double& theX, double& theY)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	// convert to world space
	x /= (w / camera->screenWidth);
	y = h - y;
	y /= (h / camera->screenHeight);
	x -= camera->screenWidth * 0.5 - camera->position.x;
	y -= camera->screenHeight * 0.5 - camera->position.y;

	theX = x;
	theY = y;
}

void SlowTimeAbility::SetLighting()
{
	scene->ToggleLightColor(0, Color(0.702, 0.529, 1));
	scene->ToggleLightPower(0, 2);
	scene->lights[0].position.Set(player->pos.x, player->pos.y, player->pos.z + 10);
	scene->setBLightEnabled(true);
}
