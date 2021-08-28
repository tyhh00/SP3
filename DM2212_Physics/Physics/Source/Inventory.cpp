/**
 Inventory Class
 By: Jordan Low
 Date: 17 Aug 2021
 */
#include "Inventory.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

Inventory::Inventory()
{
	currentItem = nullptr;
}

Inventory::~Inventory()
{
	for (Item* item : itemVector)
	{
		delete item;
	}
	itemVector.clear();

	weaponVector.clear();
	consumableVector.clear();

	if (emptyMesh)
	{
		delete emptyMesh;
	}
	if (selectedMesh)
	{
		delete selectedMesh;
	}
	if (buttonManager)
	{
		delete buttonManager;
	}
}

/**
 @brief Init Inventory
 */
void Inventory::Init(SceneBase* scene)
{
	emptyMesh = MeshBuilder::GenerateQuad("button", Color(1, 1, 1), 2.0f);
	emptyMesh->textureID = LoadTGA("Image/blackUI.tga");
	selectedMesh = MeshBuilder::GenerateQuad("button", Color(1, 1, 1), 2.0f);
	selectedMesh->textureID = LoadTGA("Image/blueUI.tga");
	barMesh = MeshBuilder::GenerateQuad("button", Color(1, 1, 1), 2.0f);

	buttonManager = new ButtonManager(80, 60);

	for (int i = 0; i < TOTAL_WEAPON_SLOTS; i++)
	{
		Button* button = ButtonFactory::createButton("WeaponItem" + std::to_string(i + 1), 80 * 0.9 + i * 5, (60 / 6), 2, 2, emptyMesh, 1.5, 3, Color(1, 1, 1), "", 2);
		Button* bar = ButtonFactory::createProgressBar("WeaponBar" + std::to_string(i + 1), 80 * 0.9 + i * 5, (60 / 6) - 2, 0.25f, 2, PROGRESSBAR_TYPE::HORIZONTAL, barMesh);
		buttonManager->addButton(button);
		buttonManager->addButton(bar);

		ProgressBar* pBar = dynamic_cast<ProgressBar*>(bar);
		pBar->SetProgress(0);
	}

	for (int i = 0; i < TOTAL_CONSUMABLE_SLOTS; i++)
	{
		Button* button = ButtonFactory::createButton("ConsumableItem" + std::to_string(i + 1), 80 * 0.9 + 5, (60 / 3) + i * 5, 2, 2, emptyMesh, 1.5, 3, Color(1, 1, 1), "", 2);
		Button* bar = ButtonFactory::createProgressBar("ConsumableBar" + std::to_string(i + 1), 80 * 0.9 + 5, (60 / 3) + (i * 5) - 2, 0.25f, 2, PROGRESSBAR_TYPE::HORIZONTAL, barMesh);
		buttonManager->addButton(button);
		buttonManager->addButton(bar);

		ProgressBar* pBar = dynamic_cast<ProgressBar*>(bar);
		pBar->SetProgress(0);
	}


	input = Input::GetInstance();

	for (int i = 0; i < sizeof(maxQuantity) / sizeof(maxQuantity[0]); i++)
	{
		maxQuantity[i] = 5;
	}
	maxQuantity[Item::I_BONE] = 15;
	maxQuantity[Item::I_SKULL] = 10;

	this->scene = scene;
}

/**
 @brief Update Inventory
 @param dt A double for delta time
 */
void Inventory::Update(double dt)
{
	if (currentItem)
	{
		currentItem->Update(dt);
		if (currentItem->GetQuantity() <= 0)
			DeleteItem(currentItem);
		else if (currentItem->GetDurability() <= 0)
		{
			currentItem->RemoveQuantity(1);
			if (currentItem->GetQuantity() > 0)
				currentItem->SetDurability(1);
		}
	}
	
	//update weapon slots
	int weaponSize = weaponVector.size();
	for (int i = 0; i < TOTAL_WEAPON_SLOTS; i++)
	{
		if (i < weaponSize)
		{
			buttonManager->getButtonByName("WeaponItem" + std::to_string(i + 1))->setQuadImage(weaponVector[i]->GetItemMesh());
		}
	}

	//update consumable slots
	int consumableSize = consumableVector.size();
	for (int i = 0; i < TOTAL_CONSUMABLE_SLOTS; i++)
	{
		if (i < consumableSize)
		{
			buttonManager->getButtonByName("ConsumableItem" + std::to_string(i + 1))->setQuadImage(consumableVector[i]->GetItemMesh());
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
		if(currentItem)
			std::cout << "qty: " << currentItem->GetQuantity() << std::endl;
	}
	if (input->IsKeyPressed(VK_RIGHT))
	{
		std::cout << "cycle forward" << std::endl;
		CycleItem(true);
		if(currentItem)
			std::cout << "qty: " << currentItem->GetQuantity() << std::endl;
	}

	if (input->IsKeyPressed('1'))
	{
		SwitchItem(INVEN_WEAPON, 0);
		selectedPos = Vector3(buttonManager->getButtonByName("WeaponItem1")->getUIInfo().originX, buttonManager->getButtonByName("WeaponItem1")->getUIInfo().originY, 0);
	}
	else if (input->IsKeyPressed('2'))
	{
		SwitchItem(INVEN_WEAPON, 1);
		selectedPos = Vector3(buttonManager->getButtonByName("WeaponItem2")->getUIInfo().originX, buttonManager->getButtonByName("WeaponItem2")->getUIInfo().originY, 0);
	}
	else if (input->IsKeyPressed('3'))
	{
		SwitchItem(INVEN_CONSUMABLE, 0);
		selectedPos = Vector3(buttonManager->getButtonByName("ConsumableItem1")->getUIInfo().originX, buttonManager->getButtonByName("ConsumableItem1")->getUIInfo().originY, 0);
	}
	else if (input->IsKeyPressed('4'))
	{
		SwitchItem(INVEN_CONSUMABLE, 1);
		selectedPos = Vector3(buttonManager->getButtonByName("ConsumableItem2")->getUIInfo().originX, buttonManager->getButtonByName("ConsumableItem2")->getUIInfo().originY, 0);
	}
	else if (input->IsKeyPressed('5'))
	{
		SwitchItem(INVEN_CONSUMABLE, 2);
		selectedPos = Vector3(buttonManager->getButtonByName("ConsumableItem3")->getUIInfo().originX, buttonManager->getButtonByName("ConsumableItem3")->getUIInfo().originY, 0);
	}


	//buttonManager->Update(scene, dt);
	//for (auto& buttonCollide : buttonManager->getButtonsInteracted())
	//{
	//	for (int i = 0; i < 3; i++)
	//	{
	//		if (buttonCollide->buttonClicked->getName() == ("UIitem" + std::to_string(i + 1)) && buttonCollide->justClicked)
	//		{
	//			SwitchItem(i);
	//		}
	//	}
	//}
	itemVector.erase(std::remove(itemVector.begin(), itemVector.end(), nullptr), itemVector.end());
}

void Inventory::Render()
{
	if (enableInv)
	{
		//update weapon slots
		int weaponSize = weaponVector.size();
		for (int i = 0; i < TOTAL_WEAPON_SLOTS; i++)
		{
			if (i < weaponSize)
			{
				buttonManager->getButtonByName("WeaponItem" + std::to_string(i + 1))->setText(std::to_string(weaponVector[i]->GetQuantity()));
				if (weaponVector[i]->GetIsDurable())
				{
					ProgressBar* bar = dynamic_cast<ProgressBar*>(buttonManager->getButtonByName("WeaponBar" + std::to_string(i + 1)));
					bar->SetProgress(weaponVector[i]->GetDurability());
				}
			}
			else
			{
				buttonManager->getButtonByName("WeaponItem" + std::to_string(i + 1))->setText("");
			}
		}

		//update consumable slots
		int consumableSize = consumableVector.size();
		for (int i = 0; i < TOTAL_CONSUMABLE_SLOTS; i++)
		{
			if (i < consumableSize)
			{
				buttonManager->getButtonByName("ConsumableItem" + std::to_string(i + 1))->setText(std::to_string(consumableVector[i]->GetQuantity()));
				std::cout << "QTY: " << consumableVector[i]->GetQuantity() << std::endl;
				if (consumableVector[i]->GetIsDurable())
				{
					ProgressBar* bar = dynamic_cast<ProgressBar*>(buttonManager->getButtonByName("ConsumableBar" + std::to_string(i + 1)));
					bar->SetProgress(consumableVector[i]->GetDurability());
				}
			}
			else
			{
				buttonManager->getButtonByName("ConsumableItem" + std::to_string(i + 1))->setText("");
			}
		}		
		buttonManager->Render(scene);
	}

	if (!selectedPos.IsZero())
		scene->RenderMeshOnScreen(selectedMesh, selectedPos.x, selectedPos.y, 2.2f, 2.2f, 2);
}

/**
 @brief Clear all inventory items
 */
void Inventory::Clear()
{
	for (Item* item : itemVector)
	{
		delete item;
	}
	itemVector.clear();


	weaponVector.clear();
	consumableVector.clear();
	currentItem = nullptr;
	if (emptyMesh)
	{
		delete emptyMesh;
		emptyMesh = NULL;
	}
	if (selectedMesh)
	{
		delete selectedMesh;
		selectedMesh = NULL;
	}
	if (buttonManager)
	{
		delete buttonManager;
		buttonManager = NULL;
	}
}

/**
 @brief Update the weapon and consumable vectors based on the item vector
 */
void Inventory::UpdateItemVector()
{
	weaponVector.clear();
	consumableVector.clear();
	for (int i = 0; i < TOTAL_WEAPON_SLOTS; i++)
	{
		buttonManager->getButtonByName("WeaponItem" + std::to_string(i + 1))->setQuadImage(emptyMesh);
	}
	for (int i = 0; i < TOTAL_CONSUMABLE_SLOTS; i++)
	{
		buttonManager->getButtonByName("ConsumableItem" + std::to_string(i + 1))->setQuadImage(emptyMesh);
	}

	for (Item* item : itemVector)
	{
		if (item == nullptr)
			continue;

		if (item->GetGroupType() == Item::G_CONSUMABLE)
		{
			Consumable* newConsumable = dynamic_cast<Consumable*>(item);
			consumableVector.push_back(newConsumable);
		}
		else if (item->GetGroupType() == Item::G_WEAPON)
		{
			Weapon* newWeapon = dynamic_cast<Weapon*>(item);
			weaponVector.push_back(newWeapon);
		}
	}
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
			if(currentItem)
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
 @param type An inventory type for the respective vector that you want to switch to
 @param index An Int for the index of the vector
 */
void Inventory::SwitchItem(INVENTORY_TYPE type, int index)
{
	switch (type)
	{
	case INVEN_ITEM:
		if (index >= itemVector.size())
		{
			currentItem = nullptr;
			break;
		}
		currentItem = itemVector[index];
		break;
	case INVEN_CONSUMABLE:
		if (index >= consumableVector.size())
		{
			currentItem = nullptr;
			break;
		}		
		currentItem = consumableVector[index];
		break;
	case INVEN_WEAPON:
		if (index >= weaponVector.size())
		{
			currentItem = nullptr;
			break;
		}		
		currentItem = weaponVector[index];
		break;
	}
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
		UpdateItemVector();
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
				delete newItem;
				newItem = nullptr;
				return;
			}
		}
	}

	if (newItem->GetGroupType() == Item::G_CONSUMABLE)
	{
		if (consumableVector.size() >= TOTAL_CONSUMABLE_SLOTS)
			return;
	}
	else if (newItem->GetGroupType() == Item::G_WEAPON)
	{
		if (weaponVector.size() >= TOTAL_WEAPON_SLOTS)
			return;
	}
	//if cannot find an existing item or is existing item is not stackable, add it to the item vector
	std::cout << "adding new item to vector" << std::endl;
	itemVector.push_back(newItem);
	currentItem = newItem;

	UpdateItemVector();
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

void Inventory::DeleteItem(Item* delItem)
{
	for (std::vector<Item*>::iterator it = itemVector.begin(); it != itemVector.end(); ++it)
	{
		Item* item = (Item*)*it;
		if (item == delItem)
		{
			delete item;
			item = nullptr;
			currentItem = nullptr;
			break;
		}
	}
	UpdateItemVector();
}

Item::ITEM_TYPE Inventory::GetCurrentItemType()
{
	if(currentItem)
		return currentItem->GetType();
	return Item::ITEM_TYPE::I_TOTAL; //Non Existent
}

Item* Inventory::GetCurrentItem()
{
	return currentItem;
}

Item* Inventory::GetItem(Item::ITEM_TYPE itemType)
{
	for (auto item : itemVector)
	{
		if (item->GetType() == itemType)
		{
			return item;
		}
	}
	return nullptr;
}
