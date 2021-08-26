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

	if (meshGenerated.size() < 1)
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
