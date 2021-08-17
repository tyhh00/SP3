#ifndef APPLE_H
#define APPLE_H
#include "Consumable.h"

class Apple : public Consumable {
public:
	Apple();
	void Init() override;
	void Update(double dt) override;
	
private:
	
};


#endif