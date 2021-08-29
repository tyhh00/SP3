#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Banana.h"
#include "GameManager.h"
#include "../Source/SoundController/SoundController.h"

Banana::Banana(Mesh* _mesh, GameObject* _player) : Consumable(I_BANANA, mesh)
{
	isStackable = true;
	quantity = 1;
	itemType = I_BANANA;
	mesh = _mesh;
	player = _player;
}

void Banana::Init()
{
}

void Banana::Update(double dt)
{
	//if player consumes banana, add 10 hp
	if (Application::IsKeyPressed(GameManager::GetInstance()->INTERACT_KEYBIND))
	{
		CSoundController::GetInstance()->PlaySoundByID(EATING);
		player->currentHP += 10;
		RemoveQuantity(1);
	}
}

bool Banana::IsEqual(Item* item1)
{
	return false;
}
