#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Apple.h"

Apple::Apple(int _quantity, int stemLength) : Consumable(I_APPLE)
	,stemLength(stemLength)
{
	isStackable = true;
	quantity = _quantity;

	input = Input::GetInstance();

	//mesh = SceneBase::GEOMETRY_TYPE::GEO_UI_APPLE;
}

void Apple::Init()
{
}

void Apple::Update(double dt)
{
	if (input->IsKeyPressed('G'))
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
