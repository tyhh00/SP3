
#include "Grappling.h"
#include "Application.h"
#include "MeshBuilder.h"


GrapplingAbility::GrapplingAbility() : Ability('Q', ABILITY_GRAPPLER)
{
	//Store keyboard instance
	input = Input::GetInstance();

	isGrappling = false;
}

GrapplingAbility::~GrapplingAbility()
{
}

void GrapplingAbility::Update(double dt)
{
	std::cout << "updating grappling ability" << std::endl;
	if (input->IsMousePressed(0))
	{
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		// convert to world space
		x /= (w / camera->screenWidth);
		y = h - y;
		y /= (h / camera->screenHeight);
		std::cout << x << " " << y << std::endl;

		temp = Vector3(x, y, 0);
		isGrappling = true;
		initialDisplacement = temp - playerPos;
		std::cout << "initial pos" << playerPos << std::endl;

		grapplingHook.active = true;
	}

	if (isGrappling)
	{
		Vector3 displacement = temp - playerPos;
		Vector3 displacement3 = playerPos - temp;

		grapplingHook.scale = Vector3(displacement.Length() / 2, 1, 1);
		grapplingHook.pos = playerPos + Vector3(displacement.x / 2, displacement.y / 2, 0);
		grapplingHook.physics->SetNormal(displacement.Normalized());

		playerPhysics->AddVelocity(Vector3(initialDisplacement.x, 0, 0));
		if (playerPhysics->GetVelocity().x > 0)
		{
			if (playerPos.x >= temp.x - displacement3.x)
			{
				std::cout << "Stopped grappling" << std::endl;
				std::cout << displacement3 << std::endl;
				std::cout << playerPos.x << std::endl;
				isGrappling = false;
				grapplingHook.active = false;
			}
		}
		else
		{
			if (playerPos.x <= temp.x - displacement3.x)
			{
				std::cout << "Stopped grappling" << std::endl;
				std::cout << displacement3 << std::endl;
				std::cout << playerPos.x << std::endl;
				isGrappling = false;
				grapplingHook.active = false;
			}
		}
	}
}

void GrapplingAbility::UpdatePlayer(Vector3& pos, Physics* _playerPhysics)
{
	playerPos = pos;
	playerPhysics = _playerPhysics;
}


void GrapplingAbility::Render()
{
	if (grapplingHook.active)
	{
		//scene->modelStack.PushMatrix();
		//scene->modelStack.Translate(grapplingHook.pos.x, grapplingHook.pos.y, grapplingHook.pos.z);
		//scene->modelStack.Scale(grapplingHook.scale.x, grapplingHook.scale.y, grapplingHook.scale.z);
		//scene->RenderMesh(meshList[GEO_WALL], false);
		//scene->modelStack.PopMatrix();
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