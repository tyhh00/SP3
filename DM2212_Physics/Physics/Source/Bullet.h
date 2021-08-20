#pragma once
#include "Mesh.h"

enum BULLET_TYPE
{
	PLASMA,
	BULLET_COUNT
};

class Bullet
{
public:


protected:
	Bullet(Mesh*, BULLET_TYPE type, Vector3 scale);
	~Bullet();

	Vector3 scale;

private:
	const BULLET_TYPE type;
};

