#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Apple.h"

Apple::Apple(int stemLength)
	: stemLength(stemLength)
{
	isStackable = true;
	quantity = 1;
	maxQuantity = 5;
	itemType = I_APPLE;
}

void Apple::Init()
{
}

void Apple::Update(double dt)
{
	if (Application::IsKeyPressed('G'))
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
