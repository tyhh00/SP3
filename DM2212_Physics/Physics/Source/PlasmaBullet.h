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
public:
	PlasmaBullet(Vector3 scale, GameObject* attachedPlayer);
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

