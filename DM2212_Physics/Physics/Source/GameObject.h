#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "Material.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,

		GO_BALL,
		GO_CUBE,

		
		GO_WALL, // INELASTIC
		GO_PILLAR, // INELASTIC

		GO_TOTAL, //must be last
	};

	GAMEOBJECT_TYPE type;
	bool active;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;

	float rotateZ;
	Vector3 dir;

	Vector3 normal;
	float momentOfInertia;
	float angularVelocity;
	float mass;



	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif