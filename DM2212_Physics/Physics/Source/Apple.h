#ifndef APPLE_H
#define APPLE_H
#include "Consumable.h"

class Apple : public Consumable {
public:
	Apple(int stemLength = 1);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;
	
private:
	int stemLength;
};


#endif