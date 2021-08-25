#ifndef BATTERY_H
#define BATTERY_H
#include "Consumable.h"
#include "Input.h"
#include "Inventory.h"

class Battery : public Consumable {
public:
	Battery(Mesh* mesh, Inventory* inventory, int _quantity = 1);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;
	
private:
	
	Input* input;

	Inventory* inventory;
};


#endif