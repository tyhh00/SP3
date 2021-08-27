
#include "SlowTime.h"
#include "Application.h"
#include "MeshBuilder.h"


SlowTimeAbility::SlowTimeAbility() : Ability('Q', ABILITY_SLOWTIME, 3.f)
{
	input = Input::GetInstance();
	abilityTimer = 0;
}

SlowTimeAbility::~SlowTimeAbility()
{}

void SlowTimeAbility::Update(double dt)
{
	abilityCD_timeleft -= dt;
	if (abilityCD_timeleft < 0)
		abilityCD_timeleft = 0.0f;

	if (input->IsKeyPressed('Q') && abilityTimer <= 0)
	{
		abilityTimer = 8;
		goManager->SetmSpeed(0.1f);
		scene->setBLightEnabled(true);
	}

	if (abilityTimer <= 0)
	{ 
		goManager->SetmSpeed(1.f);
		scene->setBLightEnabled(false);
	}
	else
	{
		abilityTimer -= dt;
	}

	std::cout << "time: " << abilityTimer << std::endl;
}

void SlowTimeAbility::Render()
{}

ABILITY_TYPE SlowTimeAbility::GetAbilityType()
{
	return type;
}

void SlowTimeAbility::setGOM(GameObjectManager* goM)
{
	goManager = goM;
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
