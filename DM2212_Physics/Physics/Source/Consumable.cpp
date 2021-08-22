#include "Consumable.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Consumable::Consumable(ITEM_TYPE type) : Item(G_CONSUMABLE, type, 1, true)
{
}

void Consumable::Init()
{
	
}

void Consumable::Update(double dt)
{

}
