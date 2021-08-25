#ifndef WEAPON_H
#define WEAPON_H
#include "Item.h"
#include "BulletSpawner.h"

class Weapon : public Item {
public:
	Weapon(ITEM_TYPE type, Mesh* mesh, BulletSpawner* bulletSpawner = nullptr);
	Weapon(ITEM_TYPE type, BulletSpawner* bulletSpawner = nullptr);
	void Init() override;
	void Update(double dt) override;
	
	BulletSpawner* GetBulletSpawner();

protected:
	BulletSpawner* bulletSpawner;
};


#endif