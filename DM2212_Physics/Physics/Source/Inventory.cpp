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
	keyboard = Keyboard::GetInstance();

	for (int i = 0; i < sizeof(maxQuantity) / sizeof(maxQuantity[0]); i++)
	{
		maxQuantity[i] = 5;
	}
}

/**
 @brief Update Inventory
 @param dt A double for delta time
 */
void Inventory::Update(double dt)
{
	if (currentItem)
		currentItem->Update(dt);


	if (keyboard->IsKeyPressed(VK_LEFT))
	{
		std::cout << "cycle backward" << std::endl;
		CycleItem(false);
		std::cout << "qty: " << currentItem->GetQuantity() << std::endl;
	}
	if (keyboard->IsKeyPressed(VK_RIGHT))
	{
		std::cout << "cycle forward" << std::endl;
		CycleItem(true);
		std::cout << "qty: " << currentItem->GetQuantity() << std::endl;
	}
	if (Application::IsKeyReleased('5'))
	{
		std::cout << "1" << std::endl;
		SwitchItem(0);
	}
	if (Application::IsKeyReleased('6'))
	{
		std::cout << "2" << std::endl;
		SwitchItem(1);
	}
}

/**
 @brief Cycle through current item with item vector
 */
void Inventory::CycleItem(bool forward)
{
	int currentItemIndex = -1;
	
	//search for the current item's index in the vector
	auto it = std::find(itemVector.begin(), itemVector.end(), currentItem);
	if (it != itemVector.end())
		currentItemIndex = std::distance(itemVector.begin(), it);

	if (forward) //cycle forward
	{
		//if successfully found curr item index
		if (currentItemIndex != -1)
		{
			//if curr item index is the last element in vector
			if (currentItemIndex == itemVector.size() - 1)
				currentItemIndex = -1;
			currentItem = itemVector[currentItemIndex + 1];
			std::cout << "curr: " << currentItem->GetType() << std::endl;
		}
	}
	else //cycle backwards
	{
		//if successfully found curr item index
		if (currentItemIndex != -1)
		{
			//if curr item index is the last element in vector
			if (currentItemIndex == 0)
				currentItemIndex = itemVector.size();
			currentItem = itemVector[currentItemIndex - 1];
			std::cout << "curr: " << currentItem->GetType() << std::endl;
		}
	}
}

/**
 @brief Switch between inventory item using index for the vector
 @param index An Int for the index of the vector
 */
void Inventory::SwitchItem(int index)
{
	if (index >= itemVector.size())
		return;
	currentItem = itemVector[index];
}

/**
 @brief Add new item to inventory
 @param newItem An Item* which gets added into the inventory
 */
void Inventory::AddItem(Item* newItem)
{
	if (itemVector.size() <= 0)
	{
		itemVector.push_back(newItem);
		currentItem = newItem;
		std::cout << currentItem->GetType() << std::endl;
		return;
	}

	bool itemExist = false;
	bool isStackable = false;

	for (Item* item : itemVector)
	{
		if (newItem->GetType() == item->GetType())
		{
			if (newItem->GetIsStackable() && item->IsEqual(newItem))
			{
				std::cout << "adding new qty to item: " << item->GetType() << std::endl;
				AddQuantity(item, newItem->GetQuantity());
				return;
			}
		}
	}

	std::cout << "adding new item to vector" << std::endl;
	itemVector.push_back(newItem);
	currentItem = newItem;
}

/**
 @brief Add quantity to an existing item
 @param item An Item* which is for the quantity to be added to the item
 @param _quantity An int for the number of quantity to be added to the item
 */
int Inventory::AddQuantity(Item* item, int _quantity)
{
	//new qty to be added
	int newQuantity = item->GetQuantity() + _quantity;

	//if new qty exceeds max qty, return remainder
	if (newQuantity > maxQuantity[item->GetType()])
	{
		int remainderQuantity = newQuantity - maxQuantity[item->GetType()];
		item->SetQuantity(maxQuantity[item->GetType()]);
		return remainderQuantity;
	}

	//else set qty to new qty
	item->SetQuantity(newQuantity);
	return 0;
}
