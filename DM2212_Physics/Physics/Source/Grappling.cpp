
#include "Grappling.h"
#include "Application.h"
#include "Player.h"
#include "MeshBuilder.h"


GrapplingAbility::GrapplingAbility(Mesh* mesh) : Ability('Z', ABILITY_GRAPPLER, 5.0f, mesh)
{
	//Store keyboard instance
	input = Input::GetInstance();

	isGrappling = false;
	maxVel = 0;
	gradualVelTimer = 0;
}

GrapplingAbility::~GrapplingAbility()
{
}

void GrapplingAbility::Init()
{
}

void GrapplingAbility::Update(double dt)
{
	abilityCD_timeleft -= dt;
	if (abilityCD_timeleft < 0)
		abilityCD_timeleft = 0.0f;

	//attach grappling hook
	if (input->IsKeyPressed(buttonChar) && abilityCD_timeleft <= 0)
	{
		double x, y;
		CursorToWorldPosition(x, y);

		//check if mouse clicked pos is on any tile block
		for (GameObject* go : goManager->GetStationaryList())
		{
			if ((x > go->pos.x - go->scale.x && x < go->pos.x + go->scale.x) && (y > go->pos.y - go->scale.y && y < go->pos.y + go->scale.y))
			{
				temp = Vector3(x, y, 0);
				initialDisplacement = temp - player->pos;
				isGrappling = true;
				grapplingHook.active = true;
				abilityCD_timeleft = GetCooldownDuration();
			}
		}


	}
	else if (input->IsKeyReleased(buttonChar)) //detach grappling hook
	{
		isGrappling = false;
		grapplingHook.active = false;
	}


	if (isGrappling)
	{
		Vector3 displacement = temp - player->pos;
		Vector3 displacement3 = player->pos - temp;

		grapplingHook.scale = Vector3(displacement.Length() / 2, 1, 1);
		grapplingHook.pos = player->pos + Vector3(displacement.x / 2, displacement.y / 2, 0);
		grapplingHook.physics->SetNormal(displacement.Normalized());

		//Vector3 halfDisplacement = Vector3(displacement.x / 2, displacement.y / 2, displacement.z);
		player->physics->AddVelocity(displacement);

		//playerPhysics->AddVelocity(Vector3(initialDisplacement.x, 0, 0));
		maxVel = 100;

		//if (playerPhysics->GetVelocity().x > 0)
		//{
		//	if (playerPos.x >= temp.x - displacement3.x)
		//	{
		//		//playerPhysics->AddVelocity(Vector3(0, initialDisplacement.Length(), 0));
		//		std::cout << "Stopped grappling" << std::endl;
		//		std::cout << displacement3 << std::endl;
		//		std::cout << playerPos.x << std::endl;
		//		isGrappling = false;
		//		grapplingHook.active = false;
		//	}
		//}
		//else
		//{
		//	if (playerPos.x <= temp.x - displacement3.x)
		//	{
		//		//playerPhysics->AddVelocity(Vector3(0, initialDisplacement.Length(), 0));
		//		std::cout << "Stopped grappling" << std::endl;
		//		std::cout << displacement3 << std::endl;
		//		std::cout << playerPos.x << std::endl;
		//		isGrappling = false;
		//		grapplingHook.active = false;
		//	}
		//}
		//std::cout << maxVel << std::endl;
	}
	else if (player->physics != nullptr)
	{
		if (player->physics->GetVelocity().x < 1 && player->physics->GetVelocity().x > -1)
		{
			maxVel = 20;
			endGrappled = true;
		}
	}

	if (isGrappling || !endGrappled)
	{
		Player* playerptr = dynamic_cast<Player*>(player);
		playerptr->curr_max_vel = maxVel;
	}
}

//void GrapplingAbility::UpdatePlayer(Vector3& pos, Physics* _playerPhysics, float& _maxVel)
//{
//	playerPhysics = _playerPhysics;
//	if (isGrappling || !endGrappled)
//	{
//		playerPos = pos;
//		_maxVel = maxVel;
//	}
//}


void GrapplingAbility::Render()
{
	if (grapplingHook.active)
	{
		float angle = Math::RadianToDegree(atan2(grapplingHook.physics->GetNormal().y, grapplingHook.physics->GetNormal().x));
		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(grapplingHook.pos.x, grapplingHook.pos.y, grapplingHook.pos.z);
		scene->modelStack.Rotate(angle + grapplingHook.physics->GetRotateZ(), 0, 0, 1);
		scene->modelStack.Scale(grapplingHook.scale.x, grapplingHook.scale.y, grapplingHook.scale.z);
		scene->RenderMesh(grapplingHook.mesh, false);
		scene->modelStack.PopMatrix();
	}
}

ABILITY_TYPE GrapplingAbility::GetAbilityType()
{
	return type;
}


void GrapplingAbility::CursorToWorldPosition(double& theX, double& theY)
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