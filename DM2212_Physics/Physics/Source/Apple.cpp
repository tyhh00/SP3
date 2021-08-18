#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Apple.h"

Apple::Apple(int _quantity, int stemLength)
	: stemLength(stemLength)
{
	isStackable = true;
	quantity = _quantity;
	itemType = I_APPLE;

	keyboard = Keyboard::GetInstance();
}

void Apple::Init()
{
}

void Apple::Update(double dt)
{
	if (keyboard->IsKeyPressed('G'))
	{
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
