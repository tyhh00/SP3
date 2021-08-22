#ifndef BATTERY_H
#define BATTERY_H
#include "Consumable.h"
#include "Input.h"

class Battery : public Consumable {
public:
	Battery(int _quantity = 1);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;
	
private:
	
	Input* input;
};


#endif