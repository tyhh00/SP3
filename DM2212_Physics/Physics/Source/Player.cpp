
#include "Player.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"
#include "Battery.h"
#include "Apple.h"
#include "FireTorch.h"

Player::Player() : input(NULL)
, goManager(NULL)
, inventory(NULL)
, accel(200.f)
, jump_force(20000.f)
, max_stamina(100.f)
, stamina(100.f)
, curr_max_vel(MAX_VEL)
, lives(3)
, max_lives(3)
, stamina_rate_multiplier(0.0f)
, invisibility(false)
{
}

Player::~Player()
{
	for (int i = 0; i < 2; i++)
	{
		if (abilityArray[i] != nullptr)
		{
			delete abilityArray[i];
			abilityArray[i] = nullptr;
		}
	}

	if (portalSprite)
	{
		delete portalSprite;
		portalSprite = NULL;
	}
	if (animatedSprites)
	{
		delete animatedSprites;
		animatedSprites = NULL;
	}
	if (livesIcon)
	{
		delete livesIcon;
		livesIcon = NULL;
	}
}

void Player::Init(MOVEMENT_MODE mode, GameObjectManager* GOM, Inventory* inventory)
{
	physics->SetMass(5);
	physics->SetMovable(true);

	if (mode == WASD)
	{
		physics->SetGravity(Vector3(0, 0, 0));
	}
	portalSprite = MeshBuilder::GenerateQuad("portal travel sprites", Color(1, 1, 1), 1.0f);
	portalSprite->textureID = LoadTGA("Image/PortalTravelSprite.tga");
	livesIcon = MeshBuilder::GenerateQuad("hp icon", Color(1, 1, 1), 1.0f);
	livesIcon->textureID = LoadTGA("Image/lives.tga");

	input = Input::GetInstance();

	for (int i = 0; i < 2; ++i)
	{
		abilityArray[i] = nullptr;
	}

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(4, 3, 2.0f, 2.0f);
	animatedSprites->AddAnimation("idle", 0, 2);
	animatedSprites->AddAnimation("right", 6, 8);
	animatedSprites->AddAnimation("left", 3, 5);
	animatedSprites->AddAnimation("up", 9, 11);

	animatedSprites->PlayAnimation("idle", -1, 1.0f);
	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/girlsprite.tga");

	goManager = GOM;
	this->inventory = inventory;
	this->mode = mode;

}

void Player::Update(double dt)
{ 
	animatedSprites->Update(dt);

	// MOVEMENT SECTION
	UpdateMovement(dt);

	// ABILITIES SECTION
	for (int i = 0; i < 2; i++)
	{
		if (abilityArray[i] != nullptr)
		{
			switch (abilityArray[i]->GetAbilityType())
			{
			case ABILITY_DASH:
			{
				abilityArray[i]->Update(dt);
				DashAbility* ability = dynamic_cast<DashAbility*>(abilityArray[i]);
				//ability->UpdatePlayer(accel, physics, speed, enableCollision, isDashing);
			}
			break;
			case ABILITY_PORTAL:
			{
				PortalAbility* ability = dynamic_cast<PortalAbility*>(abilityArray[i]);
				ability->CustomUpdate(physics->GetOnGround(), pos);
				ability->Update(dt);
				ability->CustomUpdate(pos, invisibility);
				if (invisibility)
				{
					mesh = portalSprite;
					physics->SetEnableUpdate(false);
					enableCollision = false;
				}
				else
				{
					mesh = animatedSprites;
					physics->SetEnableUpdate(true);
					enableCollision = true;
				}
			}
			break;
			case ABILITY_GRAPPLER:
			{
				abilityArray[i]->Update(dt);
				GrapplingAbility* ability = dynamic_cast<GrapplingAbility*>(abilityArray[i]);
				ability->UpdatePlayer(pos, physics);
			}
			break;
			default:
				break;
			}
		}
	}

	physics->SetVelocity(Vector3(Math::Clamp(physics->GetVelocity().x, -curr_max_vel, curr_max_vel), physics->GetVelocity().y, physics->GetVelocity().z));
}

void Player::UpdateMovement(double dt)
{		

	if (mode == WASD)
	{
		float speed = 20.f;
		if (input->IsKeyDown('A'))
		{
			physics->SetVelocity(Vector3(-speed, 0, 0));
			animatedSprites->PlayAnimation("left", -1, 1.0f);
		}
		else if (input->IsKeyDown('D'))
		{
			physics->SetVelocity(Vector3(speed, 0, 0));
			animatedSprites->PlayAnimation("right", -1, 1.0f);
		}
		else if (input->IsKeyDown('W'))
		{
			physics->SetVelocity(Vector3(0, speed, 0));
			animatedSprites->PlayAnimation("up", -1, 1.0f);
		}
		else if (input->IsKeyDown('S'))
		{
			physics->SetVelocity(Vector3(0, -speed, 0));
			animatedSprites->PlayAnimation("idle", -1, 1.0f);
		}
		else {
			physics->SetVelocity(Vector3(0, 0, 0));
		}

	}
	else if (mode == PLATFORMER)
	{
		Vector3 leftAccel(-accel * speed_multiplier, 0, 0);
		Vector3 rightAccel(accel * speed_multiplier, 0, 0);

		speed_multiplier = 1.0f;
		stamina_rate_multiplier = 0.0f;
		if (input->IsKeyDown(VK_SHIFT) && stamina > 0)
		{
			curr_max_vel = MAX_SPRINTVEL;
			speed_multiplier = 2.0f;
			stamina_rate_multiplier = 1.0f;
		}

		if (input->IsKeyDown('A'))
		{
			physics->AddVelocity(leftAccel * dt);
			stamina -= stamina_rate_multiplier * 50.f * dt;
		}
		if (input->IsKeyDown('D'))
		{
			physics->AddVelocity(rightAccel * dt);
			stamina -= stamina_rate_multiplier * 50.f * dt;
		}
		// JUMP SECTION
		if (input->IsKeyPressed(VK_SPACE)
			&& physics->GetOnGround())
		{
			std::cout << "Space Key Pressed" << std::endl;
			float accel_amt = jump_force / physics->GetMass();
			physics->AddVelocity(Vector3(0, physics->GetVelocity().y + accel_amt * dt, 0));
		}

		if (stamina < max_stamina)
		{
			stamina += 5.f * dt;
		}

		// ANIMATIONS SECTION
		if (physics->GetVelocity().x > 1)
			animatedSprites->PlayAnimation("right", -1, 1.0f);
		else if (physics->GetVelocity().x < -1)
			animatedSprites->PlayAnimation("left", -1, 1.0f);
		else
			animatedSprites->PlayAnimation("idle", -1, 1.0f);
	}
}

void Player::Render(SceneBase* scene)
{
	// ability related renders
	for (int i = 0; i < 2; i++)
	{
		if (abilityArray[i] != nullptr)
		{
			abilityArray[i]->Render();
		}
	}

	// player sprite
	float angle = Math::RadianToDegree(atan2(physics->GetNormal().y, physics->GetNormal().x));
	scene->modelStack.PushMatrix();
	scene->modelStack.Translate(pos.x, pos.y, pos.z);
	scene->modelStack.Rotate(angle + physics->GetRotateZ(), 0, 0, 1);
	scene->modelStack.Scale(scale.x, scale.y, scale.z);
	scene->RenderMesh(mesh, true);
	scene->modelStack.PopMatrix();

	// Render Stamina Bar??

	// hp
	float HPscale = 3;
	for (int i = 0; i < lives; i++)
	{
		scene->modelStack.PushMatrix();
		scene->RenderMeshOnScreen(livesIcon, HPscale * 0.5, 60 - HPscale * 0.5, HPscale, HPscale);
		scene->modelStack.PopMatrix();
	}
	
}

void Player::CollidedWith(GameObject* go)
{
	switch (go->geoTypeID)
	{
	case SceneBase::GEO_FLASHLIGHT:
		goManager->RemoveGO(go);
		inventory->AddItem(new Flashlight);
		break;
	case SceneBase::GEO_BATTERY:
		goManager->RemoveGO(go);
		inventory->AddItem(new Battery);
		break;
	case SceneBase::GEO_BONES_02:
		goManager->RemoveGO(go);
	//	inventory->AddItem(new Battery);
		break;
	case SceneBase::GEO_BONES_03:
		goManager->RemoveGO(go);
		//	inventory->AddItem(new Battery);
		break;
	case SceneBase::GEO_BONES_10:
		goManager->RemoveGO(go);
		//	inventory->AddItem(new Battery);
		break;
	case SceneBase::GEO_BONES_11:
		goManager->RemoveGO(go);
		//	inventory->AddItem(new Battery);
		break;
	case SceneBase::GEO_JUNGLE_GRASS_BLOCK:
		goManager->RemoveGO(go);
		inventory->AddItem(new FireTorch);
		break;
	case SceneBase::GEO_JUNGLE_DIRT_BLOCK:
		if (inventory->GetCurrentItem() == nullptr)
			break;
		if (inventory->GetCurrentItemType() == Item::I_FIRETORCH)
			currentHP++;
		break;
	case SceneBase::GEO_LOBBY_PORTAL_GRAVEYARD:
		std::cout << "AAAAAAAAAAA" << std::endl;
		break;
	default:
		break;
	}
}

void Player::SetAbilities(Ability* a, Ability* b)
{
	abilityArray[0] = a;
	abilityArray[1] = b;
}

float Player::GetStamina()
{
	return stamina;
}

void Player::DecreaseStamina(float amt)
{
	stamina -= amt;
}


