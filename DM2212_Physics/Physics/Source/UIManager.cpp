#include "UIManager.h"
#include "Debug.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
	for (int i = 0; i < UI_COUNT; ++i)
	{
		delete bm_array[i];
	}
}

void UIManager::Init()
{
	for (int i = 0; i < UI_COUNT; ++i)
	{
		bm_array[i] = new ButtonManager();
		active_array[i] = false;
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
