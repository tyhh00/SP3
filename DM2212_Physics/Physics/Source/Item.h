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
	Item(GROUP_TYPE, ITEM_TYPE, int);
	~Item();
	virtual void Init() = 0;
	virtual void Update(double dt) = 0;

	GROUP_TYPE GetGroupType();
	ITEM_TYPE GetType();

	int GetQuantity();
	void SetQuantity(int _quantity);
private:
	GROUP_TYPE groupType;
	ITEM_TYPE itemType;
	int quantity;
};


#endif