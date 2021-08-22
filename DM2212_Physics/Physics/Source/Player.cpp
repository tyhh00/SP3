
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
	, speed(1000.f)
	, jump_force(4000.f)
	, max_stamina(100.f)
	, stamina(100.f)
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
}

void Player::Init(GameObjectManager* GOM, Inventory* inventory)
{
	physics->SetMass(1);
	physics->SetMovable(true);

	portalSprite = MeshBuilder::GenerateQuad("portal travel sprites", Color(1, 1, 1), 0.5f);
	portalSprite->textureID = LoadTGA("Image/PortalTravelSprite.tga");

	input = Input::GetInstance();

	for (int i = 0; i < 2; ++i)
	{
		abilityArray[i] = nullptr;
	}

	accel = 0;

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(4, 3, 2.0f, 2.0f);
	animatedSprites->AddAnimation("idle", 0, 1);
	animatedSprites->AddAnimation("right", 6, 8);
	animatedSprites->AddAnimation("left", 3, 5);
	animatedSprites->PlayAnimation("idle", -1, 1.0f);
	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/girlsprite.tga");

	goManager = GOM;
	this->inventory = inventory;

}

void Player::Update(double dt)
{ 
	animatedSprites->Update(dt);

	//MOVEMENT SECTION
	speed_multiplier = 1.0f;
	stamina_rate_multiplier = 0.0f;

	if (input->IsKeyDown(VK_SHIFT) && stamina > 0)
	{
		speed_multiplier = 2.0f;
		stamina_rate_multiplier = 1.0f;
	}

	if (input->IsKeyDown('A'))
	{
		accel = -ACCEL_SPEED * speed_multiplier * dt;
		stamina -= stamina_rate_multiplier * 50.f * dt;
	}
	else if (input->IsKeyDown('D'))
	{
		accel = ACCEL_SPEED * speed_multiplier * dt;
		stamina -= stamina_rate_multiplier * 50.f * dt;
	}
	else
	{
		//if A or D was applied then reset vel x back to 0
		if (physics->GetVelocity().x >= -1 && physics->GetVelocity().x <= 1)
		{
			accel = 0;
			physics->SetVelocity(Vector3(0, physics->GetVelocity().y, physics->GetVelocity().z));
			std::cout << "Set vel to 0" << std::endl;
		}
	}

	//clamp the accel of the player
	accel = Math::Clamp(accel, -MAX_ACCEL, MAX_ACCEL);

	//keep the player's accel within bounds
	float tempAccel = accel;
	if (physics->GetVelocity().x > MAX_ACCEL_VEL && accel > 0)
		tempAccel = 0;
	else if (physics->GetVelocity().x < -MAX_ACCEL_VEL && accel < 0)
		tempAccel = 0;

	physics->AddVelocity(Vector3(tempAccel, 0, 0));

	if (stamina < max_stamina)
	{
		stamina += 5.f * dt;
	}


	// JUMP SECTION
	if (input->IsKeyPressed(VK_SPACE)
		&& physics->GetOnGround())
	{
		std::cout << "Space Key Pressed" << std::endl;
		float accel_amt = jump_force / physics->GetMass();
		physics->AddVelocity(Vector3(0, physics->GetVelocity().y + accel_amt * dt, 0));
	}

	// ANIMATIONS SECTION
	if (accel >= 1 || physics->GetVelocity().x > 1)
		animatedSprites->PlayAnimation("right", -1, 1.0f);
	else if (accel <= -1 || physics->GetVelocity().x < -1)
		animatedSprites->PlayAnimation("left", -1, 1.0f);
	else
		animatedSprites->PlayAnimation("idle", -1, 1.0f);


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
				ability->UpdatePlayer(accel, physics, speed, enableCollision, isDashing);
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

	//set player's max vel speed
	if (physics->GetVelocity().Length() > MAX_VEL)
		physics->SetVelocity(physics->GetVelocity().Normalized() * MAX_VEL);
}

void Player::Render(SceneBase* scene)
{
	for (int i = 0; i < 2; i++)
	{
		if (abilityArray[i] != nullptr)
		{
			abilityArray[i]->Render();
		}
	}

	float angle = Math::RadianToDegree(atan2(physics->GetNormal().y, physics->GetNormal().x));
	scene->modelStack.PushMatrix();
	scene->modelStack.Translate(pos.x, pos.y, pos.z);
	scene->modelStack.Rotate(angle + physics->GetRotateZ(), 0, 0, 1);
	scene->modelStack.Scale(scale.x, scale.y, scale.z);
	scene->RenderMesh(mesh, true);
	scene->modelStack.PopMatrix();

	// Render Stamina Bar??
	
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


