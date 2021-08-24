#include "UIManager.h"

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
	}
}

ButtonManager* UIManager::GetButtonManager(UI_TYPE type)
{
	return bm_array[type];
}

void UIManager::Update(double dt)
{
	//not really sure what goes in here
}
