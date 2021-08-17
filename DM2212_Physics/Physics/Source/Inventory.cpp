#include "Inventory.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

void Inventory::Init()
{
	
}

void Inventory::Update(double dt)
{
	
}

void Inventory::CycleItem()
{
	currentItem = 
}

void Inventory::AddItem(Item* newItem)
{
	itemVector.push_back(newItem);
	currentItem = newItem;
}
