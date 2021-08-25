#pragma once
#include "Bullet.h"
#include "SpriteAnimation.h"

enum PLASMABULLET_STATE
{
	CHARGINGUP_PHASE1,
	CHARGINGUP_PHASE2,
	FULLYCHARGED,
	CHARGINGDOWN,
};

class PlasmaBullet : public Bullet
{
	const bool explo = true;
	const float rad = 3.0f;

	const float AOESpread = 2.0f; //How much power it loses as the explosion spreads
public:
	PlasmaBullet(Vector3 scale, GameObject* attachedPlayer, float bulletSpeed = 100);
	~PlasmaBullet();

	//GO Virtual Functions
	void Init();
	void Update(double dt);
	void CollidedWith(GameObject* go);
	GameObject* Clone();

private:
	SpriteAnimation* animatedSprite;
	PLASMABULLET_STATE state;
	double aliveTimer;

	//Custom GO Attachment for Position Updating
	GameObject* attachedPlayer;
};

