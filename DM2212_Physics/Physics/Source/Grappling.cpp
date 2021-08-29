
#include "Grappling.h"
#include "Application.h"
#include "Player.h"
#include "MeshBuilder.h"
#include "../Source/SoundController/SoundController.h"


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
				CSoundController::GetInstance()->PlaySoundByID(GRAPPLER);
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

		player->physics->AddVelocity(displacement);

		maxVel = 100;
	}
	else if (player != nullptr)
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

void GrapplingAbility::Reset()
{
	abilityCD_timeleft = 0;
	isGrappling = false;
	grapplingHook.active = false;
	temp = Vector3(0, 0, 0);
	initialDisplacement = Vector3(0, 0, 0);
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