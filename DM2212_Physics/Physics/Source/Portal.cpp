
#include "Portal.h"
#include "Application.h"
#include "MeshBuilder.h"


PortalAbility::PortalAbility(Mesh* mesh) : Ability('Z', ABILITY_PORTAL, 10.0f, mesh)
{
	ghost_portal = false;
	ghost_player = false;
	anim_timer = 0;

	startPortal.Init();
	endPortal.Init();

	portalSprite = MeshBuilder::GenerateQuad("portal travel sprites", Color(1, 1, 1), 2.0f);
	portalSprite->textureID = LoadTGA("Image/PortalTravelSprite.tga");

	playerActiveState = true;
	conditionsMet = false;

	state = DEFAULT;
}

PortalAbility::~PortalAbility()
{
	if (portalSprite)
	{
		delete portalSprite;
		portalSprite = NULL;
	}
}

void PortalAbility::Update(double dt)
{
	switch (state)
	{
	case DEFAULT:
		// CHECK FOR ABILITY USE CONDITIONS
		if (Input::GetInstance()->IsKeyPressed('Z'))
		{
			if (conditionsMet && !endPortal.active)
			{
				// START OPENING START PORTAL
				startPortal.active = true;
				startPortal.SetAnimation("opening", 0, 0.3f);
				startPortal.pos = newPlayerPos;
				anim_timer = 0;
				ghost_portal = true;
				//camera->SetMode(Camera::CENTER);

				state = OPENING_ANIM;
				std::cout << "PORTAL ABILITY: Opening a Portal" << std::endl;
				break;
			}
			std::cout << "Cannot use Portal ability right now !" << std::endl;
		}
		break;
	case OPENING_ANIM:
		// OPENING START PORTAL
		if (anim_timer >= 0.3)
		{
			// PREP TO ENTER PLACING STATE
			startPortal.SetAnimation("idle", -1, 1.0);
			state = PLACING;
			break;
		}
		else
		{
			anim_timer += dt;
		}
		
		if (Input::GetInstance()->IsKeyDown('Z'))
		{
			double mousePosX, mousePosY;
			CursorToWorldPosition(mousePosX, mousePosY);
			endPortal.pos.Set(mousePosX, mousePosY, 0);
		}
		break;
	case PLACING:
	{
		// CHECK FOR PLACE PORTAL CONDITIONS
		if (!Input::GetInstance()->IsKeyDown('Z'))
		{
			// START OPENING END PORTAL
			ghost_portal = false;
			endPortal.active = true;
			endPortal.SetAnimation("opening", 0, 0.3f);
			anim_timer = 0;

			state = PLACING_ANIM;
			std::cout << "PORTAL ABILITY: Placing other Portal" << std::endl;
		}
		else
		{
			double mousePosX, mousePosY;
			CursorToWorldPosition(mousePosX, mousePosY);
			endPortal.pos.Set(mousePosX, mousePosY, 0);
		}
	}
		break;
	case PLACING_ANIM:
		// OPENING END PORTAL
		if (anim_timer >= 0.3)
		{
			// MOVE PLAYER FROM ONE PLACE TO OTHER
			endPortal.SetAnimation("idle", -1, 1.0f);
			anim_timer = 0;

			playerActiveState = false;
			ghost_player = true;

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
		if ((endPortal.pos - newPlayerPos).Length() < offset)
		{
			// START CLOSING START PORTAL
			newPlayerPos = endPortal.pos;
			//camera->SetMode(Camera::EDGE);
			startPortal.SetAnimation("closing", 0, 0.3f);
			anim_timer = 0;
			playerActiveState = true;
			ghost_player = false;
			state = CLOSINGSTART_ANIM;
			std::cout << "PORTAL ABILITY: Teleportation End." << std::endl;
			break;
		}

		newPlayerPos += (endPortal.pos - newPlayerPos).Normalized() * 150.0f * dt;
	}
		break;
	case CLOSINGSTART_ANIM:
		// CLOSING START PORTAL
		if (anim_timer >= 0.3)
		{ 
			// RESET START PORTAL
			// START CLOSING END PORTAL
			startPortal.active = false;
			startPortal.SetAnimation("idle", -1, 1.0f);
			endPortal.SetAnimation("closing", 0, 0.3f);
			anim_timer = 0;

			state = CLOSINGEND_ANIM;
		}
		else
		{
			anim_timer += dt;
		}
		break;
	case CLOSINGEND_ANIM:
		// CLOSING END PORTAL
		if (anim_timer >= 0.3)
		{
			// RESET END PORTAL
			endPortal.active = false;
			endPortal.SetAnimation("idle", -1, 1.0f);

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

void PortalAbility::CustomUpdate(bool playeronGround, Vector3 playerPos)
{
	conditionsMet = playeronGround;
	newPlayerPos = playerPos;
}

void PortalAbility::CustomUpdate(Vector3& playerPos, bool& playerInvisibility)
{
	playerPos = newPlayerPos;
	playerInvisibility = !playerActiveState;
}

void PortalAbility::Render()
{
	if (startPortal.active)
	{
		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(startPortal.pos.x, startPortal.pos.y, startPortal.pos.z + 0.001);
		scene->modelStack.Scale(startPortal.scale.x, startPortal.scale.y, startPortal.scale.z);
		scene->RenderMesh(startPortal.mesh, false);
		scene->modelStack.PopMatrix();
	}
	if (endPortal.active)
	{
		endPortal.mesh->material.kDiffuse.Set(1, 1, 1);
		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(endPortal.pos.x, endPortal.pos.y, endPortal.pos.z);
		scene->modelStack.Scale(endPortal.scale.x, endPortal.scale.y, endPortal.scale.z);
		scene->RenderMesh(endPortal.mesh, false);
		scene->modelStack.PopMatrix();
	}
	if (ghost_portal)
	{
		endPortal.mesh->material.kDiffuse.Set(0.1, 0.1, 0.1);
		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(endPortal.pos.x, endPortal.pos.y, endPortal.pos.z);
		scene->modelStack.Scale(endPortal.scale.x, endPortal.scale.y, endPortal.scale.z);
		scene->RenderMesh(endPortal.mesh, true);
		scene->modelStack.PopMatrix();
	}
	if (ghost_player)
	{
		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(newPlayerPos.x, newPlayerPos.y, newPlayerPos.z);
		scene->modelStack.Scale(5, 5, 5);
		scene->RenderMesh(portalSprite, true);
		scene->modelStack.PopMatrix();
	}
	
}

ABILITY_TYPE PortalAbility::GetAbilityType()
{
	return type;
}


void PortalAbility::CursorToWorldPosition(double& theX, double& theY)
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
