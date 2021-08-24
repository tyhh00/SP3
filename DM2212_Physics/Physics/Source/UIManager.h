#pragma once

#include "Buttons/ButtonManager.h"
#include "SingletonTemplate.h"

enum UI_TYPE
{
	UNIVERSAL_GAMEPLAY_STATS,
	UI_COUNT,
};

class UIManager : public CSingletonTemplate<UIManager>
{
	friend CSingletonTemplate<UIManager>;

public:
	ButtonManager* GetButtonManager(UI_TYPE);
	void Init();
	void Update(double dt);

protected:
	UIManager();
	~UIManager();

	ButtonManager* bm_array[UI_COUNT];
};

