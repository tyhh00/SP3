#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Apple.h"

Apple::Apple()
{
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
	}
}
