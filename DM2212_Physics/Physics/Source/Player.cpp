
#include "Player.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"
#include "Battery.h"
#include "Apple.h"
#include "Ghost.h"
#include "Monkey.h"
#include "FireTorch.h"
#include "Banana.h"
#include "Bone.h"
#include "Skull.h"
#include "Pickaxe.h"
#include "UIManager.h"
#include "Pistol.h"
#include "Campfire.h"
#include "Coin.h"
#include "Blueshell.h"
#include "Redshell.h"
#include "GameManager.h"
#include "../Source/SoundController/SoundController.h"


Player::Player() : input(NULL)
, goManager(NULL)
, inventory(NULL)
, accel(200.f)
, jump_force(420.f)
, max_stamina(100.f)
, stamina(100.f)
, curr_max_vel(MAX_VEL)
, stamina_rate_multiplier(0.0f)
, staminaCD(0.0)
{
	type = GO_PLAYER;
	slowedObj = false;
}

Player::~Player()
{
	//for (int i = 0; i < 2; i++)
	//{
	//	if (abilityArray[i] != nullptr)
	//	{
	//		delete abilityArray[i];
	//		abilityArray[i] = nullptr;
	//		//Set mesh to nullptr cause its already deleted in SceneBase, Player is only deleted when scenebase is deleted hence this code is valid
	//		UIManager::GetInstance()->GetButtonManager(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS)->getButtonByName("ability_" + std::to_string(i + 1) + "_icon")->setQuadImage(nullptr);
	//	}
	//}

	
	if (animatedSprites)
	{
		delete animatedSprites;
		animatedSprites = NULL;
	}
	
}

void Player::Init(Camera* _cam, MOVEMENT_MODE mode, GameObjectManager* GOM, Inventory* inventory)
{
	type = GO_PLAYER;
	physics->SetMass(5);
	physics->SetMovable(true);
	currentHP = 100;
	cam = _cam;

	if (mode == WASD)
	{
		physics->SetGravity(Vector3(0, 0, 0));
	}

	input = Input::GetInstance();
	gameManager = GameManager::GetInstance();

	//This is initialised in PlayGameState
	ProgressBar* pHealthBar = dynamic_cast<ProgressBar*>(
		UIManager::GetInstance()->GetButtonManager(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS)->getButtonByName("playerhealth")
		);
	pHealthBar->SetProgress(1.f);

	for (int i = 0; i < 2; ++i)
	{
		abilityArray[i] = nullptr;
	}

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(4, 3, 2.0f, 2.0f);
	animatedSprites->AddAnimation("idle", 1, 2);
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
			ButtonManager* bm = UIManager::GetInstance()->GetButtonManager(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS);
			Button* ready = bm->getButtonByName("ability_" + std::to_string(i+1) + "_ready");
			Button* cooldown = bm->getButtonByName("ability_" + std::to_string(i+1) + "_bg");
			Button* activatingKey = bm->getButtonByName("ability_" + std::to_string(i + 1) + "_key");
			Button* icon = bm->getButtonByName("ability_" + std::to_string(i + 1) + "_icon");

			icon->setQuadImage(abilityArray[i]->GetMeshIcon());
			
			std::string key;
			key.append(1, abilityArray[i]->GetActivatingKey());
			activatingKey->setText(key);
			if (abilityArray[i]->GetCooldownLeft() > 0)
			{
				ready->disable();
				cooldown->enable();
				float number = abilityArray[i]->GetCooldownLeft();
				std::string num_text = std::to_string(number);
				std::string rounded = num_text.substr(0, num_text.find(".") + 2);
				cooldown->setText(rounded + "s");
			}
			else {
				ready->enable();
				cooldown->disable();
			}
			//ready->disable();
			//cooldown->disable();

			abilityArray[i]->Update(dt);
		}
	}

	curr_max_vel = Math::Clamp(curr_max_vel, MAX_VEL, 100.f);
	physics->SetVelocity(Vector3(Math::Clamp(physics->GetVelocity().x, -curr_max_vel, curr_max_vel), physics->GetVelocity().y, physics->GetVelocity().z));

	if (physics->GetVelocity().Length() > 100)
		physics->SetVelocity(physics->GetVelocity().Normalized() * 100);
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
		curr_max_vel = MAX_VEL;
		if (input->IsKeyDown(VK_SHIFT) && stamina > 0)
		{
			curr_max_vel = MAX_SPRINTVEL;
			speed_multiplier = 2.0f;
			stamina_rate_multiplier = 1.0f;
			staminaCD = 0.65;
		}

		if (input->IsKeyDown('A'))
		{
			dashDir = -1;
			physics->AddVelocity(leftAccel * dt);
			stamina -= stamina_rate_multiplier * 50.f * dt;
		}
		else if (input->IsKeyDown('D'))
		{
			dashDir = 1;
			physics->AddVelocity(rightAccel * dt);
			stamina -= stamina_rate_multiplier * 50.f * dt;
		}
		else
			dashDir = 0;

		// JUMP SECTION
		if (input->IsKeyPressed(VK_SPACE)
			&& physics->GetOnGround())
		{
			std::cout << "Space Key Pressed" << std::endl;
			float accel_amt = jump_force / physics->GetMass();
			physics->AddVelocity(Vector3(0, accel_amt, 0));
		}

		staminaCD -= dt;

		if (stamina < max_stamina && staminaCD <= 0.0)
		{
			float amt = 10.f * dt * (1 + stamina * 0.1);
			if (amt > 53 * dt)
				amt = 53 * dt;
			stamina += amt;
			if (stamina > max_stamina)
				stamina = max_stamina;
		}

		// ANIMATIONS SECTION
		if (physics->GetVelocity().x > 2)
			animatedSprites->PlayAnimation("right", -1, 1.0f);
		else if (physics->GetVelocity().x < -2)
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
	/*ProgressBar stamina_bar(staminaBar, 40, 5, 15.f, 1.f);
	stamina_bar.RenderHorizontal(scene, stamina, max_stamina);*/


	//UIManager::GetInstance()->SetActive(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS, true);
	//This is initialised in UIManager
	ProgressBar* pHealthBar = dynamic_cast<ProgressBar*>(
		UIManager::GetInstance()->GetButtonManager(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS)->getButtonByName("playerhealth")
		);
	pHealthBar->SetProgress(currentHP / maxHP);

	ProgressBar* pStaminaBar = dynamic_cast<ProgressBar*>(
		UIManager::GetInstance()->GetButtonManager(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS)->getButtonByName("staminabar")
		);
	pStaminaBar->SetProgress(stamina / max_stamina);

	//// hp
	//float HPscale = 2;
	//for (int i = 0; i < lives; i++)
	//{
	//	scene->modelStack.PushMatrix();
	//	scene->RenderMeshOnScreen(livesIcon, HPscale * 0.5 + i * HPscale, 60 - HPscale * 0.5, HPscale, HPscale);
	//	scene->modelStack.PopMatrix();
	//}
	
}

void Player::CollidedWith(GameObject* go)
{
	if (go->type == GO_ENEMY)
	{
		//...
	}

	switch (go->geoTypeID)
	{
	case SceneBase::GEO_COIN:
		GameManager::GetInstance()->addCoins(dynamic_cast<Coin*>(go)->getCoinVal());

		goManager->RemoveGO(go);
		break;
	case SceneBase::GEO_FLASHLIGHT:
		goManager->RemoveGO(go);
		inventory->AddItem(new Flashlight(go->mesh));
		break;
	case SceneBase::GEO_PICKAXE:
		goManager->RemoveGO(go);
		inventory->AddItem(new Pickaxe(go->mesh));
		break;
	case SceneBase::GEO_BATTERY:
		goManager->RemoveGO(go);
		inventory->AddItem(new Battery(go->mesh, inventory));
		break;
	case SceneBase::GEO_BONES_02:
		if (inventory->GetCurrentItem() != nullptr && inventory->GetCurrentItemType() == Item::I_PICKAXE
			&& input->IsKeyPressed('F'))
		{
			goManager->RemoveGO(go);
			inventory->AddItem(new Bone(2));
		}
		break;
	case SceneBase::GEO_BONES_03:
		if (inventory->GetCurrentItem() != nullptr && inventory->GetCurrentItemType() == Item::I_PICKAXE
			&& input->IsKeyPressed('F'))
		{
			goManager->RemoveGO(go);
			inventory->AddItem(new Bone(3));
		}
		break;
	case SceneBase::GEO_BONES_10:
		if (inventory->GetCurrentItem() != nullptr && inventory->GetCurrentItemType() == Item::I_PICKAXE
			&& input->IsKeyPressed('F'))
		{
			goManager->RemoveGO(go);
			inventory->AddItem(new Skull(1));
		}
		break;
	case SceneBase::GEO_BONES_11:
		if (inventory->GetCurrentItem() != nullptr && inventory->GetCurrentItemType() == Item::I_PICKAXE
			&& input->IsKeyPressed('F'))
		{
			goManager->RemoveGO(go);
			inventory->AddItem(new Skull(1));
			inventory->AddItem(new Bone(1));
		}
		break;
	case SceneBase::GEO_JUNGLE_FIRETORCH:
		CSoundController::GetInstance()->PlaySoundByID(ITEM_PICKUP);
		goManager->RemoveGO(go);
		inventory->AddItem(new FireTorch(go->mesh));
		break;
	case SceneBase::GEO_JUNGLE_APPLE:
		CSoundController::GetInstance()->PlaySoundByID(ITEM_PICKUP);
		goManager->RemoveGO(go);
		inventory->AddItem(new Apple(go->mesh, this));
		break;
	case SceneBase::GEO_JUNGLE_PISTOL:
		CSoundController::GetInstance()->PlaySoundByID(ITEM_PICKUP);
		goManager->RemoveGO(go);
		inventory->AddItem(new Pistol(cam, goManager, pos, new LightBullet(Vector3(1.5, 0.75f, 2), this, 60), go->mesh));
		break;
	case SceneBase::GEO_JUNGLE_BANANA:
		CSoundController::GetInstance()->PlaySoundByID(ITEM_PICKUP);
		goManager->RemoveGO(go);
		inventory->AddItem(new Banana(go->mesh, this));
		break;
	case SceneBase::GEO_MACHINEPART_1:
		gameManager->setMachineStatus(1, true);
		break;
	case SceneBase::GEO_MACHINEPART_2:
		gameManager->setMachineStatus(2, true);
		break;
	case SceneBase::GEO_MACHINEPART_3:
		gameManager->setMachineStatus(3, true);
		break;
	case SceneBase::GEO_MACHINEPART_4:
		gameManager->setMachineStatus(4, true);
		break;
	case SceneBase::GEO_JUNGLE_CAMPFIRE:
	{
		Campfire* campfire = dynamic_cast<Campfire*>(go);
		if (campfire->GetIsLit())
			currentHP += 1.f; //heal player if stand beside campfire
	}
		break;
	case SceneBase::GEO_OCEAN_SEASHELL1:
		if (input->IsKeyPressed('F'))
		{
			goManager->RemoveGO(go);
			inventory->AddItem(new Blueshell(1));	
		}
		break;
	case SceneBase::GEO_OCEAN_SEASHELL2:
		if (input->IsKeyPressed('F'))
		{
			goManager->RemoveGO(go);
			inventory->AddItem(new Redshell(1));	
		}
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


