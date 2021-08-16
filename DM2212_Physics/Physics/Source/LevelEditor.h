#pragma once
#include <vector>
#include "GameObject.h"
#include "GameObjectManager.h"

class LevelEditor
{
	friend GameObjectManager;

protected:
	GameObjectManager* goManager;
	int gridLength, gridHeight;


public:
	LevelEditor();
	~LevelEditor();

};

