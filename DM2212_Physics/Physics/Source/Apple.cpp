#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Apple.h"
#include "../Source/SoundController/SoundController.h"
#include "GameManager.h"

Apple::Apple(Mesh* _mesh, GameObject* _player, int _quantity, int stemLength) : Consumable(I_APPLE, mesh)
	,stemLength(stemLength)
{
	isStackable = true;
	quantity = _quantity;
	

	input = Input::GetInstance();

	mesh = _mesh;
	player = _player;
}

void Apple::Init()
{
}

void Apple::Update(double dt)
{
	//if player consumes apple, add 5 hp
	if (input->IsKeyPressed(GameManager::GetInstance()->INTERACT_KEYBIND))
	{
		CSoundController::GetInstance()->PlaySoundByID(EATING);
		player->currentHP += 5;
		RemoveQuantity(1);
	}
}

bool Apple::IsEqual(Item* item1)
{
	Apple* checkApple = static_cast<Apple*>(item1);
	std::cout << "first apple has stem length: " << this->stemLength << " and second apple has stem length: " << checkApple->stemLength << std::endl;
	if (this->stemLength == checkApple->stemLength)
		return true;
	return false;
}