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

	void SetActive(UI_TYPE type, bool active);
	bool IsActive(UI_TYPE type);
	void SetAllActive(bool active);

	void Init();
	void Update(double dt);
	void Render(SceneBase* scene);

protected:
	UIManager();
	~UIManager();

	ButtonManager* bm_array[UI_COUNT];
	bool active_array[UI_COUNT];
};

