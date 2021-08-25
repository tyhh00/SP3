#pragma once
#include "Bullet.h"
#include "SpriteAnimation.h"

class LightBullet : public Bullet
{
public:
	LightBullet(Vector3 scale, GameObject* attachedPlayer, float _bulletSpeed);
	~LightBullet();

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

