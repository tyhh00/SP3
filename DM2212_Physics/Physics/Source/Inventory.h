#ifndef INVENTORY_H
#define INVENTORY_H

#include "GameObject.h"
#include "Item.h"
#include <vector>

class Inventory {
public:
	void Init();
	void Update(double dt);

	void CycleItem();
	void AddItem(Item* newItem);
private:
	std::vector<Item*> itemVector;
	Item* currentItem;
};


#endif