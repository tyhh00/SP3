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
void Inventory::Init(SceneBase* scene)
{
	Mesh* mesh = MeshBuilder::GenerateQuad("button", Color(1, 1, 1), 2.0f);
	mesh->textureID = LoadTGA("Image/10.tga");

	buttonManager = new ButtonManager(80, 60);

	int k = 0; //y cut-off
	int count = -1;
	for (int j = 0; j < TOTAL_INVEN_SLOTS; j++)
	{
		if (j == TOTAL_INVEN_SLOTS / 2)
		{
			k = -10;
			count = 0;
		}
		else
			count++;
		Button* button = ButtonFactory::createNoTextButton("UIitem" + std::to_string(j + 1), 80 / TOTAL_INVEN_SLOTS + 10 + count * 10, (60 / 2) + k, 5, 5, mesh);
		buttonManager->addButton(button);
	}


	input = Input::GetInstance();

	for (int i = 0; i < sizeof(maxQuantity) / sizeof(maxQuantity[0]); i++)
	{
		maxQuantity[i] = 5;
	}

	this->scene = scene;
}

/**
 @brief Update Inventory
 @param dt A double for delta time
 */
void Inventory::Update(double dt)
{
	if (currentItem)
		currentItem->Update(dt);

	int vectorSize = itemVector.size();
	for (int i = 0; i < TOTAL_INVEN_SLOTS; i++)
	{
		if (i < vectorSize)
		{
			buttonManager->getButtonByName("UIitem" + std::to_string(i + 1))->setQuadImage(itemVector[i]->GetItemMesh());
		}
	}

	if (input->IsKeyPressed(VK_TAB))
	{
		enableInv = !enableInv;
	}

	if (input->IsKeyPressed(VK_LEFT))
	{
		std::cout << "cycle backward" << std::endl;
		CycleItem(false);
		std::cout << "qty: " << currentItem->GetQuantity() << std::endl;
	}
	if (input->IsKeyPressed(VK_RIGHT))
	{
		std::cout << "cycle forward" << std::endl;
		CycleItem(true);
		std::cout << "qty: " << currentItem->GetQuantity() << std::endl;
	}

	buttonManager->Update(scene, dt);
	for (auto& buttonCollide : buttonManager->getButtonsInteracted())
	{
		for (int i = 0; i < 3; i++)
		{
			if (buttonCollide->buttonClicked->getName() == ("UIitem" + std::to_string(i + 1)) && buttonCollide->justClicked)
			{
				buttonManager->deactivateButton("UIitem" + std::to_string(i + 1));
				SwitchItem(i);
			}
		}
	}
}

void Inventory::Render()
{
	if (enableInv)
		buttonManager->Render(scene);
}

/**
 @brief Cycle through current item with item vector
 */
void Inventory::CycleItem(bool forward)
{
	//check if item vector is empty, then return as there are no items to cycle through
	if (itemVector.empty())
		return;

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
	newItem->SetScene(scene);
	//check if item vector is empty, then just add the item into the item vector
	if (itemVector.empty())
	{
		itemVector.push_back(newItem);
		currentItem = newItem;
		return;
	}

	bool itemExist = false;
	bool isStackable = false;

	for (Item* item : itemVector)
	{
		//loop through entire item vector to check for the same item type
		if (newItem->GetType() == item->GetType())
		{
			//check if the item is stackable, and if its equal to one another
			if (item->GetIsStackable() && item->IsEqual(newItem))
			{
				//add the quantity to the existing item
				std::cout << "adding new qty to item: " << item->GetType() << std::endl;
				AddQuantity(item, newItem->GetQuantity());
				return;
			}
		}
	}

	//if cannot find an existing item or is existing item is not stackable, add it to the item vector
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

	//check if new qty exceeds max qty
	if (newQuantity > maxQuantity[item->GetType()])
	{
		//set item's qty to max, and return remainder
		int remainderQuantity = newQuantity - maxQuantity[item->GetType()];
		item->SetQuantity(maxQuantity[item->GetType()]);

		return remainderQuantity;
	}

	//else set qty to new qty
	item->SetQuantity(newQuantity);
	return 0;
}

Item::ITEM_TYPE Inventory::GetCurrentItemType()
{
	return currentItem->GetType();
}

Item* Inventory::GetCurrentItem()
{
	return currentItem;
}
