#ifndef INVENTORY_H
#define INVENTORY_H

#include "GameObject.h"
#include "Item.h"
#include <vector>
#include "Keyboard.h"

class Inventory {
public:
	void Init();
	void Update(double dt);

	void CycleItem(bool forward = true);
	void SwitchItem(int index);
	void AddItem(Item* newItem);
	int AddQuantity(Item* item, int _quantity);
	Item::ITEM_TYPE GetCurrentItemType();
	Item* GetCurrentItem();
private:
	std::vector<Item*> itemVector;
	Item* currentItem;
	Keyboard* keyboard;
	
	//array to store all max Quantity
	int maxQuantity[Item::I_TOTAL];
};


#endif