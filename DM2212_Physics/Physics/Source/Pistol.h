#ifndef PISTOL_H
#define PISTOL_H
#include "Weapon.h"
#include "Input.h"
#include "GameObjectManager.h"
#include "LightBullet.h"

class Pistol : public Weapon {
public:
	Pistol(GameObjectManager* goManager, LightBullet* _lightBullet, Mesh* mesh);
	~Pistol();

	void Init();
	void Update(double dt);
	bool IsEqual(Item* item1);
	
private:
	Input* input;
	LightBullet* lightBullet;
};


#endif