/**
 Inventory Class
 By: Jordan Low
 Date: 17 Aug 2021
 */
#include "Inventory.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

/**
 @brief Init Inventory
 */
void Inventory::Init()
{
	
}

/**
 @brief Update Inventory
 @param dt A double for delta time
 */
void Inventory::Update(double dt)
{
	if (currentItem)
		currentItem->Update(dt);
}

/**
 @brief Cycle through current item with item vector
 */
void Inventory::CycleItem()
{
	int currentItemIndex = -1;
	
	//search for the current item's index in the vector
	auto it = std::find(itemVector.begin(), itemVector.end(), currentItem);
	if (it != itemVector.end())
		currentItemIndex = std::distance(itemVector.begin(), it);

	//if successfully found curr item index
	if (currentItemIndex != -1)
	{
		//if curr item index is the last element in vector
		if (currentItemIndex == itemVector.size() - 1)
			currentItemIndex = -1;
		currentItem = itemVector[currentItemIndex + 1];
	}
}

/**
 @brief Add new item to inventory
 @param newItem An Item* which gets added into the inventory
 */
void Inventory::AddItem(Item* newItem)
{
	itemVector.push_back(newItem);
	currentItem = newItem;
}
