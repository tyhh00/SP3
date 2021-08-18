#ifndef APPLE_H
#define APPLE_H
#include "Consumable.h"
#include "Keyboard.h"

class Apple : public Consumable {
public:
	Apple(int _quantity = 1, int stemLength = 1);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;
	
private:
	int stemLength;

	Keyboard* keyboard;
};


#endif