#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Cheese.h"

Cheese::Cheese()
{
}

void Cheese::Init()
{
}

void Cheese::Update(double dt)
{
	std::cout << "updating cheese" << std::endl;
	if (Application::IsKeyPressed('G'))
	{
		std::cout << "consumed a cheese" << std::endl;
	}
}
