#pragma once
#include "Bullet.h"
#include "SpriteAnimation.h"

class BananaBullet : public Bullet
{
public:
	BananaBullet(Vector3 scale, GameObject* attachedPlayer, float _bulletSpeed);
	~BananaBullet();

	//GO Virtual Functions
	void Init();
	void Update(double dt);
	void CollidedWith(GameObject* go);
	GameObject* Clone();

private:
	double aliveTimer;

	//Custom GO Attachment for Position Updating
	GameObject* attachedPlayer;
};

