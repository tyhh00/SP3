
#include "SlowTime.h"
#include "Application.h"
#include "MeshBuilder.h"


SlowTimeAbility::SlowTimeAbility() : Ability('Q', ABILITY_SLOWTIME, 3.f)
{
	input = Input::GetInstance();
	maxVel = 0;
}

SlowTimeAbility::~SlowTimeAbility()
{
}

void SlowTimeAbility::Update(double dt)
{
	abilityCD_timeleft -= dt;
	if (abilityCD_timeleft < 0)
		abilityCD_timeleft = 0.0f;

	if (input->IsKeyPressed('Q'))
		goManager->SetmSpeed(0.1f);

}

void SlowTimeAbility::Render()
{
}

void SlowTimeAbility::UpdatePlayer(Physics* physics)
{
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
