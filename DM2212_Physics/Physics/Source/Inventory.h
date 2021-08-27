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
	enum INVENTORY_TYPE
	{
		INVEN_ITEM,
		INVEN_CONSUMABLE,
		INVEN_WEAPON,
		INVEN_TOTAL
	};

	Inventory();
	~Inventory();
	void Init(SceneBase* scene);
	void Update(double dt);
	void Render();
	void Clear();

	void UpdateItemVector();
	void CycleItem(bool forward = true);
	void SwitchItem(INVENTORY_TYPE type, int index);
	void AddItem(Item* newItem);
	int AddQuantity(Item* item, int _quantity);
	void DeleteItem(Item* item);
	Item::ITEM_TYPE GetCurrentItemType();
	Item* GetCurrentItem();
	Item* GetItem(Item::ITEM_TYPE itemType);

private:

	INVENTORY_TYPE type;
	ButtonManager* buttonManager;
	std::vector<Item*> itemVector;
	std::vector<Weapon*> weaponVector;
	std::vector<Consumable*> consumableVector;
	Item* currentItem;
	Input* input;
	
	//array to store all max Quantity
	int maxQuantity[Item::I_TOTAL];
	SceneBase* scene;

	bool enableInv = true;
	static const int TOTAL_INVEN_SLOTS = 5;
	static const int TOTAL_WEAPON_SLOTS = 2;
	static const int TOTAL_CONSUMABLE_SLOTS = 3;

	Mesh* emptyMesh;
	Mesh* selectedMesh;
	Mesh* barMesh;
	Vector3 selectedPos;
};


#endif
