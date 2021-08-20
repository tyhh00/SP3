#pragma once
#include "Bullet.h"
#include "SpriteAnimation.h"

class PlasmaBullet : public Bullet
{
	const bool explo = true;
	const float rad = 3.0f;
public:
	PlasmaBullet(Mesh* mesh, int geoTypeID, Vector3 scale);
	~PlasmaBullet();

	//GO Virtual Functions
	void Init();
	void Update(double dt);
	void CollidedWith(GameObject* go);
	PlasmaBullet* Clone();

private:
	SpriteAnimation* animatedSpirte;
};

