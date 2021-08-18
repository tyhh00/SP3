#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Apple.h"

Apple::Apple(int stemLength)
	: stemLength(stemLength)
{
	isStackable = false;
	quantity = 1;
	maxQuantity = 1;
}

void Apple::Init()
{
}

void Apple::Update(double dt)
{
	std::cout << "updating apple" << std::endl;
	if (Application::IsKeyPressed('G'))
	{
		std::cout << "consumed an apple" << std::endl;
		this->SetQuantity(this->GetQuantity() - 1);
	}
}

bool Apple::IsEqual(Item* item1)
{
	Apple* checkApple = static_cast<Apple*>(item1);
	if (this->stemLength == checkApple->stemLength)
		return true;
	return false;
}
