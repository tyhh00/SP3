#ifndef WEAPON_H
#define WEAPON_H
#include "Item.h"

class Weapon : public Item {
public:
	Weapon();
	void Init() override;
	void Update(double dt) override;
	

private:
	
};


#endif