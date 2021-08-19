
#include "Portal.h"
#include "Application.h"


PortalAbilityManager::PortalAbilityManager() : camera(NULL)
{

}

void PortalAbilityManager::Init()
{
	ghost_portal = false;
	anim_timer = 0;

	startPortal.Init();
	endPortal.Init();

	conditionsMet = false;

	keyboard = Keyboard::GetInstance();

	state = DEFAULT;
}

void PortalAbilityManager::Update(Vector3& playerPos, double dt)
{
	switch (state)
	{
	case DEFAULT:
		// CHECK FOR ABILITY USE CONDITIONS
		if (keyboard->IsKeyDown('Z'))
		{
			if (conditionsMet && !endPortal.active)
			{
				// START OPENING START PORTAL
				startPortal.active = true;
				startPortal.pos = playerPos;
				startPortal.SetAnimation("opening", 0, 1.0);
				anim_timer = 0;
				camera->SetMode(Camera::CENTER);

				state = OPENING_ANIM;
				std::cout << "PORTAL ABILITY: Opening a Portal" << std::endl;
				break;
			}
			std::cout << "Cannot use Portal ability right now !" << std::endl;
		}
		break;
	case OPENING_ANIM:
		// OPENING START PORTAL
		if (anim_timer >= 1.0)
		{
			// PREP TO ENTER PLACING STATE
			startPortal.SetAnimation("idle", -1, 1.0);
			ghost_portal = true;

			state = PLACING;
			std::cout << "PORTAL ABILITY: Choose End Portal Location" << std::endl;
		}
		else
		{
			anim_timer += dt;
		}
		break;
	case PLACING:
	{
		// CHECK FOR PLACE PORTAL CONDITIONS
		if (keyboard->IsKeyReleased('Z'))
		{
			// START OPENING END PORTAL
			ghost_portal = false;
			endPortal.SetAnimation("opening", 0, 1.0);

			state = PLACING_ANIM;
			std::cout << "PORTAL ABILITY: Placing other Portal" << std::endl;
		}
		double mousePosX, mousePosY;
		CursorToWorldPosition(mousePosX, mousePosY);
		endPortal.pos.Set(mousePosX, mousePosY, 0);
	}
		break;
	case PLACING_ANIM:
		// OPENING END PORTAL
		if (anim_timer >= 1.0)
		{
			// MOVE PLAYER FROM ONE PLACE TO OTHER
			endPortal.SetAnimation("idle", -1, 1.0);
			anim_timer = 0;

			state = TELEPORTING;
			std::cout << "PORTAL ABILITY: Teleporting player now" << std::endl;
		}
		else
		{
			anim_timer += dt;
		}
		break;
	case TELEPORTING:
	{
		// MOVE PLAYER
		// CHECK IF PLAYER DONE MOVING
		float offset = 3.0f;
		if ((endPortal.pos - playerPos).Length() < offset)
		{
			// START CLOSING START PORTAL
			playerPos = endPortal.pos;
			camera->SetMode(Camera::EDGE);
			startPortal.SetAnimation("closing", -1, 1.0);
			anim_timer = 0;

			state = CLOSINGSTART_ANIM;
			std::cout << "PORTAL ABILITY: Teleportation End." << std::endl;
			break;
		}

		playerPos += (endPortal.pos - playerPos).Normalized() * 70.0f * dt;
	}
		break;
	case CLOSINGSTART_ANIM:
		// CLOSING START PORTAL
		if (anim_timer >= 1.0)
		{ 
			// RESET START PORTAL
			// START CLOSING END PORTAL
			startPortal.active = false;
			endPortal.SetAnimation("closing", -1, 1.0f);

			state = CLOSINGEND_ANIM;
		}
		else
		{
			anim_timer += dt;
		}
		break;
	case CLOSINGEND_ANIM:
		// CLOSING END PORTAL
		if (anim_timer >= 1.0)
		{
			// RESET END PORTAL
			endPortal.active = false;
			endPortal.SetAnimation("idle", -1, 1.0);

			state = DEFAULT;
		}
		else
		{
			anim_timer += dt;
		}
		break;
	}

	startPortal.Update(dt);
	endPortal.Update(dt);

}

void PortalAbilityManager::Render(SceneBase* scene)
{
	if (startPortal.active)
	{
		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(startPortal.pos.x, startPortal.pos.y, startPortal.pos.z);
		scene->modelStack.Scale(startPortal.scale.x, startPortal.scale.y, startPortal.scale.z);
		scene->RenderMesh(startPortal.mesh, true);
		scene->modelStack.PopMatrix();
	}
	if (endPortal.active 
		|| ghost_portal)
	{
		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(endPortal.pos.x, endPortal.pos.y, endPortal.pos.z);
		scene->modelStack.Scale(endPortal.scale.x, endPortal.scale.y, endPortal.scale.z);
		scene->RenderMesh(startPortal.mesh, true);
		scene->modelStack.PopMatrix();
	}
}

void PortalAbilityManager::SetCamera(Camera* camera)
{
	this->camera = camera;
}

void PortalAbilityManager::UpdateCondition(bool met)
{
	conditionsMet = met;
}

void PortalAbilityManager::CursorToWorldPosition(double& theX, double& theY)
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
