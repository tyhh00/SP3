#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"

Flashlight::Flashlight()
{
	isStackable = true;
	itemType = I_FLASHLIGHT;

	keyboard = Keyboard::GetInstance();
}

Flashlight::~Flashlight()
{
}

void Flashlight::Init()
{
}

void Flashlight::Update(double dt)
{
	
}

bool Flashlight::IsEqual(Item* item1)
{
	Flashlight* checkApple = static_cast<Flashlight*>(item1);
	return false;
}

