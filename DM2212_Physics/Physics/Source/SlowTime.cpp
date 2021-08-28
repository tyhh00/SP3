
#include "SlowTime.h"
#include "Application.h"
#include "MeshBuilder.h"


SlowTimeAbility::SlowTimeAbility(Mesh* mesh) : Ability('Q', ABILITY_SLOWTIME, 3.f, mesh)
{
	input = Input::GetInstance();
	abilityTimer = 0;
}

SlowTimeAbility::~SlowTimeAbility()
{}

void SlowTimeAbility::Init()
{
}

void SlowTimeAbility::Update(double dt)
{
	abilityCD_timeleft -= dt;
	if (abilityCD_timeleft < 0)
		abilityCD_timeleft = 0.0f;


	scene->lights[0].position.Set(player->pos.x, player->pos.y, player->pos.z + 10);

	if (input->IsKeyPressed(buttonChar) && abilityTimer <= 0)
	{
		abilityTimer = 8;
		goManager->SetmSpeed(0.1f);
		SetLighting();
	}

	if (abilityTimer <= 0)
	{ 
		goManager->SetmSpeed(1.f);
		scene->InitLights();
	}
	else
	{
		abilityTimer -= dt;
	}
}

void SlowTimeAbility::Render()
{}

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
