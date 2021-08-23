#ifndef INVENTORY_H
#define INVENTORY_H

#include "GameObject.h"
#include "Item.h"
#include "Weapon.h"
#include "Consumable.h"
#include <vector>
#include "Input.h"
#include "Buttons/ButtonManager.h"
#include "Buttons/ButtonFactory.h"

class Inventory {
public:
	void Init(SceneBase* scene);
	void Update(double dt);
	void Render();

	void CycleItem(bool forward = true);
	void SwitchItem(int index);
	void AddItem(Item* newItem);
	int AddQuantity(Item* item, int _quantity);
	Item::ITEM_TYPE GetCurrentItemType();
	Item* GetCurrentItem();
private:
	ButtonManager* buttonManager;
	std::vector<Item*> itemVector;
	Item* currentItem;
	Input* input;
	
	//array to store all max Quantity
	int maxQuantity[Item::I_TOTAL];
	SceneBase* scene;

	bool enableInv = true;
	static const int TOTAL_INVEN_SLOTS = 10;
};


#endif