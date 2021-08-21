#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Cheese.h"

Cheese::Cheese(int _numOfHoles) : Consumable(I_CHEESE)
{
	isStackable = true;
	quantity = 1;
	numOfHoles = _numOfHoles;
	itemType = I_CHEESE;
}

void Cheese::Init()
{
}

void Cheese::Update(double dt)
{
	if (Application::IsKeyPressed('G'))
	{
		std::cout << "consumed a cheese" << std::endl;
	}
}

bool Cheese::IsEqual(Item* item1)
{
	Cheese* checkCheese = static_cast<Cheese*>(item1);
	std::cout << "first cheese has: " << this->numOfHoles << " holes and second cheese has: " << checkCheese->numOfHoles << " holes" << std::endl;
	if (this->numOfHoles == checkCheese->numOfHoles)
		return true;
	return false;
}
