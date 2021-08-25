#ifndef WEAPON_H
#define WEAPON_H
#include "Item.h"

class Weapon : public Item {
public:
	Weapon(ITEM_TYPE type, Mesh* mesh);
	Weapon(ITEM_TYPE type);
	void Init() override;
	void Update(double dt) override;
	

private:
	
};


#endif