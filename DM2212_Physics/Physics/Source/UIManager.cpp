#include "UIManager.h"
#include "Debug.h"
#include "MeshBuilder.h"
#include "Buttons/ButtonFactory.h"
#include "LoadTGA.h"
#include "GameManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
	for (int i = 0; i < UI_COUNT; ++i)
	{
		delete bm_array[i];
	}
	for (auto& mesh : meshGenerated)
	{
		delete mesh;
		mesh = nullptr;
	}
	meshGenerated.clear();
}

void UIManager::Init()
{
	for (int i = 0; i < UI_COUNT; ++i)
	{
		bm_array[i] = new ButtonManager();
		active_array[i] = false;
	}

	//Init BMs
	ButtonManager* bm_gameplayStat = bm_array[UI_TYPE::UNIVERSAL_GAMEPLAY_STATS];
	bm_gameplayStat->SetZPriority(10);
	if (meshGenerated.size() < 1) //First init()
	{
		//HealthBar
		Mesh* playerHealth = MeshBuilder::GenerateQuad("health", Color(0.8, 0.2, 0.2), 1.0f);
		meshGenerated.push_back(playerHealth);

		Mesh* healthIcon = MeshBuilder::GenerateQuad("health", Color(0.9, 0.2, 0.2), 1.0f);
		healthIcon->textureID = LoadTGA("Image/lives.tga");
		meshGenerated.push_back(healthIcon);

		Mesh* coinIcon = MeshBuilder::GenerateQuad("coin", Color(0.9, 0.2, 0.2), 1.0f);
		coinIcon->textureID = LoadTGA("Image/tiles/coin_tile.tga");
		meshGenerated.push_back(coinIcon);

		//Health Bar
		bm_gameplayStat->addButton(ButtonFactory::createProgressBar("playerhealth", 7, 59, 1, 7, HORIZONTAL, playerHealth));
		//Health Icon
		bm_gameplayStat->addButton(ButtonFactory::createNoTextButton("healthicon", 2.2, 59, 1.2, 1.2, healthIcon));

		//Coin Icon
		bm_gameplayStat->addButton(ButtonFactory::createNoTextButton("coinicon", 2.2, 57, 1.2, 1.2, coinIcon));
		bm_gameplayStat->addButton(ButtonFactory::createTextButton("coinvalue", 4.6, 55.1, 2, 2, 0, 0, Color(0.9, 0.9, 0.9), std::to_string(GameManager::GetInstance()->getCoins()), 1.4, SUPERMARIO));


		//Ability Backgrounds
		Mesh* ability_bg = MeshBuilder::GenerateQuad("ability_bg", Color(1, 1, 1), 5.0f);
		ability_bg->textureID = LoadTGA("Image//ability_border.tga");
		meshGenerated.push_back(ability_bg);

		Mesh* ability_bgCooldown = MeshBuilder::GenerateQuad("ability_bg_cooldown", Color(1, 1, 1), 5.0f);
		ability_bgCooldown->textureID = LoadTGA("Image//ability_border_cooldown.tga");
		meshGenerated.push_back(ability_bgCooldown);

		bm_gameplayStat->addButton(ButtonFactory::createNoTextButton("ability_1_icon", 5, 7, 1, 1, nullptr));
		Button* ready = ButtonFactory::createNoTextButton("ability_1_ready", 5, 7, 1, 1, ability_bg);
		bm_gameplayStat->addButton(ready);
		Button* onCD = ButtonFactory::createButton("ability_1_bg", 5, 7, 1, 1, ability_bgCooldown, -0.8, 1.3, Color(0.9f, 0.9f, 0.9f), "0.00s", 1.7);
		onCD->disable();
		bm_gameplayStat->addButton(onCD);
		bm_gameplayStat->addButton(ButtonFactory::createTextButton("ability_1_key", 7, 3, 2, 2, 0, 0, Color(0.7, 0.7, 0.55), "Q", 1.4, SUPERMARIO));
		
		bm_gameplayStat->addButton(ButtonFactory::createNoTextButton("ability_2_icon", 11, 7, 1, 1, nullptr));
		Button* ready2 = ButtonFactory::createNoTextButton("ability_2_ready", 11, 7, 1, 1, ability_bg);
		bm_gameplayStat->addButton(ready2);
		//ready2->disable();
		Button* onCD2 = ButtonFactory::createButton("ability_2_bg", 11, 7, 1, 1, ability_bgCooldown, -0.8, 1.3, Color(0.9f, 0.9f, 0.9f), "0.00s", 1.7);
		onCD2->disable();
		bm_gameplayStat->addButton(onCD2);
		bm_gameplayStat->addButton(ButtonFactory::createTextButton("ability_2_key", 13, 3, 2, 2, 0, 0, Color(0.7, 0.7, 0.55), "Z", 1.4, SUPERMARIO));
		

		//STAMINA BAR
		Mesh* playerStaminaBar = MeshBuilder::GenerateQuad("staminabar", Color(0.9, 0.87, 0.0), 1.0f);
		meshGenerated.push_back(playerStaminaBar);

		Mesh* staminaIcon = MeshBuilder::GenerateQuad("stamina", Color(0.9, 0.2, 0.2), 1.0f);
		staminaIcon->textureID = LoadTGA("Image/runner.tga");
		meshGenerated.push_back(staminaIcon);

		//StaminaBar
		bm_gameplayStat->addButton(ButtonFactory::createProgressBar("staminabar", 8.4, 13, 1.3, 7, HORIZONTAL, playerStaminaBar));
		//StaminaIcon
		bm_gameplayStat->addButton(ButtonFactory::createNoTextButton("staminaicon", 3.7, 13, 2, 2, staminaIcon));


	}
}



ButtonManager* UIManager::GetButtonManager(UI_TYPE type)
{
	return bm_array[type];
}

void UIManager::Update(double dt)
{
	for (int i = 0; i < UI_COUNT; ++i)
	{
		if (active_array[i])
			bm_array[i]->Update(dt);
	}
}

void UIManager::Render(SceneBase* scene)
{
	for (int i = 0; i < UI_COUNT; ++i)
	{
		if (active_array[i])
		{
			bm_array[i]->Render(scene);
		}
	}
}

void UIManager::SetActive(UI_TYPE type, bool active)
{
	active_array[type] = active;
}

void UIManager::SetAllActive(bool active)
{
	for (int i = 0; i < UI_COUNT; ++i)
	{
		active_array[i] = active;
	}
}

bool UIManager::IsActive(UI_TYPE type)
{
	return active_array[type];
}
