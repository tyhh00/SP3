#ifndef ITEM_H
#define ITEM_H

class Item {
protected:
	enum GROUP_TYPE {
		G_CONSUMABLE,
		G_WEAPON,
		G_TOTAL
	};

	enum ITEM_TYPE {
		I_APPLE,
		I_PISTOL,
		I_TOTAL
	};
public:
	Item();
	Item(GROUP_TYPE, ITEM_TYPE, int _quantity, int _maxQuantity, bool _isStackable);
	~Item();
	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual bool IsEqual(Item* item) = 0;

	GROUP_TYPE GetGroupType();
	ITEM_TYPE GetType();

	int GetQuantity();
	void SetQuantity(int _quantity);
	int AddQuantity(int _quantity);

	int GetMaxQuantity();
	void SetMaxQuantity(int _maxQuantity);

	bool GetIsStackable();
protected:
	GROUP_TYPE groupType;
	ITEM_TYPE itemType;
	int quantity;
	int maxQuantity;
	bool isStackable;
};


#endif