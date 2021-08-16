#pragma once
#include <vector>
#include "GameObject.h"

class GameObjectManager
{

protected:
	std::vector<GameObject*> environmentals; //Tiles and static items
	std::vector<GameObject*> entities; //Player, Enemies, Items on Ground
};

