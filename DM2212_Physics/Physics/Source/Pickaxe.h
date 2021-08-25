#ifndef PICKAXE_H
#define PICKAXE_H
#include "Weapon.h"
#include "Input.h"

class Pickaxe : public Weapon {
public:
	Pickaxe(Mesh* mesh);
	~Pickaxe();

	void Init();
	void Update(double dt);
	bool IsEqual(Item* item1);
	
private:

	Input* input;
};


#endif