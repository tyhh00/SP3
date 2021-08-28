 
#include "Dash.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "Player.h"
#include "../Source/SoundController/SoundController.h"


DashAbility::DashAbility(Mesh* mesh) : Ability('Q', ABILITY_DASH, 3.f, mesh)
{
	input = Input::GetInstance();
	dashTimer = 0;
	dashDir = 0;
	isDashing = false;
	maxVel = 0;
}

DashAbility::~DashAbility()
{
}

void DashAbility::Init()
{
}

void DashAbility::Update(double dt)
{
	abilityCD_timeleft -= dt;
	if (abilityCD_timeleft < 0)
		abilityCD_timeleft = 0.0f;

	if (input->IsKeyPressed(buttonChar) && abilityCD_timeleft <= 0)
	{
		//check if player's vel is not zero and is not already dashing
		if (!player->physics->GetVelocity().IsZero() && !isDashing)
		{
			CSoundController::GetInstance()->PlaySoundByID(DASH);
			abilityCD_timeleft = GetCooldownDuration();
			isDashing = true;
		}
	}

	//if in dashing phase
	if (isDashing)
	{
		//disable gravity
		player->physics->SetGravityUpdate(false);

		//disable collision
		player->enableCollision = false;

		//start dash timer
		dashTimer += dt;
		std::cout << dashTimer << std::endl;

		//check if dash time is over
		if (dashTimer > 0.25f)
		{
			//reset dash timer, is dashing and physics update
			dashTimer = 0;
			isDashing = false;
			player->enableCollision = true;
			player->physics->SetGravityUpdate(true);
		}

		//double check if player vel is not zero if not will have divide by zero error in normalized()
		if (!player->physics->GetVelocity().IsZero())
		{
			//use the player's accel to determine the dir of the dash
			float dir = dashDir * 200 * 200 * dt;

			maxVel = 100;
			std::cout << maxVel << std::endl;

			//add the dash dir to the player's vel
			player->physics->AddVelocity(Vector3(dir, 0, 0));
		}
		std::cout << "VEL: " << player->physics->GetVelocity() << std::endl;
	}
	else
	{
		maxVel = 20;
		player->enableCollision = true;
	}

	if (isDashing)
	{
		Player* playerptr = dynamic_cast<Player*>(player);
		dashDir = playerptr->dashDir;
		playerptr->curr_max_vel = maxVel;
	}

}

void DashAbility::Render()
{
}

//void DashAbility::UpdatePlayer(int& _dashDir, Physics* _playerPhysics, float& _maxVel, bool& _enableCollision)
//{
//	if (isDashing)
//	{
//		dashDir = _dashDir;
//		_maxVel = maxVel;
//	}
//}

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
