
#include "Dash.h"
#include "Application.h"
#include "MeshBuilder.h"


DashAbility::DashAbility() : Ability('Q', ABILITY_DASH, 10.f)
{
	input = Input::GetInstance();
	dashTimer = 0;
	dashDir = 0;
	isDashing = false;
	enableCollision = false;
	maxVel = 0;
}

DashAbility::~DashAbility()
{
}

void DashAbility::Update(double dt)
{
	if (input->IsKeyPressed(buttonChar))
	{
		if (!playerPhysics->GetVelocity().IsZero() && !isDashing)
			isDashing = true;
	}

	//if in dashing phase
	if (isDashing)
	{
		//disable gravity
		playerPhysics->SetGravityUpdate(false);

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
			playerPhysics->SetGravityUpdate(true);
		}

		//double check if player vel is not zero if not will have divide by zero error in normalized()
		if (!playerPhysics->GetVelocity().IsZero())
		{
			//use the player's accel to determine the dir of the dash
			float dir = dashDir * 200 * 200 * dt;

			maxVel = 100;
			std::cout << maxVel << std::endl;

			//add the dash dir to the player's vel
			playerPhysics->AddVelocity(Vector3(dir, 0, 0));
		}
		std::cout << "VEL: " << playerPhysics->GetVelocity() << std::endl;
	}
	else
	{
		maxVel = 20;
		enableCollision = true;
	}

}

void DashAbility::Render()
{
}

void DashAbility::UpdatePlayer(int& _dashDir, Physics* _playerPhysics, float& _maxVel, bool& _enableCollision)
{
	playerPhysics = _playerPhysics;
	_enableCollision = enableCollision;
	if (isDashing)
	{
		dashDir = _dashDir;
		_maxVel = maxVel;
	}
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
