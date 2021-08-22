
#include "Dash.h"
#include "Application.h"
#include "MeshBuilder.h"


DashAbility::DashAbility() : Ability('Q', ABILITY_DASH)
{
	input = Input::GetInstance();
	dashTimer = 0;
	speed = 0;
	isDashing = false;
	enableCollision = false;
}

DashAbility::~DashAbility()
{
}

void DashAbility::Update(double dt)
{
	std::cout << "updating dash ability" << std::endl;
	if (input->IsKeyPressed('Q'))
	{
		if (!playerPhysics->GetVelocity().IsZero() && !isDashing)
			isDashing = true;
	}

	//if in dashing phase
	if (isDashing)
	{
		//disable collision
		enableCollision = false;

		//start dash timer
		dashTimer += dt;
		std::cout << dashTimer << std::endl;

		//check if dash time is over
		if (dashTimer > 0.25f)
		{
			//reset dash timer, is dashing and physics update
			dashTimer = 0;
			isDashing = false;
			enableCollision = true;
		}

		//double check if player vel is not zero if not will have divide by zero error in normalized()
		if (!playerPhysics->GetVelocity().IsZero())
		{
			//use the player's accel to determine the dir of the dash
			float dir = accel * speed * speed * dt;

			//add the dash dir to the player's vel
			playerPhysics->AddVelocity(Vector3(dir, 0, 0));
		}

		std::cout << playerPhysics->GetVelocity() << std::endl;
	}

}

void DashAbility::Render()
{
}

void DashAbility::UpdatePlayer(float& _accel, Physics* _playerPhysics, float& _speed, bool& _enableCollision, bool& _isDashing)
{
	playerPhysics = _playerPhysics;
	speed = _speed;
	enableCollision = _enableCollision;
	isDashing = _isDashing;
	accel = _accel;
}

ABILITY_TYPE DashAbility::GetAbilityType()
{
	return type;
}


void DashAbility::CursorToWorldPosition(double& theX, double& theY)
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
