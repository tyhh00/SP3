#pragma once

#include "Bullet.h"

enum BLACKHOLEBULLET_STATE
{
	ROLLING,
	ACTIVE,
	SCALINGDOWN,
};

class BlackHoleBullet : public Bullet
{
public:
	BlackHoleBullet(Mesh* mesh, int geoTypeID, Vector3 scale, GameObject* attachedPlayer, float bulletSpeed = 35);
	~BlackHoleBullet();

	//GO Virtual Functions
	void Init();
	void Update(double dt);
	void CollidedWith(GameObject* go);
	GameObject* Clone();

private:
	float spriteRotation;
	float spriteRotationSpeed; //Radians turned per second
	BLACKHOLEBULLET_STATE state;
	double aliveTimer;

	Vector3 maxScale;
	Vector3 normalisedScale;
	float scaleSpeed;

	//Custom GO Attachment for Position Updating
	GameObject* attachedPlayer;

	float deltaTime; //For CollidedWith Function
};

