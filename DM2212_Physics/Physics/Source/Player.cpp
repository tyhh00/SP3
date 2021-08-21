
#include "Player.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"
#include "Apple.h"

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

	// MOVEMENT SECTION
	/*if (AkeyDown && !Application::IsKeyPressed('A'))
	{
		AkeyDown = false;
		std::cout << "A Key Released" << std::endl;
		if (animatedSprites->GetCurrentAnimation() == "left")
		{
			animatedSprites->PlayAnimation("idle", -1, 1.0f);
		}
		physics->SetVelocity(Vector3(physics->GetVelocity().x + speed * dt, 0, 0));
	}
	else if (!AkeyDown && Application::IsKeyPressed('A'))
	{
		AkeyDown = true;
		std::cout << "A Key Pressed" << std::endl;
		animatedSprites->PlayAnimation("left", -1, 1.0f);
		physics->SetVelocity(Vector3(physics->GetVelocity().x - speed * dt, 0, 0));
	}
	if (DkeyDown && !Application::IsKeyPressed('D'))
	{
		DkeyDown = false;
		std::cout << "D Key Released" << std::endl;
		if (animatedSprites->GetCurrentAnimation() == "right")
		{
			animatedSprites->PlayAnimation("idle", -1, 1.0f);
		}
		physics->SetVelocity(Vector3(physics->GetVelocity().x - speed * dt, 0, 0));
	}
	else if (!DkeyDown && Application::IsKeyPressed('D'))
	{
		DkeyDown = true;
		std::cout << "D Key Pressed" << std::endl;
		animatedSprites->PlayAnimation("right", -1, 1.0f);
		physics->SetVelocity(Vector3(physics->GetVelocity().x + speed * dt, 0, 0));
	}*/
	speed_multiplier = 1.0f;
	stamina_rate_multiplier = 0.0f;
	if (input->IsKeyDown(VK_SHIFT) && stamina > 0)
	{
		speed_multiplier = 2.0f;
		stamina_rate_multiplier = 1.0f;
	}

	if (input->IsKeyDown('A'))
	{
		physics->SetVelocity(Vector3(-speed * speed_multiplier * dt, physics->GetVelocity().y, physics->GetVelocity().z));
		stamina -= stamina_rate_multiplier * 50.f * dt;
	}
	else if (input->IsKeyDown('D'))
	{
		physics->SetVelocity(Vector3(speed * speed_multiplier * dt, physics->GetVelocity().y, physics->GetVelocity().z));
		stamina -= stamina_rate_multiplier * 50.f * dt;
	}
	else if (input->IsKeyDown('W'))
	{
		physics->SetVelocity(Vector3(physics->GetVelocity().x, speed * speed_multiplier * dt, physics->GetVelocity().z));
		stamina -= stamina_rate_multiplier * 50.f * dt;
	}
	else if (input->IsKeyDown('S'))
	{
		physics->SetVelocity(Vector3(physics->GetVelocity().x, -speed * speed_multiplier * dt, physics->GetVelocity().z));
		stamina -= stamina_rate_multiplier * 50.f * dt;
	}
	else
	{
		physics->SetVelocity(Vector3(0, physics->GetVelocity().y, physics->GetVelocity().z));
	}

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
	if (physics->GetVelocity().x > 1)
		animatedSprites->PlayAnimation("right", -1, 1.0f);
	else if (physics->GetVelocity().x < -1)
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
				abilityArray[i]->Update(dt);
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
			default:
				break;
			}
		}
	}

	if (input->IsKeyPressed('Q'))
	{
		if (!this->physics->GetVelocity().IsZero() && !isDashing)
			isDashing = true;
	}

	//if in dashing phase
	if (isDashing)
	{
		//disable physics gravity update
		this->physics->SetEnableUpdate(false);

		//disable collision
		this->enableCollision = false;

		//start dash timer
		dashTimer += dt;

		//check if dash time is over
		if (dashTimer > 0.25f)
		{
			//reset dash timer, is dashing and physics update
			dashTimer = 0;
			isDashing = false;
			this->physics->SetEnableUpdate(true);
			this->enableCollision = true;
		}

		//double check if player vel is not zero if not will have divide by zero error in normalized()
		if (!this->physics->GetVelocity().IsZero())
		{
			//get and normalize the player vel  and find out the dash dir
			Vector3 dir = this->physics->GetVelocity().Normalized() * speed * speed * dt;

			//add the dash dir to the player's vel
			this->physics->AddVelocity(Vector3(dir.x, 0, 0));
		}
	}

	//set player's max vel speed
	if (this->physics->GetVelocity().Length() > 100)
		this->physics->SetVelocity(this->physics->GetVelocity().Normalized() * 100);
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


